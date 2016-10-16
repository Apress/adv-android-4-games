#include <stdio.h>
#include <stdlib.h>


#include <gl.h>
#include <egl.h>
#include "nanogl.h"




int main (int argc, char ** argv)
{
	

	nanoGL_Init();

	int x = eglGetError();
	printf("eglGetError = %d\n", x);

	glBegin(GL_POLYGON);
	glColor4f(1,1,1,1);
	glEnd();

	printf("eglGetError = %d\n", eglGetError());

	nanoGL_Destroy();

	exit (0);
}
