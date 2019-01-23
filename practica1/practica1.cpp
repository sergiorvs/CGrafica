#define GLUT_DISABLE_ATEXIT_HACK
//#include <windows.h>
#include <math.h>
#include <GL/glut.h>

#define KEY_ESC 27

//dibuja un simple gizmo
void displayGizmo()
{
	glBegin(GL_LINES);
	glColor3d(255,0,0);
	glVertex2d(0, 0);
	glVertex2d(1, 0);
	glColor3d(0, 255, 0);
	glVertex2d(0, 0);
	glVertex2d(0, 1);
	glEnd();
}

//EJERCICIO 1
void drawSquare(GLint centerx, GLint centery, GLint sizeEdge) {
    glBegin(GL_LINE_LOOP);
    glColor3d(0,0,255);
    glVertex2f(centerx-sizeEdge/2, centery-sizeEdge/2);
    glVertex2f(centerx-sizeEdge/2, centery+sizeEdge/2);
    glVertex2f(centerx+sizeEdge/2, centery+sizeEdge/2);
    glVertex2f(centerx+sizeEdge/2, centery-sizeEdge/2);
    glEnd();
}

//EJERCICIO 2
void drawCircle(GLint centerx, GLint centery, GLint radio) {
    glBegin(GL_LINE_LOOP);
    glColor3d(0,100,128);
   // glVertex2f(centerx + radio, centery + radio);
    for(float i=0; i<2*M_PI; i+=0.1){
        glVertex2f(radio*cos(i)+centerx, radio*sin(i)+centery);
    }
    glEnd();
}

//EJERCICIO 3
void drawCircles(GLint centerx, GLint centery, GLint radio, GLint amount = 5) {
    for(int i=0; i<amount; i++){
        drawCircle(centerx, centery, radio);
        centerx = centerx+radio;
        radio = radio*2;

    }
}

//EJERCICIO 4
void drawLCircles(GLint centerx, GLint centery, GLint radio, GLint amount = 7, GLfloat reduction = 1.2) {
    for(int i=0; i<amount; i++){
        drawCircle(centerx, centery, radio);
        centerx = centerx+radio;
        radio = radio/reduction;
        centerx = centerx + radio;
    }
}

//EJERCICIO 5
void drawLACircles(GLint centerx, GLint centery, GLint radio, GLint amount = 7, GLfloat reduction = 1.2, GLfloat angle = M_PI/2) {
    for(int i=0; i<amount; i++){
        drawCircle(centerx, centery, radio);
        centerx = radio*cos(angle)+centerx;
        centery = radio*sin(angle)+centery;
        radio = radio/reduction;
        centerx = radio*cos(angle)+centerx;
        centery = radio*sin(angle)+centery;
    }
}

//
//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	glLoadIdentity();


	//dibuja el gizmo
	displayGizmo();
	drawSquare(0,0,10);
	drawCircle(0,0,20);
	drawCircles(-42,0,2);
	drawLCircles(-42,0,5);
	drawLACircles(0,0,5);

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-50.0f,  50.0f,-50.0f, 50.0f, -1.0f, 1.0f);
	// todas la informaciones previas se aplican al la matrice del ModelView
	glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(400, 400); //posicion de la ventana
	glutCreateWindow("TP1 OpenGL : hello_world_OpenGL"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);

	glutMainLoop(); //bucle de rendering

    return 0; }
