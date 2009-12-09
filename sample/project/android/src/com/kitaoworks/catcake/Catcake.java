/*
    Copyright (c) 2007-2009 Takashi Kitao
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
    `  notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
    `  notice, this list of conditions and the following disclaimer in the
    `  documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products
    `  derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


package com.kitaoworks.catcake;

import java.io.IOException;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.os.Bundle;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;

import android.content.Context;
import android.view.MotionEvent;


public class Catcake
{
    private CatcakeView m_view;
    private CatcakeRenderer m_renderer;

    public void onCreate(Activity activity)
    {
        activity.getWindow().addFlags(android.view.WindowManager.LayoutParams.FLAG_FULLSCREEN);
        activity.requestWindowFeature(android.view.Window.FEATURE_NO_TITLE);

        m_view = new CatcakeView(this);
        m_renderer = new CatcakeRenderer();

        m_view.setRenderer(m_renderer);

        activity.setContentView(m_view);
    }

    public void onPause()
    {
        m_view.onPause();
    }

    public void onResume()
    {
        m_view.onResume();
    }

    public void onDestroy()
    {
        nativeFinish();
    }

    private static native void nativeInitialize();
    private static native void nativeUpdate();
    private static native void nativeFinalize();
    private static native void nativeOnTouch(int action, int x, int y);
    //private static native void nativeOnKeyDown(int key_code);
    //private static native void nativeOnKeyUp(int key_code);

    private class CatcakeView extends GLSurfaceView
    {
        @Override public void setRenderer(Renderer renderer)
        {
            super.setRenderer(renderer);
        }

        @Override public boolean onTouchEvent(MotionEvent event)
        {
            nativeOnTouch(event.getAction(), (int)event.getX(), (int)event.getY());

            return true;
        }
    }

    private class CatcakeRenderer implements Renderer
    {
        boolean m_is_first = true;

        @Override public void onSurfaceCreated(GL10 gl, EGLConfig config) {}

        @Override public void onSurfaceChanged(GL10 gl, int width, int height) {}

        @Override public void onDrawFrame(GL10 gl)
        {
            if (m_is_first)
            {
                m_is_first = false;

                nativeInit();
            }

            nativeUpdateFrame();
        }
    }
}
