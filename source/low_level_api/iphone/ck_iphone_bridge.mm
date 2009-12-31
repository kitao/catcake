/*
    Copyright (c) 2007-2010 Takashi Kitao

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/


#ifdef CK_IPHONE


#import "ck_iphone_bridge.h"

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>


void teardown()
{
}

void sig_handler(int ignored)
{
    teardown();
    exit(0);
}

@interface OpenGLView : UIView
{
    NSTimer* timer;
    UIApplication* m_app;
    CGPoint m_mouse_pos;
    CGRect m_window_rect;

    CGPoint m_inner_mouse_pos; // TODO
    CGPoint m_outer_mouse_pos; // TODO

    @private

    GLint backingWidth;
    GLint backingHeight;

    EAGLContext *context;

    GLuint viewRenderbuffer, viewFramebuffer;

    GLuint depthRenderbuffer;

    BOOL bufferClear;
}

- (id)initGLES;
- (BOOL)createFramebuffer;
- (void)destroyFramebuffer;
-(void)drawView;

@end

@implementation OpenGLView


+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithCoder:(NSCoder*)coder
{
    if((self = [super initWithCoder:coder]))
    {
        self = [self initGLES];
    }
    return self;
}

-(id)initGLES
{
    CAEAGLLayer *eaglLayer = (CAEAGLLayer*) self.layer;

    eaglLayer.opaque = YES;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];

    context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    if(!context || ![EAGLContext setCurrentContext:context] || ![self createFramebuffer])
    {
        [self release];
        return nil;
    }

    if (timer) {
        [timer invalidate];
    }
    timer =
    [
        NSTimer scheduledTimerWithTimeInterval:1.0 / 30.0
        target:self
        selector:@selector(FireTimer:)
        userInfo:nil
        repeats:YES
    ];

    return self;
}

-(void)layoutSubviews
{
    [EAGLContext setCurrentContext:context];
    [self destroyFramebuffer];
    [self createFramebuffer];
    [self drawView];
}

- (BOOL)createFramebuffer
{
    glGenFramebuffersOES(1, &viewFramebuffer);
    glGenRenderbuffersOES(1, &viewRenderbuffer);

    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);

    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(id<EAGLDrawable>)self.layer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);

    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);

    glGenRenderbuffersOES(1, &depthRenderbuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
    glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);

    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
    {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return NO;
    }

    return YES;
}

- (void)destroyFramebuffer
{
    glDeleteFramebuffersOES(1, &viewFramebuffer);
    viewFramebuffer = 0;
    glDeleteRenderbuffersOES(1, &viewRenderbuffer);
    viewRenderbuffer = 0;

    if(depthRenderbuffer)
    {
        glDeleteRenderbuffersOES(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
}

- (void)drawView
{
    [EAGLContext setCurrentContext:context];

    if (!bufferClear) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        bufferClear = YES;
    }


    glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);

    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];

    GLenum err = glGetError();
    if(err)
        NSLog(@"%x error", err);
}

- (void)dealloc
{
    if([EAGLContext currentContext] == context)
    {
        [EAGLContext setCurrentContext:nil];
    }

    [context release];
    context = nil;

    [super dealloc];
}

- (id)initWithFrame:(CGRect)rect
{
    m_window_rect = rect;

    self = [super initWithFrame:rect];
    if(self != nil)
    {
        self = [self initGLES];
    }
    return self;
}

- (void)drawRect:(CGRect)rect
{
    return [super drawRect:rect];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
{
    NSSet *allTouches = [event allTouches];
    UITouch *touch;

    int num = [allTouches count];
    if (num >= 2) num = 2;
    switch (num) {
    case 2:
        touch = [[allTouches allObjects] objectAtIndex:1];
        m_outer_mouse_pos = [touch locationInView:self];
        ckIPhoneBridge::setMouseState(1, true);
    case 1:
        touch = [[allTouches allObjects] objectAtIndex:0];
        m_mouse_pos = [touch locationInView:self];
        m_inner_mouse_pos = m_mouse_pos;
        ckIPhoneBridge::setMouseState(0, true);
        break;
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
{
    NSSet *allTouches = [event allTouches];
    UITouch *touch;
    int num = [allTouches count];
    if (num >= 2) num = 2;

    switch (num) {
    case 2:
        touch = [[allTouches allObjects] objectAtIndex:1];
        m_outer_mouse_pos = [touch locationInView:self];
        ckIPhoneBridge::setMouseState(1, true);
    case 1:
        touch = [[allTouches allObjects] objectAtIndex:0];
        m_mouse_pos = [touch locationInView:self];
        m_inner_mouse_pos = m_mouse_pos;
        ckIPhoneBridge::setMouseState(0, true);
        break;
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
{
    NSSet *allTouches = [event allTouches];
    UITouch *touch;
    int num = [allTouches count];
    int end_num = [touches count];
    int i, j;
    BOOL found;
    int out = 0;

    for (i = 0; i < num && out < 2; i++) {
        found = FALSE;
        for (j = 0; j < end_num && (found != TRUE); j++) {
            if ([[allTouches allObjects] objectAtIndex:i] == [[touches allObjects] objectAtIndex:j]) {
                found = TRUE;
            }
        }
        if (found != TRUE) {
            if (out == 0) {
                touch = [[allTouches allObjects] objectAtIndex:i];
                m_mouse_pos = [touch locationInView:self];
                m_inner_mouse_pos = m_mouse_pos;
                ckIPhoneBridge::setMouseState(0, true);
            } else if (out == 1) {
                touch = [[allTouches allObjects] objectAtIndex:i];
                m_outer_mouse_pos = [touch locationInView:self];
                ckIPhoneBridge::setMouseState(1, true);
            }
            out++;
        }
    }


    switch (out) {
    case 1:
        ckIPhoneBridge::setMouseState(1, false);
        break;
    case 0:
        ckIPhoneBridge::setMouseState(0, false);
        ckIPhoneBridge::setMouseState(1, false);
        break;
    }
}

- (void)touchesCanceled:(NSSet *)touches withEvent:(UIEvent *)event;
{
}

- (void)SetApplication:(UIApplication*)app
{
    m_app = app;
}

- (void)FireTimer:(NSTimer*)unused
{
    [self drawView];

    ckIPhoneBridge::updateApplication();
}

- (CGPoint&)GetMousePos;
{
    return m_mouse_pos;
}

- (CGPoint&)GetInnerMousePos;
{
    return m_inner_mouse_pos;
}

- (CGPoint&)GetOuterMousePos;
{
    return m_outer_mouse_pos;
}

@end

@interface CatcakeApp : UIApplication
{
    OpenGLView* m_view;
    int m_orientation;
    float m_accel_x, m_accel_y, m_accel_z;
}

@end

CatcakeApp* g_app;

@implementation CatcakeApp

- (void)applicationDidFinishLaunching:(id)unused
{
    signal(SIGINT, &sig_handler);

    CGRect rect = [[UIScreen mainScreen] bounds];
    UIWindow *window = [[UIWindow alloc] initWithFrame:rect];

    m_view = [[OpenGLView alloc] initWithFrame:rect];
    m_view.multipleTouchEnabled = YES;
    [window addSubview:m_view];

    g_app = self;

    [window makeKeyAndVisible];

    m_orientation = 0;
    m_accel_x = m_accel_y = m_accel_z = 0.0f;

    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter] addObserver:self
        selector:@selector(didRotate:)
        name:@"UIDeviceOrientationDidChangeNotification"
        object:nil];
}

- (void)didRotate:(NSNotification *)notification
{
    m_orientation = [[UIDevice currentDevice] orientation];
}

- (void)acceleratedInX:(float)xx Y:(float)yy Z:(float)zz;
{
    m_accel_x = xx;
    m_accel_y = yy;
    m_accel_z = zz;
}

/*
- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
    [self acceleratedInX:acceleration.x Y:acceleration.y Z:acceleration.z];
}
*/

- (CGPoint&)GetMousePos;
{
    return [m_view GetMousePos];
}

- (CGPoint&)GetInnerMousePos;
{
    return [m_view GetInnerMousePos];
}

- (CGPoint&)GetOuterMousePos;
{
    return [m_view GetOuterMousePos];
}

- (int)GetDeviceOrientation;
{
    return m_orientation;
}

- (float)GetAcceleraionX;
{
    return m_accel_x;
}

- (float)GetAcceleraionY;
{
    return m_accel_y;
}

- (float)GetAcceleraionZ;
{
    return m_accel_z;
}


@end


bool ckIPhoneBridge::createFramebuffer()
{
    return true;
}


void ckIPhoneBridge::destroyFramebuffer() {}


void ckIPhoneBridge::swapFramebuffer()
{
}


void ckIPhoneBridge::startApplication()
{
    [[NSAutoreleasePool alloc] init];

    UIApplicationMain(0, NULL, @"CatcakeApp", @"CatcakeApp");
}


int ckIPhoneBridge::getFramebufferWidth()
{
    return 320;
}


int ckIPhoneBridge::getFramebufferHeight()
{
    return 480;
}


void ckIPhoneBridge::getMousePos(int* x, int* y)
{
    CGPoint& pos = [g_app GetMousePos];

    *x = pos.x;
    *y = pos.y;
}


void ckIPhoneBridge::getInnerMousePos(int* x, int* y)
{
    CGPoint& pos = [g_app GetInnerMousePos];

    *x = pos.x;
    *y = pos.y;
}


void ckIPhoneBridge::getOuterMousePos(int* x, int* y)
{
    CGPoint& pos = [g_app GetOuterMousePos];

    *x = pos.x;
    *y = pos.y;
}


int ckIPhoneBridge::getDeviceOrientation()
{
    return [g_app GetDeviceOrientation];
}


void ckIPhoneBridge::getAcceleraion(float* x, float* y, float* z)
{
    *x = [g_app GetAcceleraionX];
    *y = [g_app GetAcceleraionY];
    *z = [g_app GetAcceleraionZ];
}


#endif // CK_IPHONE
