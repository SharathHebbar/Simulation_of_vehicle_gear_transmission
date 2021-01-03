#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#define transcz1 -2.15
#define transcz2 4.75
#ifndef M_PI
#define M_PI 3.14159265
#endif

//Declarations
static int mainmenu;
static int os;
static int menuVal =  5;
static int gearPressed = 0;
static float clutchTransx1=-3.0,clutchTransy1=4.0,clutchTransz1=transcz1;
static float clutchTransx2=-3.0,clutchTransy2=4.0,clutchTransz2=transcz2;
static int count12 = 0,prevVal=0;
static int count34 = 0;
static int reached1 = 0, reached2 = 0, reached3 = 0, reached4 = 0;
static GLfloat view_rotx = 0.0, view_roty = 90.0, view_rotz = 0.0;
static GLint gear1, gear2, gear3, gear4, gear5,gear6,gear7,gear8,gear9,gear10,attach,clutch,attach2;
static GLfloat angle = 0.0,angle2 = 0.0;
static GLuint limit;
static GLuint count = 1;
int flag1 ;
static GLfloat color1[4] = {0.7, 0.5, 0.7, 1.0};
static GLfloat color2[4] = {0.3, 0.7, 0.3, 1.0};
static GLfloat color3[4] = {0.3, 0.5, 0.5, 1.0};
static GLfloat color4[4] = {1.0, 0.8, 0.8, 1.0};
static GLfloat color5[4] = {0.0, 1.0, 0.8, 1.0};
static GLfloat color6[4] = {1.0, 0.8, 0.8, 1.0};
static GLfloat color7[4] = {0.8, 0.8, 0.0, 1.0};
static GLfloat color9[4] = {0.25, 0.25, 1.0, 1.0};

static void idle(void);

void menu(int option)
{

    if(option == 0)
    {
        exit(0);
    }
    else
    {
        gearPressed = 1;
        menuVal=option;
        if(menuVal == 6)
        {
            glutIdleFunc(idle);
            menuVal = prevVal;
        }
        if(menuVal == 7)
        {
            glutIdleFunc(NULL);
        }
    }
    glutPostRedisplay();
}

void createMenu()
{
    glutCreateMenu(menu);
    glutAddMenuEntry("GEAR 1",1);
    glutAddMenuEntry("GEAR 2",2);
    glutAddMenuEntry("GEAR 3",3);
    glutAddMenuEntry("GEAR 4",4);
    glutAddMenuEntry("NEUTRAL",5);
    glutAddMenuEntry("START",6);
    glutAddMenuEntry("STOP",7);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

static void
gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
  GLint teeth, GLfloat tooth_depth)
{
      GLint i;
      GLfloat r0, r1, r2;
      GLfloat angle, da;
      GLfloat u, v, len;

      r0 = inner_radius;
      r1 = outer_radius - tooth_depth / 2.0;
      r2 = outer_radius + tooth_depth / 2.0;

      da = 2.0 * M_PI / teeth / 4.0;

      //glShadeModel(GL_FLAT);

      glNormal3f(0.0, 0.0, 1.0);

      /* draw front face */
      glBegin(GL_QUAD_STRIP);
      for (i = 0; i <= teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;
        glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
        glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
      }
      glEnd();

      /* draw front sides of teeth */
      glBegin(GL_QUADS);
      da = 2.0 * M_PI / teeth / 4.0;
      for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;

        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
        glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
      }
      glEnd();

      glNormal3f(0.0, 0.0, -1.0);

      /* draw back face */
      glBegin(GL_QUAD_STRIP);
      for (i = 0; i <= teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;
        glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
        glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
        glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
      }
      glEnd();

      /* draw back sides of teeth */
      glBegin(GL_QUADS);
      da = 2.0 * M_PI / teeth / 4.0;
      for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;

        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
        glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
        glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
      }
      glEnd();

      /* draw outward faces of teeth */
      glBegin(GL_QUAD_STRIP);
      for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;

        glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
        glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
        u = r2 * cos(angle + da) - r1 * cos(angle);
        v = r2 * sin(angle + da) - r1 * sin(angle);
        len = sqrt(u * u + v * v);
        u /= len;
        v /= len;
        glNormal3f(v, -u, 0.0);
        glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
        glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
        glNormal3f(cos(angle), sin(angle), 0.0);
        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
        glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
        u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
        v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
        glNormal3f(v, -u, 0.0);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
        glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
        glNormal3f(cos(angle), sin(angle), 0.0);
      }

      glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
      glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);

      glEnd();

      glShadeModel(GL_SMOOTH);

      /* draw inside radius cylinder */
      glBegin(GL_QUAD_STRIP);
      for (i = 0; i <= teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;

        glNormal3f(-cos(angle), -sin(angle), 0.0);
        glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
        glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
      }
      glEnd();

}

void newCircle(float x, float y, float radius)
{
    glBegin(GL_LINE_STRIP);
    for( float i = 0.0; i < 3.14159*1.2; i += 0.05){
        glVertex2f(x + cos(i)*radius, y + sin(i)*radius);
    }
    glEnd();
}

void DrawTextXY(double x,double y,double z,double scale,char *s)
{
    int i;
    glPushMatrix();
    glTranslatef(x,y,z);
    glScalef(scale,scale,scale);
    for(i=0;i<strlen(s);i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN,s[i]);
    glPopMatrix();
}

void bitmap_output(int x,int y,char *string,void * font);

static void init(void);

static void SOLID_CLOSED_CYLINDER
(GLUquadric* quad, GLdouble base, GLdouble top,GLdouble height, GLint slices, GLint stacks)
{
    gluCylinder(quad, base, top, height, slices, stacks);
    glRotatef(180, 1,0,0);
    gluDisk(quad, 0.0f, base, slices, 1);
    glRotatef(180, 1,0,0);
    glTranslatef(0.0f, 0.0f, height);
    gluDisk(quad, 0.0f, top, slices, 1);
    glTranslatef(0.0f, 0.0f, -height);
}

static void draw_name()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    glColor3f(0.9,0.9,0.9);
    DrawTextXY(-11.0,8,0.0,0.01,"COMPUTER GRAPHICS PROJECT");
    DrawTextXY(-6.0,6.5,0.0,0.01,"USING OPENGL");
    glColor3f(0.1,0.9,0.1);
    DrawTextXY(-11.0,3.0,0.0,0.01,"UVCE");

    glColor3f(1.0,0.8,0.0);
    DrawTextXY(-10.0,-1.0,0.0,0.008,"By:");
    DrawTextXY(-8.0,-2.0,0.0,0.005,"Sharath S Hebbar(19GACSE081) ");


    DrawTextXY(-7.0,-8,0.0,0.005,"<-- Press enter key to continue -->");

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

static void draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(flag1==0){
        draw_name();
    }

    else
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glColor3f(1.0,0.0,1.0);
        DrawTextXY(-13.0,11.0,0.0,0.01,"Right click to choose gear from menu");
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glPushMatrix();
        glRotatef(view_rotx, 1.0, 0.0, 0.0);
        glRotatef(view_roty, 0.0, 1.0, 0.0);
        glRotatef(view_rotz, 0.0, 0.0, 1.0);

        //attach on first shaft - output shaft
        glPushMatrix();
        glTranslatef(-3.0f,4.0f,-12.0f);
        glRotatef(-angle*2.0 - 9.0, 0.0, 0.0, 1.0);
        glCallList(attach);
        glPopMatrix();

        //Input Shaft
        glPushMatrix();
        glColor3f(1.0f,1.0f,1.0f);
        //DrawTextXY(-13.0,11.0,0.0,0.01,"Right fhfshdfghfklhklhklhhlkfhklhfkldhglfdkldhsdklfh.dafagfdjk;gj;lfdghdjh;gfdk;lhgldfkflkdghfdklh");
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color1);
        GLUquadric *quadric2 = gluNewQuadric();
        gluQuadricDrawStyle(quadric2, GLU_FILL);
        glTranslatef(-3.0f,4.0f,-13.0f);
        glRotatef(angle, 0.0, 0.0, 1.0);
        SOLID_CLOSED_CYLINDER(quadric2, 1.0f, 1.0f, 5.0f, 20, 20);
        gluDeleteQuadric(quadric2);
        glPopMatrix();

        //Counter Shaft
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
        GLUquadric *quadric1 = gluNewQuadric();
        gluQuadricDrawStyle(quadric1, GLU_FILL);
        glTranslatef(-3.0f,-2.0f,-12.0f);
        glRotatef(angle, 0.0, 0.0, 1.0);
        SOLID_CLOSED_CYLINDER(quadric1, 1.0f, 1.0f, 22.0f, 20, 20);
        gluDeleteQuadric(quadric1);
        glPopMatrix();

        //attach on output shaft
        glPushMatrix();
        glTranslatef(-3.0f,4.0f,11.0f);
        glRotatef(angle2, 0.0, 0.0, 1.0);
        glCallList(attach);
        glPopMatrix();

        //Output shaft
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color3);
        GLUquadric *quadric3 = gluNewQuadric();
        gluQuadricDrawStyle(quadric3, GLU_FILL);
        glTranslatef(-3.0f,4.0f,-5.0f);
        glRotatef(angle2, 0.0, 0.0, 1.0);
        SOLID_CLOSED_CYLINDER(quadric3, 1.0f, 1.0f, 17.0f, 20, 20);
        gluDeleteQuadric(quadric3);
        glPopMatrix();

        //First gear on counter shaft
        glPushMatrix();
        glTranslatef(-3.0, -2.0, -9.0);
        glRotatef(angle, 0.0, 0.0, 1.0);
        glCallList(gear1);
        glPopMatrix();

        //Second gear on counter shaft
        glPushMatrix();
        glTranslatef(-3.0, -2.0, -4.0);
        glRotatef(angle - 9.0, 0.0, 0.0, 1.0);
        glCallList(gear2);
        glPopMatrix();

        //Third gear on counter shaft
        glPushMatrix();
        glTranslatef(-3.0, -2.0, 1.2);
        glRotatef(angle - 9.0, 0.0, 0.0, 1.0);
        glCallList(gear8);
        glPopMatrix();

        //Fourth gear on counter shaft
        glPushMatrix();
        glTranslatef(-3.0, -2.0, 3.0);
        glRotatef(angle - 9.0, 0.0, 0.0, 1.0);
        glCallList(gear4);
        glPopMatrix();

        //Fifth gear on counter shaft
        glPushMatrix();
        glTranslatef(-3.0, -2.0, 8.0);
        glRotatef(angle, 0.0, 0.0, 1.0);
        glCallList(gear3);
        glPopMatrix();

        //First gear on output shaft
        glPushMatrix();
        glTranslatef(-3.0, 4.0, -4.0);
        glRotatef(-angle/2.0 - 9.0, 0.0, 0.0, 1.0);
        glCallList(gear5);
        glPopMatrix();

        //attach on first gear - output shaft
        glPushMatrix();
        glTranslatef(-3.0, 4.0, -3.2);
        glRotatef(-angle/2.0 - 9.0, 0.0, 0.0, 1.0);
        glCallList(attach);
        glPopMatrix();

        //attach between first & second gear - output shaft
        glPushMatrix();
        glTranslatef(-3.0, 4.0, -1.4);
        glRotatef(angle2, 0.0, 0.0, 1.0);
        glCallList(attach2);
        glPopMatrix();

       //attach before second gear - output shaft
        glPushMatrix();
        glTranslatef(-3.0, 4.0, 0.4);
        glRotatef(-angle/1.4 - 20.0, 0.0, 0.0, 1.0);
        glCallList(attach);
        glPopMatrix();

        //Second gear on output shaft
        glPushMatrix();
        glTranslatef(-3.0, 4.0, 1.2);
        glRotatef(-angle/1.4 - 20.0, 0.0, 0.0, 1.0);
        glCallList(gear7);
        glPopMatrix();

        //Third gear on output shaft
        glPushMatrix();
        glTranslatef(-3.0, 4.0, 3.0);
        glRotatef(-angle*1.4 - 20.0, 0.0, 0.0, 1.0);
        glCallList(gear9);
        glPopMatrix();

        //attach after third gear - output shaft
        glPushMatrix();
        glTranslatef(-3.0, 4.0, 3.8);
        glRotatef(-angle*1.4 - 20.0, 0.0, 0.0, 1.0);
        glCallList(attach);
        glPopMatrix();

        //attach between third & fourth gear - output shaft
        glPushMatrix();
        glTranslatef(-3.0, 4.0, 5.5);
        glRotatef(angle2, 0.0, 0.0, 1.0);
        glCallList(attach2);
        glPopMatrix();

        //attach before fourth gear - output shaft
        glPushMatrix();
        glTranslatef(-3.0, 4.0, 7.2);
        glRotatef(-angle*2.0 - 20.0, 0.0, 0.0, 1.0);
        glCallList(attach);
        glPopMatrix();

        //Fourth gear on output shaft
        glPushMatrix();
        glTranslatef(-3.0, 4.0, 8.0);
        glRotatef(-angle*2.0 - 20.0, 0.0, 0.0, 1.0);
        glCallList(gear10);
        glPopMatrix();

        //Input Shaft Gear
        glPushMatrix();
        glTranslatef(-3.0, 4.0, -9.0);
        glRotatef(-2.0*angle - 9.0, 0.0, 0.0, 1.0);
        glCallList(gear6);
        glPopMatrix();

        //Dog Clutches - 1
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color9);
        GLUquadric *clutch1 = gluNewQuadric();
        gluQuadricDrawStyle(clutch1, GLU_FILL);
        glTranslatef(clutchTransx1,clutchTransy1,clutchTransz1);
        glRotatef(angle2, 0.0, 0.0, 1.0);
        SOLID_CLOSED_CYLINDER(clutch1, 1.4f, 1.4f, 1.5f, 20, 20);
        gluDeleteQuadric(clutch1);
        glPopMatrix();

        //Dog Clutches - 2
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color9);
        GLUquadric *clutch2 = gluNewQuadric();
        gluQuadricDrawStyle(clutch2, GLU_FILL);
        glTranslatef(clutchTransx2,clutchTransy2,clutchTransz2);
        glRotatef(angle2, 0.0, 0.0, 1.0);
        SOLID_CLOSED_CYLINDER(clutch2, 1.4f, 1.4f, 1.5f, 20, 20);
        gluDeleteQuadric(clutch2);
        glPopMatrix();

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glColor3f(1.0,0.0,0.0);
        glLineWidth(5.0);

      //show direction of rotation
        glPushMatrix();
        glTranslatef(-3.0f,4.0f,-12.0f);
        glRotatef(-3.0*angle - 9.0, 0.0, 0.0, 1.0);
        newCircle(0, 0, 2);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-3.0f,-2.0f,-11.0f);
        glRotatef(2.0*angle, 0.0, 0.0, 1.0);
        newCircle(0, 0, 2);
        glPopMatrix();

        if(reached1 || reached2 || reached3 || reached4){
          glPushMatrix();
          glTranslatef(-3.0f,4.0f,11.0f);
          glRotatef(angle2, 0.0, 0.0, 1.0);
          newCircle(0, 0, 2);
          glPopMatrix();
        }

        //bitmap_output(1.0,10.0,"Jawad Ahmed (1PE13CS064)",GLUT_BITMAP_HELVETICA_18);

         glEnable(GL_LIGHTING);
         glEnable(GL_LIGHT0);

         glPopMatrix();
      }
      glutSwapBuffers();

      count++;
      if (count == limit) {
        exit(0);
      }
}

void display(void)
{
    draw();
}


static void reshape(int width, int height);

static void stabilize34()
{
    if(prevVal == 3 && clutchTransz2 < transcz2){
        clutchTransz2+=0.025;
        reached3 = 0;
    }
    if(prevVal==4 && clutchTransz2 > transcz2){
        clutchTransz2-=0.025;
        reached4 = 0;
    }
}

static void stabilize12()
{
    if(prevVal == 1 && clutchTransz1 < transcz1){
        clutchTransz1+=0.025;
        reached1 = 0;
    }
    if(prevVal==2 && clutchTransz1 > transcz1){
        clutchTransz1-=0.025;
        reached2 = 0;
    }
}

static void idle(void)
{
  angle += 2.0;

  if(gearPressed == 1){
    if(menuVal==1){//if first gear
        //stabilize other gears
        stabilize34();
        if(!reached1){
            clutchTransz1 -= 0.025;
            count12++;
        }
        else {
            angle2 -= 2.0;
        }
        if(prevVal==2){
            if(count12 == 64){
                reached1 = 1;
                prevVal = menuVal;
                count12 = 0;
                reached2 = 0;
            }
        }
        else{
            if(count12 == 32){
                reached1 = 1;
                prevVal = menuVal;
                count12 =0;
            }
        }
    }

    if(menuVal == 2){
        stabilize34();
        if(!reached2){
            clutchTransz1 += 0.025;
            count12++;
        }
        else {
            angle2 -= 4.0;
        }

        if(prevVal==1){
            if(count12 == 64){
                reached2 = 1;
                prevVal = menuVal;
                count12 = 0;
                reached1 = 0;
            }
        }
        else{
            if(count12 == 32){
                reached2 = 1;
                prevVal = menuVal;
                count12 = 0;
            }
        }
    }

    if(menuVal == 3){
        stabilize12();
        if(!reached3){
            clutchTransz2 -= 0.025;
            count34++;
        }
        else {
            angle2 -= 6.0;
        }

        if(prevVal==4){
            if(count34 == 64){
                reached3 = 1;
                prevVal = menuVal;
                count34 = 0;
                reached4 = 0;
            }
        }
        else{
            if(count34 == 32){
                reached3 = 1;
                prevVal = menuVal;
                count34 = 0;
            }
        }
    }

    if(menuVal == 4){
        stabilize12();
        if(!reached4){
            clutchTransz2 += 0.025;
            count34++;
        }
        else {
            angle2 -= 10.0;
        }

        if(prevVal==3){
            if(count34 == 64){
                reached4 = 1;
                prevVal = menuVal;
                count34 = 0;
                reached3 = 0;
            }
        }
        else{
            if(count34 == 32){
                reached4 = 1;
                prevVal = menuVal;
                count34 = 0;
            }
        }
    }

    //Neutral
    if(menuVal==5)
    {
        stabilize12();
        stabilize34();
    }

  }
  glutPostRedisplay();
}

/* change view angle, exit upon ESC */
/* ARGSUSED1 */
static void
key(unsigned char k, int x, int y)
{
  switch (k) {
  case 'z':
    view_rotz += 5.0;
    break;
  case 'Z':
    view_rotz -= 5.0;
    break;
  case 27:  /* Escape */
    exit(0);
    break;
    case 13:
        flag1=1;
        break;
  default:
    return;
  }
  glutPostRedisplay();
}

/* change view angle */
static void special(int k, int x, int y)
{
  switch (k) {
  case GLUT_KEY_UP:
    view_rotx += 5.0;
    break;
  case GLUT_KEY_DOWN:
    view_rotx -= 5.0;
    break;
  case GLUT_KEY_LEFT:
    view_roty += 5.0;
    break;
  case GLUT_KEY_RIGHT:
    view_roty -= 5.0;
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

/* new window size or exposure */
static void reshape(int width, int height)
{
  GLfloat h = (GLfloat) height / (GLfloat) width;

  glViewport(0, 0, (GLint) width, (GLint) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(31.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -50.0);
}

static void init(void)
{
  static GLfloat pos[4] = {-5.0, 5.0, 10.0, 0.0};
  GLfloat lightIntensity[] = {1.0f, 1.0f, 1.0f, 1.0f};

  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  //First gear on counter shaft
  gear1 = glGenLists(1);
  glNewList(gear1, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
  gear(1.0, 3.3, 1.5, 20, 0.7);
  glEndList();

  //Second gear on counter shaft
  gear2 = glGenLists(1);
  glNewList(gear2, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color5);
  gear(1.0, 2.0, 1.5, 20, 0.5);
  glEndList();

  //Third gear on counter shaft
  gear8 = glGenLists(1);
  glNewList(gear8, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color5);
  gear(1.0, 2.7, 1.5, 20, 0.5);
  glEndList();


  //Fourth gear on counter shaft
  gear4 = glGenLists(1);
  glNewList(gear4, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color5);
  gear(0.5, 3.3, 1.5, 20, 0.7);
  glEndList();

  //Fifth gear on counter shaft
  gear3 = glGenLists(1);
  glNewList(gear3, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color6);
  gear(0.5, 4.0, 1.5, 20, 0.7);
  glEndList();

  //First gear on output shaft
  gear5 = glGenLists(1);
  glNewList(gear5, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color5);
  gear(0.5, 4.0, 1.5, 20, 0.7);
  glEndList();

  //Second gear on output shaft
  gear7 = glGenLists(1);
  glNewList(gear7, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color5);
  gear(0.5, 3.3, 1.5, 20, 0.5);
  glEndList();

  //Third gear on output shaft
  gear9 = glGenLists(1);
  glNewList(gear9, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color5);
  gear(0.5, 2.7, 1.5, 20, 0.5);
  glEndList();

  //Fourth gear on output shaft
  gear10 = glGenLists(1);
  glNewList(gear10, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color5);
  gear(0.5, 2.0, 1.5, 15, 0.5);
  glEndList();

  //Input Shaft Gear
  gear6 = glGenLists(1);
  glNewList(gear6, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color7);
  gear(0.5, 2.4, 1.5, 15, 0.7);
  glEndList();

  //attach
  attach = glGenLists(1);
  glNewList(attach, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color7);
  gear(0.4, 1.0, 1.5, 30, 0.3);
  glEndList();

  //attach
  attach2 = glGenLists(1);
  glNewList(attach2, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color7);
  gear(0.4, 1.0, 1.0, 30, 0.3);
  glEndList();

  glEnable(GL_NORMALIZE);
}

void
visible(int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc(idle);
  else
    glutIdleFunc(NULL);
}

main(int argc, char *argv[])
{
  flag1=0;
  glutInit(&argc, argv);
  if (argc > 1) {
    /* do 'n' frames then exit */
    limit = atoi(argv[1]) + 1;
  } else {
    limit = 0;
  }
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(1152,864);
  glutInitWindowPosition(0,0);
  glutCreateWindow("SIMULATION OF VEHICLE GEAR TRANSMISSION ");
  createMenu();
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);
  glutVisibilityFunc(visible);
  glutMainLoop();
  return 0;
}
