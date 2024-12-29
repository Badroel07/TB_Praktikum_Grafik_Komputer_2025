#include <GL/glew.h>
#include <GL/glut.h>
#include <FreeImage.h>
#include <stdio.h>

// Prototype
void TehKotak();
void drawCartecius();
void display();
void reshape(int w, int h);
void updateRotation();

// variabel global
GLuint loadTexture(const char* filename);
GLuint texture_sisiKaleng, texture_atas, texture_bawah, tekstur_kaleng;
GLuint textureFront, textureBack, textureLeft, textureRight, textureTop, textureBottom;
float angleX = 0.0f, angleY = 0.0f; // Variabel rotasi
float tick = 0.0;

// Fungsi utama
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Minuman Kemasan 3D");

    glewInit();
    glEnable(GL_DEPTH_TEST);

    // Tekstur Teh Kotak
    textureFront = loadTexture("assets/Teh_Kotak/front.png");
    textureBack = loadTexture("assets/Teh_Kotak/back.png");
    textureLeft = loadTexture("assets/Teh_Kotak/left.png");
    textureRight = loadTexture("assets/Teh_Kotak/right.png");
    textureTop = loadTexture("assets/Teh_Kotak/top.png");
    textureBottom = loadTexture("assets/Teh_Kotak/bottom.png");
    // Tekstur Susu Kaleng
    texture_sisiKaleng = loadTexture("assets/Susu_Kaleng/sisi.png");
    texture_atas = loadTexture("assets/Susu_Kaleng/atas.png");
    texture_bawah = loadTexture("assets/Susu_Kaleng/bawah.png");
    tekstur_kaleng = loadTexture("assets/Susu_Kaleng/tekstur_kaleng.png");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(updateRotation); // Fungsi idle untuk animasi rotasi

    glutMainLoop();
    return 0;
}

// Fungsi untuk memuat tekstur dari file gambar
GLuint textureID = 0;
GLuint loadTexture(const char* path) {
    glGenTextures(1, &textureID);
    void* imgData;
    int imgWidth;
    int imgHeight;
    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(path);
    if (format == FIF_UNKNOWN) {
        printf("Unknown file type for texture image file %s\n", path);
        return -1;
    }
    FIBITMAP* bitmap = FreeImage_Load(format, path, 0);
    if (!bitmap) {
        printf("Failed to load image %s\n", path);
        return -1;
    }
    FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap);
    FreeImage_Unload(bitmap);
    imgData = FreeImage_GetBits(bitmap2);
    imgWidth = FreeImage_GetWidth(bitmap2);
    imgHeight = FreeImage_GetHeight(bitmap2);
    if (imgData) {
        printf("Texture image loaded from file %s, size %dx%d\n", path, imgWidth, imgHeight);
        int format;
        if (FI_RGBA_RED == 0)
            format = GL_RGB;
        else
            format = GL_BGR;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, imgData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        textureID++;
        return textureID - 1;
    } else {
        printf("Failed to get texture data from %s\n", path);
    }
    return -1;
}

void SusuKaleng() {
    GLUquadric* object = gluNewQuadric();
    gluQuadricTexture(object, GL_TRUE);
    gluQuadricNormals(object, GLU_SMOOTH);

    float radius = 0.17f; // Sesuaikan diameter susu kaleng (lebar Teh Kotak adalah 0.6, jadi radius 0.2)
    float height = 0.4f; // Sesuaikan tinggi susu kaleng (tinggi Teh Kotak adalah 1.0, jadi kaleng lebih kecil)

    // Cylinder (sisi kaleng)
    glBindTexture(GL_TEXTURE_2D, texture_sisiKaleng);
    gluCylinder(object, radius, radius, 0.6f, 50, 4);

    // Top (penutup atas)
    glPushMatrix();
    glTranslatef(0, 0, 0.62); // Pindahkan ke atas kaleng
    glBindTexture(GL_TEXTURE_2D, texture_atas);
    gluDisk(object, 0, radius, 50, 2);
    glPopMatrix();

    // Bottom (penutup bawah)
    glPushMatrix();
    glTranslatef(0, 0, 0); // Pindahkan ke bawah kaleng
    glBindTexture(GL_TEXTURE_2D, texture_bawah);
    gluDisk(object, 0, radius, 40, 1);
    glPopMatrix();
}

void SusuKaleng2() {
    GLUquadric* object = gluNewQuadric();
    gluQuadricTexture(object, GL_TRUE);
    gluQuadricNormals(object, GLU_SMOOTH);

    float radius = 0.2f; // Sesuaikan diameter susu kaleng (lebar Teh Kotak adalah 0.6, jadi radius 0.2)
    float height = 0.4f; // Sesuaikan tinggi susu kaleng (tinggi Teh Kotak adalah 1.0, jadi kaleng lebih kecil)

    // Cylinder (sisi kaleng)
    glBindTexture(GL_TEXTURE_2D, tekstur_kaleng);
    gluCylinder(object, 0.1699f, 0.1699f, 0.6159f, 50, 4);
}
// Fungsi untuk menggambar kubus (kemasan minuman) dengan tekstur
void TehKotak() {
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
    drawCartecius();

    // Rotasi objek utama (Teh Kotak)
    glPushMatrix();
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // Menggambar Teh Kotak
    TehKotak();
    glPopMatrix();

    // Rotasi dan posisi objek kedua (Susu Kaleng)
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslatef(1.0, 0.0, 0.0); // Posisikan di samping Teh Kotak
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 0.0f, 1.0f);

    // Menggambar Susu Kaleng
    SusuKaleng2();
    SusuKaleng();
    glPopMatrix();

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

void drawCartecius()
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    //x line
    glVertex3f(-10.0, 0.0, 0.0);
    glVertex3f(10.0, 0.0, 0.0);

    //y line
    glVertex3f(0.0, -10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);

    //z line
    glVertex3f(0.0, 0.0, -10.0);
    glVertex3f(0.0, 0.0, 10.0);
    glEnd();
}
