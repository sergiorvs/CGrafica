//Compilar: g++ practica5.cpp -o p5 -lglut -lGLU -lGL -std=c++11 -lfreeimage
//Ejecutar: ./p5


#define GLUT_DISABLE_ATEXIT_HACK	
//#include <windows.h>

#include "terreno.h"
//#include "TextureManager.cpp"

using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27
#define W_KEY 119


void init_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//texturas
GLint terrain;

//Terreno
Terreno fontvieille;

//Luz
GLfloat Light0Pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat direction[] = {0.0, -1.0, 0.0};



//variables para el gizmo
float delta_x = 0.0; 
float delta_y = 0.0;
float mouse_x, mouse_y;
float var_x = -2450.0;
float var_z = -4800.0;
float var_y = -500.0;
float step = 0; //0.0 Posicion inicial. 1.0 Traslacion. 2.0 Primera Rotacion(en y). 3.0 Segunda Rotacion (en x) 4.0 Ultima Rotacion (en z)


///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		var_z += 50;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		var_z -= 50;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		var_x += 50;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		var_x -= 50;
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
	glutCreateWindow("P5: Terreno");


	initGL();
	terrain = TextureManager::Inst()->LoadTexture("fontvieille.tga", GL_RGB, GL_RGB);
	init_scene();
	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);
	
	
	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);

	glutKeyboardFunc(&window_key);
	glutSpecialFunc(&callback_special);
	glutIdleFunc(&window_idle);

	fontvieille.load("fontvieille.txt");

	glutMainLoop();

	return 1;
}



GLvoid initGL()
{

	glEnable(GL_DEPTH_TEST);


	glClearColor(RED, GREEN, BLUE, ALPHA);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_TEXTURE);
	glShadeModel(GL_SMOOTH); // modelo de shading try GL_FLAT
	glEnable(GL_CULL_FACE); //no trata las caras escondidas
	glEnable(GL_DEPTH_TEST); // Activa el Z-Buffer
	glDepthFunc(GL_LEQUAL); //Modo de funcionamiento del Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Activa la corrección de perspectiva

	//materiales
	GLfloat roofAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat mat_defused[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat mat_shininess[] = {500.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, roofAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_defused);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);


	GLfloat Light0Amb[4] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat Light0Dif[4] = {0.7f, 0.6f, 0.5f, 1.0f};
	GLfloat Light0Spec[4]= {1.0f, 0.9f, 0.8f, 1.0f};



	// los parámetros de colores de la luz 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light0Spec);

	// la posición de la luz 0

	//Activación de la luz
	glEnable(GL_LIGHTING);
	//Activación de la luz 0
	glEnable(GL_LIGHT0);
}


void Gizmo3D(){


	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(3000.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 3000.0f, 0.0f);
	glEnd();


	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 3000.0f);
	glEnd();


	return;

}


GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();




	gluPerspective(45.0f, 1.0f, 0.01f, 100000.0f);


	

	glTranslatef(var_x, var_y, var_z);
	glRotatef(delta_x, 0.0, 1.0, 0.0);
	glRotatef(delta_y, 1.0, 0.0, 0.0);


	glLightfv(GL_LIGHT0, GL_POSITION, Light0Pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	glPushMatrix();
	glTranslatef(Light0Pos[0],Light0Pos[1],Light0Pos[2]);
	glutSolidSphere(30,100,100);
	glPopMatrix();

	Gizmo3D();


	fontvieille.display();

	

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
	case W_KEY:
		var_y -= 25;
		break;
	case 115:
		var_y += 25;
		break;
	case 105:		//i hacia arriba
		Light0Pos[1] += 10;
		break;
	case 108:		//derecha
		Light0Pos[0] += 10;
		break;
	case 107:
		Light0Pos[1] -= 10;
		break;
	case 106:
		Light0Pos[0] -=10;
		break;
	case 112:
		Light0Pos[2] +=10;
		break;
	case 241:
		Light0Pos[2] -=10;
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





