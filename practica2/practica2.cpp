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

void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//function called on each frame
GLvoid window_idle();

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

	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}

void displayGizmo()
{
	glBegin(GL_LINES);
	glColor3d(255,0,0);
	glVertex2d(0, 0);
	glVertex2d(100, 0);
	glColor3d(0, 255, 0);
	glVertex2d(0, 0);
	glVertex2d(0, 100);
	glColor3d(0, 0, 255);
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
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

    displayGizmo();

	/*dibujar aqui*/
	//EJERCICIO DEL 1 AL 3

   /* glColor3d(0,0,255);

    //glTranslatef()
    glPushMatrix();
    glTranslatef(eje, 0.0f, 0.0f);

    if(eje > 8) flag = true;
    if(eje < -8) flag = false;
    if(flag == false){
        eje += 10 * dt;
    }else{
        eje -= 10 * dt;
    }

    glPushMatrix();



	glRotatef(alpha, 0.0f, 0.0f, 1.0f);
	glTranslatef(8.0f, 0.0f, 0.0f);


	glColor3d(0, 255, 0);
	glutSolidTeapot(3);
	//traslaciones para el toro

    //glTranslatef(8*cos(alpha)-3*sin(alpha)+8, 3*cos(alpha)+8*sin(alpha)+3*flag, 0);
    glRotatef(alpha*3, 0, 1, 0);
    glTranslated(10,0,0);
    //glTranslatef(8*cos(alpha)-3*sin(alpha), 3*cos(alpha)+8*sin(alpha), 0);

	glColor3d(189, 236, 182);
	glutSolidTorus(0.5, 1, 50, 50);

	//fin toro
	glPopMatrix();

    glColor3d(0,255,255);
	glutSolidSphere(1, 10000, 10);
	glRotated(alpha, 1, 0, 0);
	glTranslated(0,-8,0);
	glColor3d(255,0,0);
	glutSolidCube(3);

	glPopMatrix();


	alpha += 30 * dt; //da la vuelta completa en 1 minuto
*/

	//EJERCICIO 4

    //sol
	glPushMatrix();
	glRotatef(alpha,1,1,1);
    glColor3d(255,255,0);
	glutSolidSphere(4,8,8);
    glPopMatrix();


    //tierra
 	glPushMatrix();
 	//para girar la tierra sobre si misma
 	glPushMatrix();
 	glTranslated(0,0,0);
 	glRotatef(3*alpha,1,1,1);
 	glPopMatrix();

 	glRotatef(alpha, 0.0f, 0.0f, 1.0f);
    glTranslated(10,0,0);
	glColor3d(0,0,255);
	glutSolidSphere(2,8,8);

	//luna
	//girar luna sobre si misma
    glPushMatrix();
 	glTranslated(0,0,0);
 	glRotatef(1.5*alpha,1,1,1);
 	glPopMatrix();

    glRotatef(2*alpha, 0.0f, 0.0f, 1.0f);
	glTranslatef(10, 0, 0);
    glColor3d(130, 130, 130);
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



	alpha += 30 * dt; //da la vuelta completa en 1 minuto



	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

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