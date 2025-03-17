#include <GL/glut.h>

static int shoulder1 = 0, shoulder2 = 0, shoulder3 = 0, elbow = 0, hand = 0;
static int finger1 = 0, finger2 = 0, finger3 = 0, finger4 = 0, finger5 = 0;

void init(void) {
	glClearColor (0.0, 0.0, 0.0, 0.0); 
	glShadeModel (GL_FLAT);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT); 
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glScalef(0.45, 0.45, 0.45);
	glRotatef((GLfloat)shoulder1, 0.0, 1.0, 0.0);
	glRotatef((GLfloat)shoulder2, 1.0, 0.0, 0.0);
	glRotatef((GLfloat)shoulder3, 0.0, 0.0, 1.0);
	glTranslatef(1.5, 0.0, 0.0);
	glPushMatrix();
	glScalef(3.0, 0.4, 1.0); 
	glutWireCube(1.0); 
	glPopMatrix();
	
	glTranslatef (1.0, 0.0, 0.0);
	glRotatef((GLfloat) elbow, 0.0, 0.0, 1.0);
	glTranslatef(1.5, 0.0, 0.0);
	glPushMatrix();
	glScalef(3.0, 0.4, 1.0); 
	glutWireCube(1.0); 
	glPopMatrix();
	
	glTranslatef(1.0, 0.0, 0.0);
	glRotatef((GLfloat)hand, 0.0, 0.0, 1.0);
	glTranslatef(0.5, 0.0, 0.0);
	glPushMatrix();
	glScalef(1.0, 0.4, 1.0);
	glutWireCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.0, -0.4);
	glRotatef((GLfloat)finger1, 0.0, 0.0, 1.0);
	glTranslatef(0.35, 0.0, 0.0);
	glScalef(0.7, 0.4, 0.2);
	glutWireCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.0, -0.125);
	glRotatef((GLfloat)finger2, 0.0, 0.0, 1.0);
	glTranslatef(0.4, 0.0, 0.0);
	glScalef(0.8, 0.4, 0.2);
	glutWireCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.0, 0.125);
	glRotatef((GLfloat)finger3, 0.0, 0.0, 1.0);
	glTranslatef(0.5, 0.0, 0.0);
	glScalef(1.0, 0.4, 0.2);
	glutWireCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.0, 0.4);
	glRotatef((GLfloat)finger4, 0.0, 0.0, 1.0);
	glTranslatef(0.4, 0.0, 0.0);
	glScalef(0.8, 0.4, 0.2);
	glutWireCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.5);
	glRotatef((GLfloat)finger5, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 0.25);
	glScalef(0.4, 0.2, 0.5);
	glutWireCube(1.0);
	glPopMatrix();

	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION); 
	glLoadIdentity ();
	gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0); 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	glTranslatef (0.0, 0.0, -5.0);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'z': shoulder1 = (shoulder1 + 5) % 360; 
            glutPostRedisplay();
            break;
        case 'Z': shoulder1 = (shoulder1 - 5) % 360;
            glutPostRedisplay();
			break;
		case 'P': shoulder2 = (shoulder2 + 5) % 360;
			glutPostRedisplay();
			break;
		case 'p': shoulder2 = (shoulder2 - 5) % 360;
			glutPostRedisplay();
			break;
		case 'x': shoulder3 = (shoulder3 + 5) % 360;
			glutPostRedisplay();
			break;
		case 'X': shoulder3 = (shoulder3 - 5) % 360;
			glutPostRedisplay();
			break;
		case 'w': elbow = (elbow + 5) % 360;
            glutPostRedisplay();
            break;
		case 'W': elbow = (elbow - 5) % 360;
			glutPostRedisplay();
			break;
		case 'y': hand = (hand + 5) % 360;
			glutPostRedisplay();
			break;
		case 'Y': hand = (hand - 5) % 360;
			glutPostRedisplay();
			break;
		case 'a': finger1 = (finger1 + 5) % 360;
			glutPostRedisplay();
			break;
		case 'A': finger1 = (finger1 - 5) % 360;
			glutPostRedisplay();
			break;
		case 'b': finger2 = (finger2 + 5) % 360;
			glutPostRedisplay();
			break;
		case 'B': finger2 = (finger2 - 5) % 360;
			glutPostRedisplay();
			break;
		case 'c': finger3 = (finger3 + 5) % 360;
			glutPostRedisplay();
			break;
		case 'C': finger3 = (finger3 - 5) % 360;
			glutPostRedisplay();
			break;
		case 'd': finger4 = (finger4 + 5) % 360;
			glutPostRedisplay();
			break;
		case 'D': finger4 = (finger4 - 5) % 360;
			glutPostRedisplay();
			break;
		case 'e': finger5 = (finger5 - 5) % 360;
			glutPostRedisplay();
			break;
		case 'E': finger5 = (finger5 + 5) % 360;
			glutPostRedisplay();
			break;
		case 27: exit(0);
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); 
	glutInitWindowSize (700, 600); 
	glutInitWindowPosition (100, 100); 
	glutCreateWindow (argv[0]); 
	init();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape); 
	glutKeyboardFunc(keyboard); 
	glutMainLoop();
}