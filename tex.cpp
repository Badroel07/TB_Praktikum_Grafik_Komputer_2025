#include <GL/glew.h>
#include <GL/glut.h>
#include <FreeImage.h>
#include <stdio.h>
using namespace std;

GLuint texture_sisiKaleng;
GLuint texture_atas;
GLuint texture_bawah;

void createObject() {
    GLUquadric* object = gluNewQuadric();
    gluQuadricTexture(object, GL_TRUE);
    gluQuadricNormals(object, GLU_SMOOTH);

    // Cylinder 
    gluCylinder(object, 2, 2, 7, 50, 4);

    // Top 
    glPushMatrix();
    glTranslatef(0, 0, 7); // Move to the top of the cylinder
    glBindTexture(GL_TEXTURE_2D, texture_atas);
    gluDisk(object, 0, 2, 50, 2);
    glPopMatrix();

    // Bottom 
    glPushMatrix();
    glTranslatef(0, 0, 0); // Move to the bottom of the cylinder
    glBindTexture(GL_TEXTURE_2D, texture_bawah);
    gluDisk(object, 0, 2, 40, 1);
    glPopMatrix();
}

float tick = 0.0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(-90, 1.0, 0.0, 0.0);
    glRotatef(tick += 0.01, 0.0, 0.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, texture_sisiKaleng);
    createObject();

    glPopMatrix();
    glutSwapBuffers();
    glutPostRedisplay();
}

GLuint textureID = 0;
int loadTexture(const char* path) {
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

void initProjection() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_TEXTURE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, 1800 / 900, 1.0, 100.0);
    gluLookAt(10.0, 13.0, 5.0,
              0.0, 5.0, 0.0,
              0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);

    texture_sisiKaleng = loadTexture("susu steril sisi.jpg");
    texture_atas = loadTexture("susu steril atas.jpg");
    texture_bawah = loadTexture("susu steril bawah.jpg");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glewInit();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1800, 900);
    glutInitWindowPosition(100, -50);
    glutCreateWindow("Texture");
    initProjection();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
