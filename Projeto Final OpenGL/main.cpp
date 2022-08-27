//----------------------------------------------------------------------------------------------------------------------------------
/*

PONTIFÍCIA UNIVERSIDADE CATÓLICA DO PARANÁ
ESCOLA POLITÉCNICA
BACHARELADO EM CIÊNCIA DA COMPUTAÇÃO
CONSTRUÇÃO DE SOFTWARE GRÁFICO 3D
PROFESSOR: LUIZ ANTONIO PAVÃO
ALUNO: MANOEL FELIPE DE ALMEIDA BINA

*/
//----------------------------------------------------------------------------------------------------------------------------------
#include <windows.h>  // for MS Windows
#include <GL\freeglut.h>  // GLUT, include glu.h and gl.h
#include <math.h>
#include <fstream>		    // File library
#include <iostream>
using namespace std;

//----------------------------------------------------------------------------------------------------------------------------------

char title[] = "OpenGL-PUC PR (Projeto Final)";
GLfloat angleV, fAspect;
GLfloat angle = 0.0f, angle1 = 0.0f;
float PI = 3.141592;
float rAngle = 0.0;
double rotate_x = 0;
double rotate_y = 0;
int iluminacao = 0;
int numObjetos;
int ListaObj[10];         // Lista de Objetos
int x[10], y[10], z[10];	// coordenadas dos objetos
int objeto;
float r[10], g[10], b[10];  // cores dos objetos

//----------------------------------------------------------------------------------------------------------------------------------

void update(int value) {
	rAngle += 0.5f;
	if (rAngle > 360) rAngle -= 360;
	glutPostRedisplay();				//enable/disable the spinning camera
	glutTimerFunc(25, update, 0);		//enable/disable the spinning camera
}
//----------------------------------------------------------------------------------------------------------------------------------

void initGL(void) {
	glShadeModel(GL_SMOOTH);								// Enable smooth shading
	angleV = 90;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Set background color to black and opaque
	glClearDepth(10.0f);									// Set background depth to farthest
	glEnable(GL_DEPTH_TEST);							// Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);								// Set the type of depth-test
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

	GLfloat luzAmbiente[4] = { 0.2,0.2,0.2,1.0 };
	GLfloat luzDifusa[4] = { 0.7,0.7,0.7,1.0 };			// "color" 
	GLfloat luzEspecular[4] = { 1.0, 1.0, 1.0, 1.0 };	// "bright" 
	GLfloat posicaoLuz[4] = { 10.0, 250.0, 250.0, 1.0 };

	// Capacidade de brilho do material
	GLfloat especularidade[4] = { 1.0,1.0,1.0,1.0 };
	GLint especMaterial = 60;
	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH); // glShadeModel(GL_FLAT);
	// Define a refletância do material 
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, especMaterial);
	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);
	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
}
//----------------------------------------------------------------------------------------------------------------------------------

// Função para leitura do arquivo de cena
void DisplayFileRead(const char* fileName) // na versão 2015 (char * fileName)
{
	fstream inStream;
	inStream.open(fileName, ios::in); // abre o arquivo
	if (inStream.fail()) return;      //falha na abertura do arquivo
	cout << "Abertura do arquivo com sucesso ..." << endl;
	inStream >> numObjetos;			  // lê primeira linha do arquivo, número de objetos 
	cout << numObjetos << " Objetos na cena ..." << endl;
	for (int i = 1; i <= numObjetos; i++) { // leitura das demais linhas, ID dos objetos, posição e cor
		inStream >> ListaObj[i] >> x[i] >> y[i] >> z[i] >> r[i] >> g[i] >> b[i];
	}
	inStream.close();				 // fecha o arquivo
}

//----------------------------------------------------------------------------------------------------------------------------------

// função para criação do objeto cilindro
void cilindro(float radius, float height, int numeroLados) {

	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 4.0);

	double aumento = 2.0 * PI / numeroLados;

	double x, z;
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_TRIANGLE_FAN); //BASE
	glVertex3f(0.0, 0.0, 0.0); // CENTRO
	for (double angle = 0.0; angle < (2.0 * PI) + aumento; angle += aumento)
	{
		x = radius * sin(angle);
		z = radius * cos(angle);
		glVertex3f(x, 0.0, z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN); //TOPO
	glVertex3f(0.0, height, 0.0); // CENTRO
	for (double angle = 0.0; angle < (2.0 * PI) + aumento; angle += aumento)
	{
		x = radius * sin(angle);
		z = radius * cos(angle);
		glVertex3f(x, height, z);
	}
	glEnd();

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP); // LATERAL

	for (double angle = 0.0; angle < (2.0 * PI) + aumento; angle += aumento)
	{
		x = radius * sin(angle);
		z = radius * cos(angle);
		glVertex3f(x, 0.0, z);
		glVertex3f(x, height, z);
	}
	glEnd();

}
//----------------------------------------------------------------------------------------------------------------------------------

// função para criação do objeto cone
void Cone(float r, float H)
{
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();	
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 4.0);

	glColor3f(0.1f, 1.0f, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	for (double angle = 0; angle < 2 * 3.14; angle += 2 * 3.14 / 36)
		glVertex3f(r * sin(angle), 0, r * cos(angle));
	glEnd();
	glColor3f(0.7f, 0.0f, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, H, 0);
	for (double angle = 0; angle < 2 * 3.14; angle += 2 * 3.14 / 36)
		glVertex3f(r * sin(angle), 0, r * cos(angle));
	glEnd();
	glTranslatef(-8, 0, 0);


}

//----------------------------------------------------------------------------------------------------------------------------------

// função para criação dos objetos prisma
void prisma1(float radius, float height, int numeroLados) {

	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 4.0);

	double aumento = 2.0 * PI / numeroLados;

	double x, z;
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_TRIANGLE_FAN); //BASE
	glVertex3f(0.0, 0.0, 0.0); // CENTRO
	for (double angle = 0.0; angle < (2.0 * PI) + aumento; angle += aumento)
	{
		x = radius * sin(angle);
		z = radius * cos(angle);
		glVertex3f(x, 0.0, z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN); //TOPO
	glVertex3f(0.0, height, 0.0); // CENTRO
	for (double angle = 0.0; angle < (2.0 * PI) + aumento; angle += aumento)
	{
		x = radius * sin(angle);
		z = radius * cos(angle);
		glVertex3f(x, height, z);
	}
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP); // LATERAL

	for (double angle = 0.0; angle < (2.0 * PI) + aumento; angle += aumento)
	{
		x = radius * sin(angle);
		z = radius * cos(angle);
		glVertex3f(x, 0.0, z);
		glVertex3f(x, height, z);
	}
	glEnd();

}

//----------------------------------------------------------------------------------------------------------------------------------

/* Handler for window-repaint event. Called back when the window first appears and
whenever the window needs to be re-painted. */
void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

	if (iluminacao == true) {
		glEnable(GL_LIGHTING);
	}

	else {
		glDisable(GL_LIGHTING);
	}
	glPushMatrix();
	glRotatef(rAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(rotate_x, 1.0f, 0.0f, 0.0f);
	glRotatef(rotate_y, 0.0f, 1.0f, 0.0f);

	//------------------------------------------
	for (int i = 1; i <= numObjetos; i++) {
		glTranslatef(x[i], y[i], z[i]);
		glColor3f(r[i], g[i], b[i]);
		glColor3f(1, 0, 0);
		printf("%f %f %f\n", r[i], g[i], b[i]);
		switch (ListaObj[i]) {
		case 1:
			if (objeto == 1) {
				//retangulo 
				glPushMatrix();
				prisma1(20.0, 58.0, 4);
				glPopMatrix();
			}
			break;
		case 2:
			if (objeto == 2) {
				//objetos da biblioteca glut

				//teapot
				glPushMatrix();
				glRotatef(-30, 0.0, 1.0, 1.0);
				glutSolidTeapot(10);
				glPopMatrix();

				//teacup
				glPushMatrix();
				glTranslatef(20.0, -8.0, -9.0);
				glRotatef(40, 0, 1.0, 0);
				glutSolidTeacup(10);
				glPopMatrix();

			}
			break;
		case 3:
			if (objeto == 3) {
				//esfera
				glPushMatrix();
				glRotatef(40, 0, 0, 0);
				glutSolidSphere(9, 24, 9);
				glPopMatrix();

			}
			break;
		case 4:
			if (objeto == 4) {
				//cubo
				glPushMatrix();
				glRotatef(0, 1.0, 1.0, 1.0);
				glutSolidCube(24);
				glPopMatrix();

			}
			break;
		case 5:
			if (objeto == 5) {
				//cilindro
				glPushMatrix();
				glRotatef(0, 4, 4, 8);
				cilindro(10.0, 80.0, 36);
				glPopMatrix();


			}
			break;
		case 6:
			if (objeto == 6) {
				//teraedro
				glPushMatrix();
				glRotatef(80, 40, 40, 10);
				glutSolidTetrahedron();
				glPopMatrix();

			}
			break;
		case 7:
			if (objeto == 7) {
				//dodecaedro
				glPushMatrix();
				glRotatef(80, 40, 40, 10);
				glutSolidDodecahedron();
				glPopMatrix();

			}
			break;
		case 8:
			if (objeto == 8) {
				//cone
				glPushMatrix();
				glRotatef(180, 0, 0, 0);
				Cone(20.0, 30.0);
				glPopMatrix();

			}
			break;
		case 9:
			if (objeto == 9) {
				// prisma pentagonal
				glPushMatrix();
				prisma1(20.0, 58.0, 5);
				glPopMatrix();

				// prisma hexagonal
				glPushMatrix();
				glTranslatef(80.0, 0.0, 0.0);
				prisma1(20.0, 58.0, 6);
				glPopMatrix();

			}
			break;

			// limpar objetos da tela
		case 0:

			glutSwapBuffers();
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------------------

/* Handler for window re-size event. Called back when the window first appears and
whenever the window is re-sized with its new width and height */
void reshape(GLsizei w, GLsizei h) {
	if (h == 0) h = 1;
	// specify dimensions from Viewport
	glViewport(0, 0, w, h);
	fAspect = w / h;
	// Função usada para especificar o volume de visualização
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();
	// Especifica a projeção perspectiva
	gluPerspective(angleV, fAspect, 0.1, 500);
	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();
	// Especifica posição do observador e do alvo
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);


}
//----------------------------------------------------------------------------------------------------------------------------------

// funções para rotacionar os objetos
void keyboard(int key, int x, int y) {

	//rotaciona para direita
	if (key == GLUT_KEY_RIGHT)
		rotate_y += 10;

	//rotaciona para esquerda
	else if (key == GLUT_KEY_LEFT)
		rotate_y -= 10;

	//rotaciona para cima
	else if (key == GLUT_KEY_UP)
		rotate_x -= 10;

	//rotaciona para baixo
	else if (key == GLUT_KEY_DOWN)
		rotate_x += 10;

	//iluminação
	else if (key == GLUT_KEY_PAGE_UP)
		iluminacao = 1;

	else if (key == GLUT_KEY_PAGE_DOWN)
		iluminacao = 0;

	glutPostRedisplay();


}

//----------------------------------------------------------------------------------------------------------------------------------

//função das ações com a comunicação com o teclado
void function_keyboard(unsigned char Key, int x, int y) {

	switch (Key)
	{
	case '0':
		objeto = 0;
		break;
	case '1':
		objeto = 1;
		break;
	case '2':
		objeto = 2;
		break;
	case '3':
		objeto = 3;
		break;
	case '4':
		objeto = 4;
		break;
	case '5':
		objeto = 5;
		break;
	case '6':
		objeto = 6;
		break;
	case '7':
		objeto = 7;
		break;
	case '8':
		objeto = 8;
		break;
	case '9':
		objeto = 9;
		break;
	}
	glutPostRedisplay();
}
//----------------------------------------------------------------------------------------------------------------------------------

// função do mouse e zoom
void mouse(int button, int state, int x, int y)
{
	float w = glutGet(GLUT_INIT_WINDOW_WIDTH);
	float h = glutGet(GLUT_INIT_WINDOW_HEIGHT);

	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {
			if (angleV >= 4) angleV -= 3;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {
			if (angleV <= 176) angleV += 3;
		}
	reshape(w, h);
	glutPostRedisplay();

}

//----------------------------------------------------------------------------------------------------------------------------------

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	DisplayFileRead("dispFile.txt");
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(640, 480);     // Set the window's initial width & height
	glutInitWindowPosition(350, 80);  // Position the window's initial top-left corner
	glutCreateWindow(title);          // Create window with the given title

	glutKeyboardFunc(function_keyboard);
	glutDisplayFunc(render);          // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);         // Register callback handler for window re-size event

	initGL();                         // Our own OpenGL initialization
	glutSpecialFunc(keyboard);	      // Register callback handler for arrow keys (keyborad)
	glutMouseFunc(mouse);
	glutTimerFunc(25, update, 0);	  // turn the objt a little bit
	glutMainLoop();                   // Enter the infinite event-processing loop
	return 0;
}