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


package com.kitaoworks.catcake;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;

import android.content.Context;
import android.view.MotionEvent;


public class Catcake
{
    private CatcakeView m_view;
    private CatcakeRenderer m_renderer;

    public Catcake(String lib_name)
    {
        System.loadLibrary(lib_name);
    }

    public void onCreate(Activity activity)
    {
        activity.getWindow().addFlags(android.view.WindowManager.LayoutParams.FLAG_FULLSCREEN);
        activity.requestWindowFeature(android.view.Window.FEATURE_NO_TITLE);

        m_view = new CatcakeView(activity);
        m_renderer = new CatcakeRenderer();

        m_view.setRenderer(m_renderer);

        activity.setContentView(m_view);
    }

    public void onPause()
    {
        m_view.onPause();
        nativePause();
    }

    public void onResume()
    {
        m_view.onResume();
        nativeResume();
    }

    public void onDestroy()
    {
        nativeFinalize();
    }

    private static native void nativeInitialize();
    private static native void nativeUpdate();
    private static native void nativeFinalize();
    private static native void nativePause();
    private static native void nativeResume();
    private static native void nativeTouch(int action, int x, int y);
    //private static native void nativeOnKeyDown(int key_code);
    //private static native void nativeOnKeyUp(int key_code);

    private class CatcakeView extends GLSurfaceView
    {
        public CatcakeView(Context context)
        {
            super(context);
        }

        @Override public void setRenderer(Renderer renderer)
        {
            super.setRenderer(renderer);
        }

        @Override public boolean onTouchEvent(MotionEvent event)
        {
            nativeTouch(event.getAction(), (int)event.getX(), (int)event.getY());

            return true;
        }
    }

    private class CatcakeRenderer implements Renderer
    {
        boolean m_is_first = true;

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {}

        public void onSurfaceChanged(GL10 gl, int width, int height) {}

        public void onDrawFrame(GL10 gl)
        {
            if (m_is_first)
            {
                m_is_first = false;

                nativeInitialize();
            }

            nativeUpdate();
        }
    }
}
