#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include <windows.h>
#include <GL/glut.h>
#include "draw.h"
using namespace std;
#define pi (2*acos(0.0))
extern void toMau(GLfloat amdiff[]);

double cameraHeight, cameraAngle;
double angle, map_angle;
double move_pos = 2;
double l = 140, w = 140, h = 20;
bool eaten, started = false, pause = false;
int sg = 5, ar = 0, score = 0, highestScore;

point o(0, 0, 0);
point pos, fruit;
class segment
{
public:
    int ang, x, y, z;
    segment()
    {
        ang = 0; x = 0; y = 0; z = 0;
    }
};
point head;
segment sgs[1000];
void keyListener(unsigned char key, int x, int y);
void specialKeyListener(int key, int x, int y);
void drawBoard();
void write(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    }
}
// loadtexture
GLuint loadimage(const char* fileName)
{
    FILE* file;
    unsigned char header[54], * data;
    unsigned int dataPos, size, width, height;
#pragma warning(disable:4996)
    file = fopen(fileName, "rb");
    fread(header, 1, 54, file);
    dataPos = *(int*)&(header[0x0A]);
    size = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);
    if (size == NULL)
        size = width * height * 3;
    if (dataPos == NULL)
        dataPos = 54;
    data = new unsigned char[size];
    fread(data, 1, size, file);
    fclose(file);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
    return texture;
}

GLint skin;
GLint apple;
GLint grass;
GLint brick;
GLint head1;

void initTexture() {
    skin = loadimage("snake1.bmp");
    apple = loadimage("apple.bmp");
    grass = loadimage("grass.bmp");
    brick = loadimage("brick.bmp");
    head1 = loadimage("snake1.bmp");
}
// Luu diem cao vao file txt
void saveHighestScore(int s = score)
{
    ofstream file;
    file.open("C:\\Users\\dotie\\OneDrive\\Desktop\\New folder\\Snake 3D Pro\\Snake 3D Pro\\Snake 3D Pro\\HighScore.txt");
    file << s;
    file.close();
}

// Lay diem cao nhat
int getHighestScore(int s = score)
{
    ifstream file;
    int sc = 0;
    file.open("C:\\Users\\dotie\\OneDrive\\Desktop\\New folder\\Snake 3D Pro\\Snake 3D Pro\\Snake 3D Pro\\HighScore.txt");
    if (file.is_open())
    {
        file >> sc;
        file.close();
    }
    return sc;
}

// Ve ran
void drawSnake()
{

    // Ve dau ran
    int i;
    point c1(0.6, 0.1, 0.1), c2(0, 0, 0), c3(1, 1, 1);

    for (i = 1; i < sg; i++)
    {
        glPushMatrix();
        glTranslated(sgs[i].x * 10, sgs[i].y * 10, 0);
        draw::solid(o, 10, 10, 8, c1, c2, c3,skin);
        glPopMatrix();
    }

    // Ve than ran
    c1.set(0.5, 0.5, 1);
    glPushMatrix();
    glTranslated(sgs[0].x * 10, sgs[0].y * 10, 0);
    draw::solid(o, 12, 12, 10, c1, c2, c3,0);
    glPopMatrix();
}

// Thiet lap ket thuc game
bool isDeath()
{
    if (abs(head.x) >= l || abs(head.y) >= w)
    {
        return true;
    }

    for (int i = 1; i < sg; i++)
    {
        if (sgs[0].x == sgs[i].x && sgs[0].y == sgs[i].y)
            return true;
    }
    return false;
}

// Thiet lap hàm ran an moi
bool isEaten()
{
    if (head.x == fruit.x && head.y == fruit.y)
        return true;
    else
        return false;
}

// Thiet lap moi xuan hien
void drawFruit()
{
    if (eaten)
    {
        int ll = floor(l / 10), ww = floor(w / 10);
        fruit.x = (rand() % (2 * ll) - ll) * 10; fruit.y = (rand() % (2 * ww) - ww) * 10;
        eaten = false;
    }
    point f = fruit;
    point c1(0.6, 0.1, 0.1), c2(0, 0, 0), c3(1, 1, 1);
    draw::solid(f, 6, 6, 8, c1, c2, c3,apple);
}


void pregame()
{
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    glRasterPos3f(20, 130, 10);
    stringstream ss;
    ss << highestScore;
    write("Ky luc: " + ss.str());

    glColor3f(1, 0, 0);
    glRasterPos3f(-100, -50, 70);
    write("Nhan F1 de bat dau tro choi");

    glColor3f(0.9, 0.9, 0.9);
    glRasterPos3f(-100, -50, 0);
    write("Di chuyen cua ran");
    glRasterPos3f(-100, -65, 0);
    write("4 - Di trai");
    glRasterPos3f(-100, -80, 0);
    write("6 - Di phai");
    glRasterPos3f(-100, -95, 0);
    write("F1 - Bat dau/Bat dau lai tro choi");
    glRasterPos3f(-100, -110, 0);
    write("F2 - Tam Dung/Tiep tuc tro choi");

    glColor3f(0, 0, 1);
    glRasterPos3f(-140, -100, -20);
    write("Ban se chet neu ban dam vao canh  ban hoac tu dam vao ban than");
    glEnable(GL_LIGHTING);
    glutSwapBuffers();
}
void display()
{

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(200 * cos(cameraAngle), 200 * sin(cameraAngle), cameraHeight, 0, 0, -80, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);

    if (!started)
    {
        pregame();
        return;
    }
    // Score
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    glRasterPos3f(100, 100, 70);
    stringstream ss, ss1;
    ss << score;
    write("Diem: " + ss.str());
    glRasterPos3f(100, 100, 40);
    ss1 << highestScore;
    write("Ky luc: " + ss1.str());
    glEnable(GL_LIGHTING);
    // Add objects from here
    //draw::axes();
    drawBoard();
    
    drawFruit();
    
    if (isEaten())
    {
        eaten = true;
        drawFruit();
        score++;
        sg++;
    }

    if (isDeath())
    {
        if (score > highestScore) saveHighestScore(score);
        sgs[0] = sgs[1];
        drawSnake();
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 1);
        glRasterPos3f(-50, 0, 20);
        write("Tro choi ket thuc");
        glEnable(GL_LIGHTING);
    }
    else
    {
        for (int i = sg - 1; i > 0; i--)
        {
            sgs[i] = sgs[i - 1];
        }
        sgs[0].ang = ar;
        sgs[0].x += coss(ar);
        sgs[0].y += sinn(ar);
        cout << sgs[0].x << " " << sgs[0].y << " " << sgs[0].ang << " " << ar << " " << coss(ar) << " " << sinn(ar) << endl << endl;
        head.x += 10 * coss(ar);
        head.y += 10 * sinn(ar);
        if (!isDeath()) drawSnake();
    }

    //Double Buffer
    glutSwapBuffers();
}
// hàm thiết lập độ sáng và màu của nguồn sáng (light0)
void lighting() // FUNCTION FOR LIGHTING
{
    glClearColor(0.2, 0.2, 0.2, 1.0);
    // printf("%s\n", "YESS ");
    GLfloat light_directional[] = { 1.0,1.0,1.0,1.0 };
    GLfloat light_diffuse[] = { 0.8,0.8,0.8,1.0 };
    GLfloat light_ambient[] = { 0.2,0.2,0.2,1.0 };

    GLfloat light_positional[] = { 10.0,10.0,10.0,1.0 };

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_positional);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}
void init()
{
    cameraHeight = 160.0; cameraAngle = pi + 0.9; angle = 0; map_angle = 90;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80, 1, 10, 1000.0);
    sg = 5; ar = 0;
    head.set(0, 0, 0);
    for (int i = 0; i < sg; i++)
    {
        sgs[i].x = -i;
        sgs[i].y = 0;
        sgs[i].ang = 0;
    }
    eaten = true; pause = false;
    score = 0;
    highestScore = getHighestScore();
    lighting();
}

// Toc do di chuyen cua ran
void animate(int value)
{
    if (!pause)
        glutPostRedisplay();
    glutTimerFunc(200, animate, value);
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(980, 680);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow("Snake 3D");
    initTexture();
    init();

    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutTimerFunc(200, animate, 1);

    glutSpecialFunc(specialKeyListener);
    glutKeyboardFunc(keyListener);

    glutMainLoop();

    return 0;
}

// Thiet lap phim di chuyen
void keyListener(unsigned char key, int x, int y)
{
    if (pause || !started)
        return;
    switch (key)
    {
    case 'a':
    case '4':
        ar++;
        break;

    case 'd':
    case '6':
        ar--;
        break;
    }
    if (ar < 0) ar = 4 + ar;
    if (ar >= 4) ar = ar % 4;
    glutPostRedisplay();
}

// Ve ban 
void drawBoard()
{

    int d = 10;
    
    //glColor3f(0.2, 0.8, 0);
    GLfloat mau1[] = { 0.2, 0.8, 0 };
    toMau(mau1);
    point p1(l + 2, w + 2, 0), p2(l + 2, -w - 2, 0), p3(-l - 2, -w - 2, 0), p4(-l - 2, w + 2, 0);
    draw::rectangle(p1, p2, p3, p4,grass);

    glBegin(GL_LINES); {
        //glColor3f(0, 0, 0);
        toMau(qaBlack);
        for (int i = -l; i <= l; i += d)
        {
            glVertex3f(l, i, 0);
            glVertex3f(-l, i, 0);
            glVertex3f(i, -w, 0);
            glVertex3f(i, w, 0);
        }
    }glEnd();

    //glColor3f(1, 0, 0);
    toMau(qaRed);
    double ll = l + 2, ww = w + 2;
    p1.set(ll, ww, 0); p2.set(ll, ww, h); p3.set(ll, -ww, h); p4.set(ll, -ww, 0);
    draw::rectangle(p1, p2, p3, p4, brick);

    p1.set(ll, -ww, 0); p2.set(ll, -ww, h); p3.set(-ll, -ww, h); p4.set(-ll, -ww, 0);
    draw::rectangle(p1, p2, p3, p4, brick);

    p1.set(-ll, -ww, 0); p2.set(-ll, -ww, h); p3.set(-ll, ww, h); p4.set(-ll, ww, 0);
    draw::rectangle(p1, p2, p3, p4, brick);

    p1.set(-ll, ww, 0); p2.set(-ll, ww, h); p3.set(ll, w, h); p4.set(ll, ww, 0);
    draw::rectangle(p1, p2, p3, p4, brick);
}

// Thiet lap phim chuc nang 
void specialKeyListener(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_DOWN:
        cameraHeight -= 3.0;
        break;
    case GLUT_KEY_UP:
        cameraHeight += 3.0;
        break;
    case GLUT_KEY_RIGHT:
        cameraAngle += 0.03;
        break;
    case GLUT_KEY_LEFT:
        cameraAngle -= 0.03;
        break;
    case GLUT_KEY_F1:
        started = true;
        init();
        break;
    case GLUT_KEY_F2:
        pause = !pause;
        break;
    default:
        break;
        glutPostRedisplay();
    }
}