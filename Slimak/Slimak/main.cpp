/*
* GKOM OpenGL Slimak
* PROJEKT UZYWA GLUT.H I LADOWANIA OBRAZKOW SOIL.H
*/
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include "glut.h"
#include "SOIL.h"
#include <gl/GL.h>

#define _USE_MATH_DEFINES
#include <cmath>
GLuint tex[6];
GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 2.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 10.0f, 0.0f, 1.0f };
GLfloat LightPosition2[] = { 0.0f, 10.0f, 5.0f, 1.0f };
GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };   // Storage For Three Types Of Fog
GLfloat fogColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };      // Fog Color
static int frame_no = 0;


bool	active = TRUE;				// Window Active Flag Set To TRUE By Default

float	slowdown = 2.0f;				// Slow Down Particles
float	xspeed;						// Base X Speed (To Allow Keyboard Direction Of Tail)
float	yspeed;						// Base Y Speed (To Allow Keyboard Direction Of Tail)
float	zoom = -40.0f;				

GLuint	loop;						
GLuint	col;						
GLuint	delay;						

GLdouble camx = 0;
GLdouble camy = 0;
GLdouble camz = 0;

GLdouble positionx = camx;
GLdouble positiony = camy;
GLdouble positionz = camz;

GLdouble centerx = camx;
GLdouble centery = camy;
GLdouble centerz = camz-1;

float angleXZ = -90;
float angleYZ = 90;

float move;
int x;




void init()
{
	
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//wlaczanie perspektywy
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);//wlaczanie tekstur
	glClearColor(0.3, 0.5, 1.0, 0.0);	//czyszczenie buforu na kolor
	glAlphaFunc(GL_GREATER, 0.0f);
	glEnable(GL_ALPHA_TEST);//wlaczanie kanalu alpha
	glEnable(GL_DEPTH_TEST);//wlczanie glebi
	tex[0] = SOIL_load_OGL_texture("trawa.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_INVERT_Y |
		SOIL_FLAG_COMPRESS_TO_DXT);
	tex[1] = SOIL_load_OGL_texture("pine.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_COMPRESS_TO_DXT);
	tex[2] = SOIL_load_OGL_texture("shell.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_COMPRESS_TO_DXT);
	tex[3] = SOIL_load_OGL_texture("shell1.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_COMPRESS_TO_DXT);
	tex[4] = SOIL_load_OGL_texture("shell2.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_COMPRESS_TO_DXT);
	tex[5] = SOIL_load_OGL_texture("body.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_COMPRESS_TO_DXT);
	tex[6] = SOIL_load_OGL_texture("body1.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_COMPRESS_TO_DXT);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, LightPosition2);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHTING);
	glDepthMask(GL_TRUE);
	
	
}

void Poly(float x1, float y1, float z1, float x2, float y2, float z2,
	float x3, float y3, float z3, float x4, float y4, float z4, int texture, float przes)
	//funkcja rysuje prostokat (poly) xyz wypelniony tekstura
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(1, przes + 0); glVertex3f(x1, y1, z1);
	glTexCoord2f(1, przes + 1); glVertex3f(x2, y2, z2);
	glTexCoord2f(0, przes + 0); glVertex3f(x3, y3, z3);
	glTexCoord2f(0, przes + 1); glVertex3f(x4, y4, z4);
	glEnd();
}

void Trawa(float x, float y, float z, int typ)
{
	Poly(x + 3, y, z, x + 3, y, z + 3, x, y, z, x, y, z + 3, tex[0], 0);
}

void Drzewo(float X, float Z, int r)
{
	
	Poly(X, 4 + 2 * r, 3 + Z, X, -3, 3 + Z,
		X, 4 + 2 * r, -3 + Z, X, -3, -3 + Z, tex[1], 0);
	Poly(3 + X, 4 + 2 * r, Z, 3 + X, -3, Z,
		-3 + X, 4 + 2 * r, Z, -3 + X, -3, Z, tex[1], 0);
}



void displayObjects()
{
	//TRAWA
	for (int i=0; i<30; i++)
	for (int j=0; j<30; j++) Trawa(((float)j*3)-40,-2.3f,((float)i*3)-40,0);

	//DRZEWA
	for (int i = 0; i < 20; i++)
		Drzewo(-37 + 3 * i, -20, i % 2);
	for (int i = 0; i<20; i++)
		Drzewo(-25, -38 + 3 * i, i % 2);
	for (int i = 0; i<20; i++)
		Drzewo(15, -38 + 3 * i, i % 2);
	Drzewo(10, -5, 2);
	Drzewo(-10, -5, 1);
	//glEnable(GL_COLOR_MATERIAL);

	//Slimak
	GLfloat torus_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat torus_diffuse2[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat sphere_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat sphere_diffuse1[] = { 0.7, 0.7, 0.7, 1.0 }; 
	GLfloat sphere_diffuse2[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat sphere_diffuse3[] = { 0.7, 0.7, 0.7, 1.0 }; 
	GLfloat sphere_diffuse4[] = { 0.7, 0.7, 0.7, 1.0 };

    //torus duzy
	glPushMatrix();
	glTranslatef(move+0.2, -1, -7);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex[2]);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, torus_diffuse);
	glutSolidTorus(0.275f, 0.85f, 20.0f, 20.0f);
	glPopMatrix();

	//torus maly
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex[3]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, torus_diffuse2);
	glutSolidTorus(0.175, 0.475, 20, 20);
	glScalef(0.5, 0.5, 0.5);
	glPopMatrix();

    //sfera w torusach
	glPushMatrix();
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, tex[4]);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_diffuse);
	glScalef(0.4, 0.4, 0.4);
	glutSolidSphere(1.0, 50, 50);
	glPopMatrix();

	//sfera aka elipsa glowa
	glPushMatrix();
	glRotatef(-30.0, 0, 0, 1);
	glTranslatef(-0.8, -1, 0);
	glScalef(0.9, 0.3, 0.3);
	glBindTexture(GL_TEXTURE_2D, tex[6]);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_diffuse1);
	glutSolidSphere(1.0, 50, 50);
    glPopMatrix();

	//sfera aka elipsa ogonek
	glPushMatrix();
	glRotatef(15.0, 0, 0, 1);
	glTranslatef(0.6, -0.9, 0);
	glScalef(0.5, 0.2, 0.3);
	glBindTexture(GL_TEXTURE_2D, tex[6]);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_diffuse2);
	glutSolidSphere(1.0, 60, 60);
	glPopMatrix();

	//oko
	glPushMatrix();
	glRotatef(-60.0, 0, 0, 1);
	glTranslatef(-1.1, -1.5, -0.1);
	glScalef(0.3, 0.05, 0.05);
	glBindTexture(GL_TEXTURE_2D, tex[5]);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_diffuse3);
	glutSolidSphere(1.0, 60, 60);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-60.0, 0, 0, 1);
	glTranslatef(-1.1, -1.5, 0.1);
	glScalef(0.3, 0.05, 0.05);
	glBindTexture(GL_TEXTURE_2D, tex[5]);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_diffuse4);
	glutSolidSphere(1.0, 60, 60);
	glPopMatrix();
	glPopMatrix();
	

}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	gluLookAt(positionx, positiony, positionz, positionx + centerx, positiony + centery, positionz + centerz, 0, 1, 0);
	displayObjects();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
	
}

void reshape(GLsizei w, GLsizei h)
{
	if (h > 0 && w > 0) {
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, (GLfloat)w / (GLfloat)h, 1, 100.0f);

		glMatrixMode(GL_MODELVIEW);
		
	}
}

void SpecialKeys(int key, int x, int y)
{
	float step = 0.3;
	switch (key)
	{
	case GLUT_KEY_F7:
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glFogi(GL_FOG_MODE, fogMode[2]);        // Fog Mode
		glFogfv(GL_FOG_COLOR, fogColor);            // Set Fog Color
		glFogf(GL_FOG_DENSITY, 0.05f);           
		glHint(GL_FOG_HINT, GL_NICEST);   
		glFogf(GL_FOG_START, -10.0f);
		glFogf(GL_FOG_END, 15.0f);
		glEnable(GL_FOG);
		break;
	case GLUT_KEY_F8:
		glClearColor(0.3, 0.5, 1.0, 0.0);
		glDisable(GL_FOG);
		break;
		// kursor w lewo
	case GLUT_KEY_LEFT:
		camz -= step*cos(angleXZ * M_PI / 180);
		camx += step*sin(angleXZ * M_PI / 180);
		break;

		// kursor w gore
	case GLUT_KEY_UP:
		camx += step*cos(angleXZ * M_PI / 180);
		camy += step*cos(angleYZ * M_PI / 180);
		camz += step*sin(angleXZ * M_PI / 180);
		break;

		// kursor w prawo
	case GLUT_KEY_RIGHT:
		camz += step*cos(angleXZ * M_PI / 180);
		camx -= step*sin(angleXZ * M_PI / 180);
		break;

		// kursor w dol
	case GLUT_KEY_DOWN:
		camx -= step*cos(angleXZ * M_PI / 180);
		camy -= step*cos(angleYZ * M_PI / 180);
		camz -= step*sin(angleXZ * M_PI / 180);
		break;
	}

	camx = camx < -20 ? -20 : camx;
	camy = camy < -20 ? -20 : camy;
	camz = camz < -20 ? -20 : camz;
	camx = camx > 20 ? 20 : camx;
	camy = camy > 20 ? 20 : camy;
	camz = camz > 20 ? 20 : camz;
	positionx = camx;
	positiony = camy;
	positionz = camz;

	// odrysowanie okna
	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void Keyboard(unsigned char key, int x, int y)
{
	float step = 0.02;
	switch (key)
	{
	case 'a':
		centerz -= step*cos(angleXZ * M_PI / 180);
		centerx += step*sin(angleXZ * M_PI / 180);
		angleXZ -= 1;
		break;
	case 'd':
		centerz += step*cos(angleXZ * M_PI / 180);
		centerx -= step*sin(angleXZ * M_PI / 180);
		angleXZ += 1;
		break;
	case '1':
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glClearColor(0.3, 0.5, 1.0, 0.0);
		break;
	case '2':
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glClearColor(0.2, 0.1, 0.8, 0.0);
    break;

	case 'q':
		move = move <= -7 ? -7 : move - 0.1;
		break;

	case 'w':
		move = move >= 7 ? 7 : move +0.1;
		break;
	}	// odrysowanie okna
	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	FreeConsole();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Slimak");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(Keyboard);
	glutIdleFunc(display);

	init();

	glutMainLoop();

	return 0;
}
