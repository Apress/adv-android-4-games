/* San Angeles Observation OpenGL ES version example
 * Copyright 2009 The Android Open Source Project
 * All rights reserved.
 *
 * This source is free software; you can redistribute it and/or
 * modify it under the terms of EITHER:
 *   (1) The GNU Lesser General Public License as published by the Free
 *       Software Foundation; either version 2.1 of the License, or (at
 *       your option) any later version. The text of the GNU Lesser
 *       General Public License is included with this source in the
 *       file LICENSE-LGPL.txt.
 *   (2) The BSD-style license that is included with this source in
 *       the file LICENSE-BSD.txt.
 *
 * This source is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files
 * LICENSE-LGPL.txt and LICENSE-BSD.txt for more details.
 */
//#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <EGL/egl.h>
/*
#include <egl.h>
#include "nanogl.h"

#include <EGL/eglnatives.h>
typedef struct egl_native_window_t*     EGLNativeWindowType;
*/
int   gAppAlive   = 1;

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;

static long
_getTime(void)
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}

/* Call to initialize the graphics state */
void
//Java_com_example_SanAngeles_DemoRenderer_nativeInit( JNIEnv*  env )
nativeInit()
{
    //importGLInit();
    appInit();
    gAppAlive    = 1;
    sDemoStopped = 0;
    sTimeOffsetInit = 0;
}

/*
void
Java_com_example_SanAngeles_DemoRenderer_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    __android_log_print(ANDROID_LOG_INFO, "SanAngeles", "resize w=%d h=%d", w, h);
}
*/
/* Call to finalize the graphics state */
void
//Java_com_example_SanAngeles_DemoRenderer_nativeDone( JNIEnv*  env )
nativeDone()
{
    appDeinit();
    importGLDeinit();
}

/* This is called to indicate to the render loop that it should
 * stop as soon as possible.
 */
/* 
void
Java_com_example_SanAngeles_DemoGLSurfaceView_nativePause( JNIEnv*  env )
{
    sDemoStopped = !sDemoStopped;
    if (sDemoStopped) {
        sTimeStopped = _getTime();
    } else {
        sTimeOffset -= _getTime() - sTimeStopped;
    }
}
*./

/* Call to render the next GL frame */
void
//Java_com_example_SanAngeles_DemoRenderer_nativeRender( JNIEnv*  env )
nativeRender()
{
    long   curTime;

    /* NOTE: if sDemoStopped is TRUE, then we re-render the same frame
     *       on each iteration.
     */
    if (sDemoStopped) {
        curTime = sTimeStopped + sTimeOffset;
    } else {
        curTime = _getTime() + sTimeOffset;
        if (sTimeOffsetInit == 0) {
            sTimeOffsetInit = 1;
            sTimeOffset     = -curTime;
            curTime         = 0;
        }
    }

    //__android_log_print(ANDROID_LOG_INFO, "SanAngeles", "curTime=%ld", curTime);

    appRender(curTime, sWindowWidth, sWindowHeight);
}

EGLContext eglContext;
EGLSurface eglSurface;
EGLConfig eglConfig;
EGLDisplay eglDisplay;
EGLNativeWindowType displayWindow;

const EGLint config16bpp[] =
{
	EGL_RED_SIZE, 5,
	EGL_GREEN_SIZE, 6,
	EGL_BLUE_SIZE, 5,
	EGL_NONE
};

static void initGL()
{
	EGLint majorVersion, minorVersion;
    int numConfigs;

// Display
	eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	
	printf("EGL Display %p\n", eglDisplay);
	
 // Window surface that covers the entire screen, from libui.
    displayWindow = android_createDisplaySurface();

	int w = displayWindow->width;
	int h = displayWindow->height;
	
    printf("Window specs: %d*%d format=%d\n", w, h , displayWindow->format);

	// Init	
    eglInitialize(eglDisplay, &majorVersion, &minorVersion);
    printf("GL version: %d.%d\n",majorVersion,minorVersion);

	// Choose config
	if (!eglChooseConfig(eglDisplay, config16bpp, &eglConfig, 1, &numConfigs))
    {
    	printf("eglChooseConfig failed\n");
    	if (eglContext==0) printf("Error code: %x\n", eglGetError());
    }

	// create context
    eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, NULL);

    if (eglContext==0) {
		printf("Error code: %x\n", eglGetError());
		exit(-1);
	}
    printf("GL context: %x\n", eglContext);
	
	// create surface
    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, displayWindow,  NULL);
	
    if (eglSurface==0) {
		printf("Error code: %x\n", eglGetError());
		exit (-1);
	}
	printf("GL surface: %x\n", eglSurface);

	// make current
    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

}

int main (int argc, char ** argv)
{
	printf("Angeles Init\n");
	
//	nanoGL_Init();
	
	initGL();
	nativeInit();
	
	int nloops = 100;
	int loop = 0;
    while (loop < nloops)
    {
		nativeRender();
    	eglSwapBuffers(eglDisplay, eglSurface);
		loop++;
		printf("loop %d/%d\n", loop, nloops);
	}
	
	
	printf("Angeles Done\n");
}
