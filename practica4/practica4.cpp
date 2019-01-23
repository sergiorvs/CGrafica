#define GLUT_DISABLE_ATEXIT_HACK	
//#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include <string>
#include <cstring>

#include "TextureManager.cpp"

using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27

struct Point{
	GLfloat x,y,z;

	Point(GLfloat _x = 0, GLfloat _y = 0, GLfloat _z = 0){
		x = _x; y = _y; z = _z;
	}

	GLvoid printPoints(){
		cout<<x<<", "<<y<<", "<<z<<endl;
	}

	Point operator -(Point b){
		Point result;
		result.x = x - b.x;
		result.y = y - b.y;
		result.z = z - b.z;
		return result;
	}

	Point operator *(Point v){
		Point result;
		result.x = y*v.z - z*v.y;
		result.y = -1*(x*v.z - z*v.x);
		result.z = x*v.y - y*v.x;
		return result;
	}

	Point operator /(GLfloat k){
		Point result;
		result.x = x/k;
		result.y = y/k;
		result.z = z/k;
		return result;
	}

	GLfloat magnitude(){
		return sqrt(x*x + y*y + z*z);
	}
};

Point updateNormal(Point a, Point b, Point c){
	Point ab = b - a;
	//cout<<"AB = ";ab.printPoints();
	Point ac = c - a; 
	//cout<<"AC = ";ac.printPoints();
	Point normal = ab * ac;
	//cout<<"ab*ac=";normal.printPoints();
	normal = normal/normal.magnitude();

	return normal;
}

void init_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//texturas
GLint texture;
GLint grass;
GLint roof;
GLint trunk;
GLint leaf;

//Objetos
GLUquadricObj *sphere = NULL;
GLUquadricObj *cylinder = NULL;


//variables para el gizmo
float delta_x = 0.0; 
float delta_y = 0.0;
float mouse_x, mouse_y;
float var_x = 0.0;
float var_z = -30.0;
float step = 0; //0.0 Posicion inicial. 1.0 Traslacion. 2.0 Primera Rotacion(en y). 3.0 Segunda Rotacion (en x) 4.0 Ultima Rotacion (en z)

//normales
Point nGround(0,1,0);
Point nBackWall(0,0,-1);
Point nFrontWall(0,0,1);
Point nRightWall(1,0,0);
Point nLeftWall(-1,0,0);
Point nRightRoof(1,0,0);
Point nLeftRoof(-1,0,0);
Point nFrontRoof = updateNormal(Point(0,5,4), Point(7,5,4), Point(0,7,2));
Point nBackRoof = updateNormal(Point(0,5,0), Point(7,5,0), Point(0,7,2));

///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		var_z += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		var_z -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		var_x += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		var_x -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_PAGE_UP:
		step++;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		step--;
		glutPostRedisplay();
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
		mouse_x = x;
		mouse_y = y;
	}
}

///////////////////////////////////////////////////////////////////////////////
//(3)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{
	
	delta_x += x - mouse_x;
	delta_y += y - mouse_y;
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}


//function called on each frame
GLvoid window_idle();



int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(800, 800);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("P4: Iluminacion y Texturas");


	initGL();
	init_scene();

	texture = TextureManager::Inst()->LoadTexture("brick2.jpeg", GL_RGB, GL_RGB);
	grass = TextureManager::Inst()->LoadTexture("grass2.png", GL_RGB, GL_RGB);
	roof = TextureManager::Inst()->LoadTexture("roofs.jpg", GL_RGB, GL_RGB);
	trunk = TextureManager::Inst()->LoadTexture("tronco.jpg", GL_RGB, GL_RGB);
	leaf = TextureManager::Inst()->LoadTexture("leafs.jpeg", GL_RGB, GL_RGBA);
	
	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);
	
	
	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);

	glutKeyboardFunc(&window_key);
	//glutKeyboardUpFunc(&window_key_up); //key release events
	glutSpecialFunc(&callback_special);
	//glutSpecialUpFunc(&callback_special_up); //key release events
	
	
	//function called on each frame
	glutIdleFunc(&window_idle);


	glutMainLoop();

	return 1;
}



GLvoid initGL()
{

	glEnable(GL_DEPTH_TEST);


	glClearColor(RED, GREEN, BLUE, ALPHA);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_TEXTURE);
	//glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH); // modelo de shading try GL_FLAT
	glEnable(GL_CULL_FACE); //no trata las caras escondidas
	glEnable(GL_DEPTH_TEST); // Activa el Z-Buffer
	glDepthFunc(GL_LEQUAL); //Modo de funcionamiento del Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Activa la corrección de perspectiva

	//materiales
	GLfloat roofAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat mat_defused[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat mat_shininess[] = {100.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, roofAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_defused);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);


    //colores: luz roja
	GLfloat Light0Amb[4] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat Light0Dif[4] = {0.7f, 0.6f, 0.5f, 1.0f};
	GLfloat Light0Spec[4]= {1.0f, 0.9f, 0.8f, 1.0f};

	// posiciones: luz puntual
	GLfloat Light0Pos[4] = {4.0f, 30.0f, -5.0f, 1.0f};

	GLfloat direction[] = {0.0, -1.0, 0.0};

	// los parámetros de colores de la luz 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light0Spec);

	// la posición de la luz 0
	glLightfv(GL_LIGHT0, GL_POSITION, Light0Pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);

	// Activación de la luz
	glEnable(GL_LIGHTING);
	//Activación de la luz 0
	glEnable(GL_LIGHT0);
}


void Gizmo3D(){


	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(3.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	glEnd();


	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 3.0f);
	glEnd();


	return;

}


GLvoid displayGround(){
	//glColor3f(0.027, 0.62, 0.25);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, grass);
	glNormal3f(nGround.x, nGround.y, nGround.z);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(40.0f, 0.0f, 40.0f);
	glTexCoord2f(1.0, 0.0f);
	glVertex3f(40.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0f, 0.0f, 40.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

GLvoid BackWall(){
	//glColor3f(0.46, 0, 0.051);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glNormal3f(nBackWall.x, nBackWall.y, nBackWall.z);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0f);
	glVertex3f(7.0f, 5.0f, 0.0f);
	glTexCoord2f(1.0, 0.0f);
	glVertex3f(7.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

GLvoid FrontWall(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glColor3f(0.46, 0, 0.051);
	glNormal3f(nFrontWall.x, nFrontWall.y, nFrontWall.z);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 0.0f, 4.0f);
	glTexCoord2f(1.0, 0.0f);
	glVertex3f(7.0f, 0.0f, 4.0f);
	glTexCoord2f(1.0, 1.0f);
	glVertex3f(7.0f, 5.0f, 4.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0f, 5.0f, 4.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

GLvoid LeftWall(){
	//glColor3f(0.46, 0, 0.051);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);	
	glNormal3f(nLeftWall.x, nLeftWall.y, nLeftWall.z);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 0.0f, 4.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0f, 5.0f, 4.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

GLvoid RightWall(){
	//glColor3f(0.46, 0, 0.051);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glNormal3f(nRightWall.x, nRightWall.y, nRightWall.z);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(7.0f, 5.0f, 4.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(7.0f, 0.0f, 4.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(7.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(7.0f, 5.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

GLvoid LeftRoof(){
	//glColor3f(0.46, 0, 0.051);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glNormal3f(nLeftRoof.x, nLeftRoof.y, nLeftRoof.z);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(0.0f, 7.0f, 2.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(0.1, 1.0);
	glVertex3f(0.0f, 5.0f, 4.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

GLvoid RightRoof(){
	//glColor3f(0.46, 0, 0.051);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glNormal3f(nRightRoof.x, nRightRoof.y, nRightRoof.z);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(7.0f, 7.0f, 2.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(7.0f, 5.0f, 4.0f);
	glTexCoord2f(0.1, 1.0);
	glVertex3f(7.0f, 5.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

GLvoid BackRoof(){
	//glColor3f(0.51, 0.51, 0.51);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, roof);
	glNormal3f(nBackRoof.x, nBackRoof.y, nBackRoof.z);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(7.0f, 7.0f, 2.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(7.0f, 5.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 5.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0f, 7.0f, 2.0f);
	glEnd();
}

GLvoid FrontRoof(){
	//glColor3f(0.51, 0.51, 0.51);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, roof);
	glNormal3f(nFrontRoof.x, nFrontRoof.y, nFrontRoof.z);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 5.0f, 4.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(7.0f, 5.0f, 4.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(7.0f, 7.0f, 2.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0f, 7.0f, 2.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

GLvoid displayHouse(){
	BackWall();
	FrontWall();
	RightWall();
	LeftWall();
	RightRoof();
	LeftRoof();
	BackRoof();
	FrontRoof();
}

GLvoid displayTree(){
	glPushMatrix();
	glTranslatef(12, 0, 3);

	glRotatef(-90, 1, 0, 0);



	//glColor3f(0.50, 0.25, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, trunk);
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, TRUE);
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluCylinder(cylinder,1.5,1.0,3.0,30,30);
	gluDeleteQuadric(cylinder);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(12, 4.5, 3);
	//glColor3f(0.18, 0.34, 0.17);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, leaf);
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere,2.0,30,30);
	gluDeleteQuadric(sphere);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	gluPerspective(45.0f, 1.0f, 0.01f, 100.0f);
	glTranslatef(-5,-5,-10);

	glTranslatef(var_x, 0.0, var_z);
	glRotatef(delta_x, 0.0, 1.0, 0.0);
	glRotatef(delta_y, 1.0, 0.0, 0.0);

	//glClearColor(RED, GREEN, BLUE, ALPHA);

	//Gizmo3D();
	displayGround();
	displayHouse();
	displayTree();


	

	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

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





