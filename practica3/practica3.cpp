#define GLUT_DISABLE_ATEXIT_HACK
//#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <GL/glut.h>
using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27

int time_2=0;
int timebase=0;

float alpha = 0;

float eje = -8;
bool flag = 0;


double camPosX = 0;
double camPosY = 20;
double camPosZ = 40;

double centX = 0;
double centY = 0;
double centZ = 0;


double angRotX = 0;
double angRotY = 0;

double temDTX = 0;
double temDTY = 0;

double changeSpeed = 1;
double speedAngle = 1;

float x=10, y=0, z=0;

void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//function called on each frame
GLvoid window_idle();

///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
	    camPosZ = camPosZ - changeSpeed;
		centZ = centZ - changeSpeed;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;

	case GLUT_KEY_DOWN:
	    camPosZ = camPosZ + changeSpeed;
	    centZ = centZ + changeSpeed;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;

	case GLUT_KEY_LEFT:
	    camPosX = camPosX - changeSpeed;
	    centX = centX - changeSpeed;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;

	case GLUT_KEY_RIGHT:
	    camPosX = camPosX + changeSpeed;
	    centX = centX + changeSpeed;
		glutPostRedisplay();			// et on demande le réaffichage.
		break;
	}

}



///////////////////////////////////////////////////////////////////////////////
//(2)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
	    temDTX = sin(x);
        temDTY = cos(y);
        cout<< x << "; "<< y <<endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
//(3)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{
    angRotX = x - temDTX;
    angRotY = y - temDTY;
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 2 : Transformaciones");


	initGL();
	init_scene();

	glutDisplayFunc(&window_display);

	glutReshapeFunc(&window_reshape);

	glutKeyboardFunc(&window_key);

	glutSpecialFunc(&callback_special); //teclas especiales
	glutMouseFunc(&callback_mouse); //clicks
	glutMotionFunc(&callback_motion);  //movimiento del mouse

	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}




void displayGizmo()
{
	glBegin(GL_LINES);
	glColor3d(255,0,0);
	glVertex3d(0, 0, 0);
	glVertex3d(100, 0, 0);
	glColor3d(0, 255, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 100, 0);
	glColor3d(0, 0, 255);
	glVertex3d(0, 0,0);
    glVertex3d(0, 0, 100);
	glEnd();
}


GLvoid initGL()
{
	GLfloat position[] = { 0.0f, 5.0f, 10.0f, 0.0 };

	//enable light : try without it
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	//light 0 "on": try without it
	glEnable(GL_LIGHT0);

	//shading model : try GL_FLAT
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	//enable material : try without it
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(RED, GREEN, BLUE, ALPHA);
}

GLvoid window_display()
{
    time_2 = glutGet(GLUT_ELAPSED_TIME); // recupera el tiempo ,que paso desde el incio de programa
    float dt = float(time_2 -timebase)/1000.0;// delta time
    timebase = time_2;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);
	gluPerspective(45,1,0.1,100);
	//glTranslated(1, 1, -30);      //traslacion en z EJERCICIO 2
    gluLookAt(camPosX,camPosY,camPosZ,centX,centY,centZ,0,1,0);


    //EJERCICIO 3, CALCULO DE X, Y DE LA TIERRA
    //cout<<"x = " << x << " ;y = " << y <<" alpha = "<<(alpha)<<endl;
    x = 10*sin((alpha)*M_PI/180);
    y = 10*cos((alpha)*M_PI/180);

    gluLookAt(0, 0, 20, x, y, 0, 0, 1, 0);

////////////////////////////////
    /*glRotated(angRotY,0.0f,0.0f,1.0f);
    glRotated(angRotX,0.0f,1.0f,0.0f);*/
    displayGizmo();
//////////////////////////////


//Ejercicio 1
/*
    glColor3d(0,0,255);

    //glTranslatef()
    glPushMatrix();

	//glRotatef(alpha, 0.0f, 0.0f, 1.0f);
	glTranslatef(8*cos(alpha), 8*sin(alpha), 0.0f);         //EJERCICIO 1 ROTACION SIN QUE GIRE LA TETERA
    //glTranslated(8,0,0);

	glColor3f(0, 1, 0);
	glutSolidTeapot(3);

	glPopMatrix();



	alpha += 6 * dt; //da la vuelta completa en 1 minuto

*/


    //sol
	glPushMatrix();
	glRotatef(alpha,1,1,1);
    glColor3f(1,1,0);
	glutSolidSphere(4,8,8);
    glPopMatrix();



    //tierra
 	glPushMatrix();

 	glRotatef(alpha, 0.0f, 0.0f, 1.0f);
    glTranslated(10,0,0);
	glColor3f(0,0,1);
	glutSolidSphere(2,8,8);



	//luna
	//girar luna sobre si misma
    glPushMatrix();
 	glTranslated(0,0,0);
 	glRotatef(1.5*alpha,1,1,1);
 	glPopMatrix();

    glRotatef(2*alpha, 0.0f, 0.0f, 1.0f);
	glTranslatef(4, 0, 0);
    glColor3f(1, 1, 1);
    glutSolidSphere(1, 8, 8);
	glPopMatrix();

	//marte
	glPushMatrix();
	glPushMatrix();
	glTranslated(0,0,0);
 	glRotatef(alpha,1,1,1);
	glPopMatrix();

	glRotatef(alpha,0,0,1);
	glTranslatef(18,0,0);
	glColor3f(0.8,0.3,0);
	glutSolidSphere(2.5, 8, 8);
    glPopMatrix();



	alpha += 60 * dt; //da la vuelta completa en 1 minuto


	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);
	gluPerspective(45, 4.0, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);
}



void init_scene()
{

}

GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key) {
	case ECHAP:
		exit(1);
		break;

	default:
		printf("La touche %d non active.\n", key);
		break;
	}
}


//function called on each frame
GLvoid window_idle()
{


	glutPostRedisplay();
}