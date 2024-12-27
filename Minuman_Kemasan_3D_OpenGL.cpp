#include <GL/glew.h>
#include <GL/glut.h>
#include <FreeImage.h>
#include <stdio.h>

// Prototype
void drawBox();
void display();
void reshape(int w, int h);
void updateRotation();
GLuint loadTexture(const char* filename);

GLuint textureFront, textureBack, textureLeft, textureRight, textureTop, textureBottom;
float angleX = 0.0f, angleY = 0.0f; // Variabel rotasi

// Fungsi utama
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Minuman Kemasan 3D");

    glewInit();
    glEnable(GL_DEPTH_TEST);

    // Memuat tekstur untuk setiap sisi
    textureFront = loadTexture("assets_TehKotak/front.jpg");
    textureBack = loadTexture("assets_TehKotak/back.jpg");
    textureLeft = loadTexture("assets_TehKotak/left.jpg");
    textureRight = loadTexture("assets_TehKotak/right.jpg");
    textureTop = loadTexture("assets_TehKotak/top.jpg");
    textureBottom = loadTexture("assets_TehKotak/bottom.jpg");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(updateRotation); // Fungsi idle untuk animasi rotasi

    glutMainLoop();
    return 0;
}

// Fungsi untuk memuat tekstur dari file gambar
GLuint loadTexture(const char* filename) {
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
    FIBITMAP* image = FreeImage_Load(format, filename);
    FIBITMAP* image32Bits = FreeImage_ConvertTo32Bits(image);

    int width = FreeImage_GetWidth(image32Bits);
    int height = FreeImage_GetHeight(image32Bits);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(image32Bits));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FreeImage_Unload(image32Bits);
    FreeImage_Unload(image);

    return textureID;
}

// Fungsi untuk menggambar kubus (kemasan minuman) dengan tekstur
void drawBox() {
    glEnable(GL_TEXTURE_2D);

    // Sisi depan
    glBindTexture(GL_TEXTURE_2D, textureFront);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.3, -0.5,  0.2);
    glTexCoord2f(1.0, 0.0); glVertex3f( 0.3, -0.5,  0.2);
    glTexCoord2f(1.0, 1.0); glVertex3f( 0.3,  0.5,  0.2);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.3,  0.5,  0.2);
    glEnd();

    // Sisi belakang
    glBindTexture(GL_TEXTURE_2D, textureBack);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); 
    glVertex3f(-0.3, -0.5, -0.2);
    glTexCoord2f(1.0, 0.0); 
    glVertex3f(-0.3,  0.5, -0.2);
    glTexCoord2f(1.0, 1.0); 
    glVertex3f( 0.3,  0.5, -0.2);
    glTexCoord2f(0.0, 1.0); 
    glVertex3f( 0.3, -0.5, -0.2);
    glEnd();

    // Sisi kiri
    glBindTexture(GL_TEXTURE_2D, textureLeft);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.3, -0.5, -0.2);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.3, -0.5,  0.2);
    glTexCoord2f(1.0, 1.0); glVertex3f(-0.3,  0.5,  0.2);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.3,  0.5, -0.2);
    glEnd();

    // Sisi kanan
    glBindTexture(GL_TEXTURE_2D, textureRight);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f( 0.3, -0.5, -0.2);
    glTexCoord2f(1.0, 0.0); glVertex3f( 0.3, -0.5,  0.2);
    glTexCoord2f(1.0, 1.0); glVertex3f( 0.3,  0.5,  0.2);
    glTexCoord2f(0.0, 1.0); glVertex3f( 0.3,  0.5, -0.2);
    glEnd();

    // Sisi atas
    glBindTexture(GL_TEXTURE_2D, textureTop);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.3,  0.5, -0.2);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.3,  0.5,  0.2);
    glTexCoord2f(1.0, 1.0); glVertex3f( 0.3,  0.5,  0.2);
    glTexCoord2f(0.0, 1.0); glVertex3f( 0.3,  0.5, -0.2);
    glEnd();

    // Sisi bawah
    glBindTexture(GL_TEXTURE_2D, textureBottom);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.3, -0.5, -0.2);
    glTexCoord2f(1.0, 0.0); glVertex3f( 0.3, -0.5, -0.2);
    glTexCoord2f(1.0, 1.0); glVertex3f( 0.3, -0.5,  0.2);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.3, -0.5,  0.2);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Fungsi untuk merender objek
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Mengatur kamera
    gluLookAt(2.0, 2.0, -2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Rotasi objek
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // Menggambar kotak
    drawBox();

    glutSwapBuffers();
}

// Fungsi untuk menangani perubahan ukuran jendela
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

// Fungsi untuk memperbarui rotasi objek
void updateRotation() {
    angleX += 0.01f; // Kecepatan rotasi pada sumbu X
    angleY += 0.01f; // Kecepatan rotasi pada sumbu Y

    if (angleX > 360.0f) angleX -= 360.0f;
    if (angleY > 360.0f) angleY -= 360.0f;

    glutPostRedisplay(); // Meminta redisplay
}
