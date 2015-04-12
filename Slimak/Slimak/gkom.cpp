#include <windows.h>
#include <GL/gl.h>
#include "glut.h"
#include "SOIL.h"
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>

GLdouble eyex = 0;
GLdouble eyey = 0;
GLdouble eyez = 0;

GLdouble dx = 0;
GLdouble dy = 0;
GLdouble dz = 0;

GLdouble positionx = eyex;
GLdouble positiony = eyey;
GLdouble positionz = eyez;

// wspó³rzêdne punktu w którego kierunku jest zwrócony obserwator,

GLdouble centerx = eyex;
GLdouble centery = eyey;
GLdouble centerz = eyez - 1;

int oldX = 0;
int oldY = 0;

int oldTime = 0;

float angleXZ = -90;
float angleYZ = 90;

float l = 0;

float sensitivity = -0.5f;
GLuint skybox[6];

GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//GLfloat light_position[] = { 0.0, 10.0, 10.0, 1.0 };
GLfloat lm_ambient[] = { l, l, l, 1 };

void init()
{



	//glLightfv( GL_LIGHT0, GL_POSITION, light_position );
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);

	//cianiowanie plaskie
	//glShadeModel( GL_FLAT );

	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);
	glDisable(GL_LIGHT5);
	glDisable(GL_LIGHT6);
	glDisable(GL_LIGHT7);

	// wlaczenie obskugi wlasciwoœci materialow
	glEnable(GL_COLOR_MATERIAL);

	// wlasciwoœci materialu okreslone przez kolor wierzcholkow
	glColorMaterial(GL_FRONT, GL_AMBIENT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// normalizacja wektorow zeby np swiatlo sie nie psulo przy skalowaniu
	glEnable(GL_NORMALIZE);

	glCullFace(GL_FRONT);

	//cieniowanie interpolowane
	glShadeModel(GL_SMOOTH);

	skybox[0] = SOIL_load_OGL_texture("img/right.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_INVERT_Y |
		SOIL_FLAG_COMPRESS_TO_DXT);

	skybox[1] = SOIL_load_OGL_texture("img/left.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_INVERT_Y |
		SOIL_FLAG_COMPRESS_TO_DXT);

	skybox[2] = SOIL_load_OGL_texture("img/top.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_INVERT_Y |
		SOIL_FLAG_COMPRESS_TO_DXT);

	skybox[3] = SOIL_load_OGL_texture("img/bottom.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_INVERT_Y |
		SOIL_FLAG_COMPRESS_TO_DXT);

	skybox[4] = SOIL_load_OGL_texture("img/back.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_INVERT_Y |
		SOIL_FLAG_COMPRESS_TO_DXT);

	skybox[5] = SOIL_load_OGL_texture("img/front.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS |
		SOIL_FLAG_INVERT_Y |
		SOIL_FLAG_COMPRESS_TO_DXT);

	for (int i = 0; i < 6; i++)
	{
		if (0 == skybox[i])
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
		}
	}

}

void dragonfly(int frame_no)
{
	// wazka

	//glowa
	glRotatef(-90, 0, 0, 1);
	glPushMatrix();

	glColor3f(0.02, 0.25, 0.42);
	glTranslatef(0, 2.25, 0);
	glPushMatrix();
	glTranslatef(0.1, -0.1, 0);
	glRotatef(90, 0, 0, 1);
	glScalef(0.6, 1, 0.6);
	glScalef(0.3, 0.3, 0.3);
	glutSolidSphere(1, 24, 24);
	glPopMatrix();

	glColor3f(0.74, 0.22, 0.19);
	glPushMatrix();
	glTranslatef(0, 0, -0.15);
	glRotatef(15, 0, 1, 0);
	glRotatef(80, 0, 0, 1);
	glScalef(0.6, 1, 0.6);
	glScalef(0.3, 0.3, 0.3);
	glutSolidSphere(1, 24, 24);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 0.15);
	glRotatef(-15, 0, 1, 0);
	glRotatef(80, 0, 0, 1);
	glScalef(0.6, 1, 0.6);
	glScalef(0.3, 0.3, 0.3);
	glutSolidSphere(1, 24, 24);
	glPopMatrix();
	glPopMatrix();

	// korpus
	glColor3f(0.06, 0.52, 0.9);
	glPushMatrix();
	glScalef(0.2, 0.7, 0.2);
	glTranslatef(0, 1.85, 0);
	lathe(body, -1.06, 1.06, 20, 36, 0.5);
	glPopMatrix();

	//skrzydla
	glColor4f(0, 0, 0, 0.5);
	glPushMatrix();
	glTranslatef(-0.36, 1.5, 0.17);
	glRotatef(-10, 1, 0, 0);
	glScalef(1, 0.15, 1);
	glRotatef(frame_no % 40 - 20, 0, 1, 0);
	glTranslatef(0, 0, 2);
	circle(2, 24);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.36, 1.2, 0.17);
	glRotatef(10, 1, 0, 0);
	glScalef(1, 0.15, 1);
	glRotatef(frame_no % 40 - 20, 0, 1, 0);
	glTranslatef(0, 0, 2);
	circle(2, 24);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.36, 1.5, -0.17);
	glRotatef(10, 1, 0, 0);
	glScalef(1, 0.15, 1);
	glRotatef(-(frame_no % 40 - 20), 0, 1, 0);
	glTranslatef(0, 0, -2);
	circle(2, 24);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.36, 1.2, -0.17);
	glRotatef(-10, 1, 0, 0);
	glScalef(1, 0.15, 1);
	glRotatef(-(frame_no % 40 - 20), 0, 1, 0);
	glTranslatef(0, 0, -2);
	circle(2, 24);
	glPopMatrix();
	// ogon
	glPushMatrix();
	glColor3f(0.02, 0.25, 0.42);
	glPushMatrix();
	glRotatef(-5, 0, 0, 1);
	glScalef(0.1, 0.4, 0.1);
	glScalef(1, 0.6, 1);
	glTranslatef(-0.45, 0.12, 0);
	lathe(tail, -0.4, 2.5, 50, 36, 0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-6, 0, 0, 1);
	glScalef(0.1, 0.4, 0.1);
	glScalef(1, 0.8, 1);
	glTranslatef(-0.4, -2.2, 0);
	lathe(tail, -0.4, 2.1, 50, 36, 0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-7, 0, 0, 1);
	glScalef(0.1, 0.4, 0.1);
	glTranslatef(-0.28, -4.04, 0);
	lathe(tail, -0.4, 2.1, 50, 36, 0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-9, 0, 0, 1);
	glScalef(0.1, 0.4, 0.1);
	glScalef(1, 1.2, 1);
	glTranslatef(0.32, -5.75, 0);
	lathe(tail, -0.4, 2.1, 50, 36, 0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-10, 0, 0, 1);
	glScalef(0.1, 0.4, 0.1);
	glScalef(1, 1.3, 1);
	glTranslatef(0.8, -7.7, 0);
	lathe(tail, -0.4772, 2, 50, 36, 0);
	glPopMatrix();
	glPopMatrix();
}

void display()

{
	GLfloat light_position[] = { 10, 8, 8, 1 };

	static int frame_no = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (frame_no<360) frame_no++; else frame_no = 0;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 4.0, 8.0, 0.0, 2.5, 0.0, 0.0, 1.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	displayObjects(frame_no);


	glFlush();

	glutSwapBuffers();

}



void reshape(GLsizei w, GLsizei h)

{

	if (h > 0 && w > 0) {

		glViewport(0, 0, w, h);

		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();


		gluPerspective(50.0f, ((float)w) / (float)h, 1.0, 100.0);


		glMatrixMode(GL_MODELVIEW);

	}




}

nt main(int argc, char** argv)

{
	/*LoadFile("girl.obj", &s_ModelVerticesCount, &s_ModelIndicesCount, &s_ModelVertices, &s_ModelNormals, &s_ModelIndices);
	LoadFile("viking.obj", &s_ModelVerticesCount2, &s_ModelIndicesCount2, &s_ModelVertices2, &s_ModelNormals2, &s_ModelIndices2);*/
	InitializeDishData();

	cout << "Initializing GLUT..." << endl;
	glutInit(&argc, argv);

	cout << "Creating window with context..." << endl;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);



	glutInitWindowPosition(0, 0);

	glutInitWindowSize(800, 600);



	glutCreateWindow("GKOM lab. 3: OpenGL Vertex Arrays");


	cout << "Registering handlers...." << endl;

	glutDisplayFunc(display);

	glutReshapeFunc(reshape);

	glutIdleFunc(display);


	cout << "Initializing rendering pipeline..." << endl;
	init();


	cout << "Running..." << endl;
	glutMainLoop();
	cout << "Cleaning up..." << endl;

	DeinitializeDishData();

	delete s_ModelVertices;
	delete s_ModelIndices;
	delete s_ModelNormals;


	delete s_ModelVertices2;
	delete s_ModelIndices2;
	delete s_ModelNormals2;

	cout << "Done:) CU!" << endl;



	return 0;

}


