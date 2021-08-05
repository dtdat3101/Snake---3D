#ifndef DRAW_H
#define DRAW_H
#define pi (2*acos(0.0))
// cac mau co ban cho lighting
static GLfloat qaBlack[] = { 0,0,0,1.0 };
static GLfloat qaGreen[] = { 0,1.0,0,1.0 };
static GLfloat qaRed[] = { 1.0,0,0,1.0 };
static GLfloat qaBlue[] = { 0,0,1.0,1.0 };
static GLfloat qaWhite[] = { 1.0,1.0,1.0,1.0 };
static GLfloat qaYellow[] = { 1.0,1.0,0,1.0 };
static GLfloat qaGrey[] = { 0.5,0.5,0.5,0.5 };
static GLfloat qaLowAmbient[] = { 0.2,0.2,0.2,1.0 };
static GLfloat qaFullAmbient[] = { 1.0,1.0,1.0,1.0 };

// hàm tô màu cho chất liệu của vật thể
void toMau(GLfloat amdiff[]) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, amdiff);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, amdiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
    glMaterialfv(GL_LIGHT0, GL_AMBIENT, qaLowAmbient);
}
double d2r(double d)
{
    return (d) * (double)(pi / 180);
}
double r2d(double r)
{
    return (r) * (double)(180 / pi);
}
int sinn(int a)
{
    switch (a)
    {
    case 0: return 0;
    case 1: return 1;
    case 2: return 0;
    case 3: return -1;
    }
}
int coss(int a)
{
    switch (a)
    {
    case 0: return 1;
    case 1: return 0;
    case 2: return -1;
    case 3: return 0;
    }
}
class point
{
public:
    double x, y, z;
    point()
    {
        x = 0; y = 0; z = 0;
    }
    point(double a, double b, double c)
    {
        x = a; y = b; z = c;
    }
    void set(double a, double b, double c)
    {
        x = a; y = b; z = c;
    }
    void reset()
    {
        x = 0; y = 0; z = 0;
    }
};
class draw
{
public:
    static void axes()
    {
        glBegin(GL_LINES); {
            //glColor3f(0, 1, 0);
            toMau(qaGreen);
            glVertex3f(100, 0, 0);
            glVertex3f(-100, 0, 0);

            //glColor3f(1, 0, 0);
            toMau(qaRed);
            glVertex3f(0, -100, 0);
            glVertex3f(0, 100, 0);

            //glColor3f(0, 0, 1);
            toMau(qaBlue);
            glVertex3f(0, 0, 100);
            glVertex3f(0, 0, -100);
        }glEnd();
    }
    static void triangle(point p1, point p2, point p3)
    {
        glBegin(GL_TRIANGLES); {
            glVertex3f(p1.x, p1.y, p1.z);
            glVertex3f(p2.x, p2.y, p2.z);
            glVertex3f(p3.x, p3.y, p3.z);
        }glEnd();
    }

    static void rectangle(point p1, point p2, point p3, point p4,GLint tex)
    {
        if (tex !=0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        }
        glBegin(GL_QUADS); {
            glVertex3f(p1.x, p1.y, p1.z); glTexCoord2f(0.00, 0.0);
            glVertex3f(p2.x, p2.y, p2.z); glTexCoord2f(0.00, 1.0);
            glVertex3f(p3.x, p3.y, p3.z); glTexCoord2f(1.00, 1.0);
            glVertex3f(p4.x, p4.y, p4.z); glTexCoord2f(1.00, 0.0);
        }glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    static void solid(point p1, double l, double w, double h, point c1, point c2, point c3,GLint tex)
    {
        point p2, p3, p4, p5, p6, p7, p8;
        p2 = p3 = p4 = p1;
        p2 = p1; p2.x -= l;
        p3 = p2; p3.y -= w;
        p4 = p1; p4.y -= w;
        p5 = p1; p6 = p2; p7 = p3; p8 = p4;
        p5.z += h; p6.z += h; p7.z += h; p8.z += h;

        //glColor3f(c1.x, c1.y, c1.z);
        GLfloat mau1[] = { c1.x, c1.y, c1.z };
        toMau(mau1);
        rectangle(p1, p2, p3, p4,tex);
        rectangle(p5, p6, p7, p8,tex);

        //glColor3f(c2.x, c2.y, c2.z);
        GLfloat mau2[] = { c2.x, c2.y, c2.z };
        toMau(mau2);
        rectangle(p1, p2, p6, p5,tex);
        rectangle(p1, p4, p8, p5,tex);
        rectangle(p2, p3, p7, p6,tex);
        rectangle(p4, p3, p7, p8,tex);
    }
};
#endif
