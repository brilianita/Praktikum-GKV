#include <GL/glut.h>
#include <math.h>

void roda(float radius, int segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

void jalan() {
    // jalan hitam
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -0.4f);
    glVertex2f(1.0f, -0.4f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    // garis putih di tengah jalan
    glColor3f(1.0f, 1.0f, 1.0f); 
    for (float i = -0.9f; i < 1.0f; i += 0.3f) {
        glBegin(GL_POLYGON);
        glVertex2f(i, -0.55f);
        glVertex2f(i + 0.15f, -0.55f);
        glVertex2f(i + 0.15f, -0.52f);
        glVertex2f(i, -0.52f);
        glEnd();
    }
}

void mobil() {
    glPushMatrix();
    glTranslatef(0.0f, -0.3f, 0.0f);
    // body mobil
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.6f, 0.0f);
    glVertex2f(0.6f, 0.0f);
    glVertex2f(0.4f, 0.15f);
    glVertex2f(-0.4f, 0.15f);
    glEnd();

	// Jok
	glColor3f(0.2f, 0.2f, 0.2f);
	glBegin(GL_POLYGON);	
	glVertex2f(-0.15f, 0.15f);
	glVertex2f(0.01f, 0.15f);
	glVertex2f(0.01f, 0.23f);
	glVertex2f(-0.15f, 0.23f);
	glEnd();

    // Kaca depan
    glColor3f(0.5f, 0.7f, 0.9f);
    glBegin(GL_POLYGON);
    glVertex2f(0.1f, 0.15f);
    glVertex2f(0.3f, 0.15f);
    glVertex2f(0.2f, 0.25f);
    glVertex2f(0.05f, 0.25f);
    glEnd();

    // Kaca spion
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_POLYGON);
    glVertex2f(0.35f, 0.15f);
    glVertex2f(0.38f, 0.15f);
    glVertex2f(0.37f, 0.18f);
    glVertex2f(0.34f, 0.18f);
    glEnd();

    // Roda kiri
    glPushMatrix();
    glTranslatef(-0.35f, -0.05f, 0.0f);
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    roda(0.1f, 30);
    glColor3f(0.5f, 0.0f, 0.0f);
    roda(0.05f, 20);
    glPopMatrix();

    // Roda kanan
    glPushMatrix();
    glTranslatef(0.35f, -0.05f, 0.0f);
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    roda(0.1f, 30);
    glColor3f(0.5f, 0.0f, 0.0f);
    roda(0.05f, 20);
    glPopMatrix();

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Langit
    glColor3f(0.5f, 0.8f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(1.0f, -0.4f);
    glVertex2f(-1.0f, -0.4f);
    glEnd();

    jalan();
    mobil();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Mobil");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
