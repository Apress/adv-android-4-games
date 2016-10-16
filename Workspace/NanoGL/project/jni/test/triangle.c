#include <stdio.h>
#include <stdlib.h>
/*
#include <GLES/gl.h>
#include <EGL/egl.h>
*/
#include <gl.h>
#include <egl.h>
#include "nanogl.h"

#include <EGL/eglnatives.h>
typedef struct egl_native_window_t*     EGLNativeWindowType;


const EGLint config16bpp[] =
{
EGL_RED_SIZE, 5,
EGL_GREEN_SIZE, 6,
EGL_BLUE_SIZE, 5,
EGL_NONE
};

GLfloat colors[3][4] =
{
    {1.0f, 0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f, 1.0f}
};

GLfloat vertices[3][3] =
{
    {0.0f, 0.7f, 0.0f},
    {-0.7f, -0.7f, 0.0f},
    {0.7f, -0.7f, 0.0f}
};

void draw_tri(int width, int height)
{
    glViewport(0,0,width,height);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	
	int i = 0;
	glBegin(GL_TRIANGLES);
	for ( i = 0 ; i < 3 ; i++) {
		glColor4fv(colors[i]);
		glVertex3fv(vertices[i]);
	}
	glEnd();
/*
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(4, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    // Draw the triangle (3 vertices)
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
*/	
}

EGLContext eglContext;
EGLSurface eglSurface;
EGLConfig eglConfig;
EGLDisplay eglDisplay;
EGLNativeWindowType displayWindow;

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
	printf("NanoGL simple test\n");
	printf("Init\n");
	
	nanoGL_Init();
	initGL();

	int w = displayWindow->width;
	int h = displayWindow->height;
	
	int nloops = 200;
	int loop = 0;
    while (loop < nloops)
    {
    	draw_tri(w, h);
    	eglSwapBuffers(eglDisplay, eglSurface);
		loop++;
    }
	
	eglDestroyContext(eglDisplay, eglContext);
	//eglDestroySurface(eglDisplay, eglSurface);
	
	nanoGL_Destroy();
/*
	nanoGL_Init();

	int x = eglGetError();
	printf("eglGetError = %d\n", x);

	glBegin(GL_POLYGON);
	glColor4f(1,1,1,1);
	glEnd();

	printf("eglGetError = %d\n", eglGetError());

	nanoGL_Destroy();
*/
	exit (0);
}
