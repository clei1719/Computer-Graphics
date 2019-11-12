/*
 *  finalProject
 *
 *  Demonstrates using lighting and textures.
 *
 *  Key bindings:
 *  l          Toggle lighting on/off
 *  t          Change textures
 *  m          Toggles texture mode modulate/replace
 *  a/A        decrease/increase ambient light
 *  d/D        decrease/increase diffuse light
 *  s/S        decrease/increase specular light
 *  e/E        decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  []         Lower/rise light
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
#include <time.h>

int mode=0;       //  Texture mode
int ntex=0;       //  Cube faces
int axes=1;       //  Display axes
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
int rep=1;        //  Repitition
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[9]; // Texture names
unsigned int flameEye[2]; // Texture names
unsigned int alienShip[2]; // Texture names
unsigned int houses[5]; // Texture names

struct RGB {
    unsigned char R;
    unsigned char G;
    unsigned char B;
};


GLfloat angleSpaceship = 0.0f;
GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat z = 0.0f;
GLfloat angleCube = 0.0f;

float velocity = 0.05f;

GLint r = 1;
GLint g = 0;
GLint b = 1;
GLint cycleTex = 0;

double t = 0;

int refreshMills = 15; // refresh interval in milliseconds

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 int randomNum()
 {
   int temp;
   temp = rand() % 256 + 1;
   return temp;
 }

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   //  Front
   glColor3f(1,0,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   glColor3f(0,0,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glColor3f(1,1,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[3]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glColor3f(0,1,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[4]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glColor3f(0,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glColor3f(1,0,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[6]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
static void cube2(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,houses[0]);
   //  Front
   //glColor3f(1,0,0);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   //glColor3f(0,0,1);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   //glColor3f(1,1,0);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   //glColor3f(0,1,0);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   //glColor3f(0,1,1);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   //glColor3f(1,0,1);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void pyramid(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
  //  Set specular color to white
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Transform cube
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[0]); //Binds Textures to object
   //Draw Base of Pyramid
   // clockwise to put Normal pointing down or out.
   glBegin(GL_QUADS);
   //glColor3f(1,0,0);
   //glColor3f(1,0,1);
   glNormal3f( 0, -1, 0);
   glTexCoord2f(0, 0); glVertex3f(-1,0,+1); // 0
   glTexCoord2f(1, 0); glVertex3f(+1,0,-1); // 1
   glTexCoord2f(1, 1); glVertex3f(+1,0,-1); // 2
   glTexCoord2f(0, 1); glVertex3f(+1,0,+1); // 3
   glEnd();

   //  pyramid
   // counterclockwise for normals pointing out.
   glBegin(GL_TRIANGLES);
   //  Front
   //glColor3f(1,0,0);
   glNormal3f(0, 1/sqrt(2), 1/sqrt(2));
   glTexCoord2f(0, 0); glVertex3f(-1,0,+1);// 0
   glTexCoord2f(1, 0); glVertex3f(+1,0,+1); // 1
   glTexCoord2f(1, 0.5); glVertex3f(0,+1,0); // 2
   //  Back
   //glColor3f(0,1,0);
   glNormal3f(0,1/sqrt(2),-1/sqrt(2));
   glTexCoord2f(0, 0); glVertex3f(+1,0,-1); // 0
   glTexCoord2f(1, 0); glVertex3f(-1,0,-1); // 1
   glTexCoord2f(1, 0.5); glVertex3f(0,+1,0); // 2
   //  Right
   //glColor3f(1,1,0);
   glNormal3f(1/sqrt(2), 1/sqrt(2), 0);
   glTexCoord2f(0, 0); glVertex3f(+1,0,+1); // 0
   glTexCoord2f(1, 0); glVertex3f(+1,0,-1); // 1
   glTexCoord2f(1, 0.5); glVertex3f(0,+1,0); // 2
   //  Left
   //glColor3f(0,1,0);
   glNormal3f(-1/sqrt(2), 1/sqrt(2), 0);
   glTexCoord2f(0, 0); glVertex3f(-1,0,-1); // 0
   glTexCoord2f(1, 0); glVertex3f(-1,0,+1); // 1
   glTexCoord2f(1, 0.5); glVertex3f(0,+1,0); // 2
   //  End
   glEnd();

   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
static void ground(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{

  //  Set specular color to white
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Transform cube
  glTranslated(x,y,z);
  glRotated(th,0,0,0);
  glScaled(dx,dy,dz);


  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

  glBindTexture(GL_TEXTURE_2D,texture[8]); //Binds Textures to object

  glBegin(GL_QUADS);
  //glColor3f(0,0,0);
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0, 0); glVertex3f(-10,10,0); // 0
  glTexCoord2f(1, 0); glVertex3f(10,10,0); // 1
  glTexCoord2f(1, 1); glVertex3f(10,-10,0); // 2
  glTexCoord2f(0, 1); glVertex3f(-10,-10,0); // 3
  glEnd();
  glPopMatrix();
}
static void road(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{

  //  Set specular color to white
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  //  Save transformation
  glPushMatrix();
  //  Transform cube
  glTranslated(x,y,z);
  glRotated(th,0,0,0);
  glScaled(dx,dy,dz);


  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

  glBindTexture(GL_TEXTURE_2D,texture[7]); //Binds Textures to object

  glBegin(GL_QUADS);
  //glColor3f(0,0,0);
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0, 0); glVertex3f(-10,0,0); // 0
  glTexCoord2f(1, 0); glVertex3f(10,0,0); // 1
  glTexCoord2f(1, 1); glVertex3f(10,-1,0); // 2
  glTexCoord2f(0, 1); glVertex3f(-10,-1,0); // 3
  glEnd();
  glPopMatrix();
}
static void door(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
  //  Save transformation
  glPushMatrix();
  //  Transform cube
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  glBegin(GL_QUADS);
  glColor3f(0,0,0);
  glNormal3f( 0, 0, 1);
  glVertex3f(.5,0,0); // 0
  glVertex3f(.6,0,0); // 1
  glVertex3f(.6,.5,0); // 2
  glVertex3f(.5,.5,0); // 3
  glEnd();
  glPopMatrix();
}
static void windows(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
  //  Save transformation
  glPushMatrix();
  //  Transform cube
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  glBegin(GL_QUADS);
  glColor3ub(121, 209, 231);
  glNormal3f( 0, 0, 1);
  glVertex3f(.5,0,0); // 0
  glVertex3f(.6,0,0); // 1
  glVertex3f(.6,.5,0); // 2
  glVertex3f(.5,.5,0); // 3
  glEnd();
  glPopMatrix();
}
static void tallHouse(double x, double y, double z, double r)
{

  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(r,0,0,0);
  glScaled(.35,.15,.35);
  glColor3f(0.52,0.37,0.26);
  pyramid(0,2.5,0  , 1,2,1 , 0);
  glPopMatrix();

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(r,0,0,0);
   glScaled(.3,.2,.3);
   //glColor3f(0.329412,0.329412,0.329412);
   glColor3f(0,0,.61);
   cube2(0,-1,0 , 1,3,1 , 0);
   glPopMatrix();
   //door
   glPushMatrix();
   glColor3f(0,0,0);
   door(.4,-.08,.22 , 1,.3,1 , 0);
   glPopMatrix();
   //window left 1
   glPushMatrix();
   windows(.5,.5,.22 , .7,.2,0 , 0);
   glPopMatrix();

   //window left 2
   glPushMatrix();
   windows(.5,.3,.22 , .7,.2,0 , 0);
   glPopMatrix();

   //window left 3
   glPushMatrix();
   windows(.5,.1,.22 , .7,.2,0 , 0);
   glPopMatrix();

   //window right 1
   glPushMatrix();
   windows(.73,.5,.22 , .7,.2,0 , 0);
   glPopMatrix();

   //window right 2
   glPushMatrix();
   windows(.73,.3,.22 , .7,.2,0 , 0);
   glPopMatrix();

   //window right 3
   glPushMatrix();
   windows(.73,.1,.22 , .7,.2,0 , 0);
   glPopMatrix();
}
static void house(double x, double y, double z, double r)
{

  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(r,0,0,0);
  glScaled(.35,.15,.35);
  glColor3f(0.52,0.37,0.26);
  pyramid(0,0,0  , 1,1,1 , 0);
  glPopMatrix();

   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(r,0,0,0);
   glScaled(.3,.2,.3);
   glColor3f(0.329412,0.329412,0.329412);
   cube2(0,-1,0 , 1,1,1 , 0);
   glPopMatrix();
   //door
   glPushMatrix();
   door(-.55,.22,.22 , 1,.3,1 , 0);
   glPopMatrix();
   //window 1
   glPushMatrix();
   windows(-.25,.35,.22 , .7,.2,0 , 0);
   glPopMatrix();
   //window 2
   glPushMatrix();
   windows(-.53,.35,.22 , .7,.2,0 , 0);
   glPopMatrix();

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}
static void Vertex1(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glColor3f(1,1,1);
   glTexCoord2d(2*(th/360.0),2*(ph/180.0+0.5));
   glVertex3d(x,y,z);
}
static void Vertex2(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glColor3f(1,1,1);
   glTexCoord2d(th/360.0,ph/180.0+0.5);
   glVertex3d(x,y,z);
}
static void ball(double x,double y,double z,double r) // with texture
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};

   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   //glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE); // this is optional...
   int delay = floor((int) t/5); // every 5 seconds flip texture
   glBindTexture(GL_TEXTURE_2D,alienShip[delay%2]); //Binds Textures to object
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex1(th,ph);
         Vertex1(th,ph+inc);
      }
      glEnd();
   }
   glDisable(GL_TEXTURE_2D);
   //  Undo transofrmations
   glPopMatrix();
}
static void ball2(double x,double y,double z,double r) // without texture
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};

   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   //glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);


   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}
static void sphere2(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE); // this is optional...
   int delay = floor((int) t/5); // every 5 seconds flip texture
   //glBindTexture(GL_TEXTURE_2D,texture[delay%5]); //Binds Textures to object

   glBindTexture(GL_TEXTURE_2D,flameEye[delay%2]); //Binds Textures to object
   cycleTex += 1;
   if(cycleTex == 6)
   {
    cycleTex = 0;
   }
   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=20;th<=360;th+=d)
      {
         Vertex2(th,ph);
         Vertex2(th,ph+d);
      }
      glEnd();
   }
   glDisable(GL_TEXTURE_2D);
   //  Undo transformations
   glPopMatrix();
}

static void insideSphere2(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      glColor3ub(th+1,th*3,th+ph); //gives the gradient in the eye.
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }
   //  Undo transformations
   glPopMatrix();
}
 static void eyeOfSoloman()
 {
   //Moon or Sun
   glPushMatrix();
   glTranslated(.5,1,-1);
   glRotated(0,0,0,0);
   glScaled(.3,.3,.3);
   glColor3f(sin(1),sin(1),0);
   glColor3ub(252, 100, 35);
   sphere2(3,3,3,2);
   glPopMatrix();

   //Inside Moon or Sun
   glPushMatrix();
   glTranslated(.5,1,-1);
   glRotated(0,0,0,0);
   glScaled(.3,.3,.3);
   //glColor3f(0,0,1);
   insideSphere2(3,3,3,2);
   glPopMatrix();
 }

   static void spaceship()//(double x,double y, double z, double height, double width, double depth)
   {
     glPushMatrix();
     //Body
     glPushMatrix();
     glTranslated(0,.838,0);
     glRotatef(45, 0, 0, 0);
     glScaled(.25,.05,.25);
     ball2(1,1,1,2);
     glPopMatrix();

     //Bottom of Ship
     glPushMatrix();
     glTranslated(0.06,.8,0.06);
     glRotated(45,0,0,0);
     glScaled(.17,.05,.17);
     glColor3f(0,0,1);
     ball(1,1,1,2);
     glPopMatrix();

     //Top of Ship
     glPushMatrix();
     glTranslated(0.06,.876,0.06);
     glRotated(45,0,0,0);
     glScaled(.17,.05,.17);
     glColor3f(0,0,1);
     ball(1,1,1,2);
     glPopMatrix();


     glPopMatrix();
   }
   void static mothership()
   {
     unsigned int delay = floor(t/.045);
     unsigned int delay2 = floor(t * 2);
     glTranslated(x,y,z);
     printf("Z: %f\n",z);
     glRotatef(angleSpaceship, 0, 1, 0);
     glColor3ub(r,g,b);
     spaceship();
     angleSpaceship += 1.6f; // speed of spin
     z += velocity; // 0.0003f // movement foward..need this to be modified...so I can control it.

     //This loops the spaceship when it goes to far away and restarts the scene.
     if(z > 10)
     {
       z = -5;
     }
     else if(z < 4.0)
     {
       // calulate velocity v^2 = v_0^2 * 2a * Changein(x) // for neg accel
       velocity += ((pow(0, 2) - pow(0.05,2)) / (2 * (4.0 - -5) ) ); // accel is .003f
     }
     // slows spaceship down by decrementing velocity within the z range translation.
     //Essentially slows ship down where we want it.
     else if(z > 4.0  && z < 4.1)
     {
      velocity = 0;
      velocity += 0.0007;
     }
     else if(z > 4.1)
     {
       // calulate velocity v^2 = v_0^2 * 2a * Changein(x) // for pos accel
       velocity += ((pow(0.05, 2) - pow(0,2)) / (2 * (10 - 4.1) ) ); // accel is .003f
     }

     srand(delay);
     r = rand() % 215 + 255;
     g = 0;
     b = rand() % 70 + 255;
   }

   void static eye()
   {
     //eye of soloman
     glPushMatrix();
     glTranslated(0,2,-3);
     glRotated(-270,1,0,0);
     eyeOfSoloman();
     glPopMatrix();
   }
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Length of axes
   const double len=2.0;
   //  Eye position
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Set perspective
   glLoadIdentity();
   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //  Draw scene
   //cube(0,0,0 , 0.5,0.5,0.5 , 0);
   //eye of soloman
   glPushMatrix();
   glTranslated(1,2,-3);
   glRotated(-30,0,1,0);
   eye();
   glPopMatrix();

   //house
   glPushMatrix();
   glTranslated(0,0,0);
   house(0, .5, 0, 45);
   glPopMatrix();

   //house 4
   glPushMatrix();
   glTranslated(0,0,-1);
   house(0, .5, 0, 90);
   glPopMatrix();

   //house2
   glPushMatrix();
   glTranslated(-1,0,0);
   house(0, .5, 0, 45);
   glPopMatrix();

   //tall House 1
   glPushMatrix();
   glTranslated(0,.3,0);
   tallHouse(1, .48, 0, 45);
   glPopMatrix();

   //house3
   glPushMatrix();
   glTranslated(-1.5,0,0);
   house(0, .5, 0, 45);
   glPopMatrix();

   //tall House
   glPushMatrix();
   glTranslated(-.5,.3,0);
   tallHouse(1, .48, 0, 45);
   glPopMatrix();



   glPushMatrix();
   glTranslated(0,1.2,0);
   glRotated(90,1,0,0);
   //glColor3f(0,0,1);
   ground(1,0,1 , 1,1,1 , 0);
   glPopMatrix();

   glPushMatrix();
   glTranslated(0,1.2005,1.3);
   glRotated(90,1,0,0);
   //glColor3f(0,0,1);
   road(1,0,1 , 1,1,1 , 0);
   glPopMatrix();



   //spaceship
   glPushMatrix();
   //glTranslated(-.5,1,1); //original working Location
   glTranslated(-.5,1,-20);
   //glRotated(45,0,0,0);
   glScaled(5,1.5,5);
   //glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   mothership();//(x, y, z, height, width, depth);
   glPopMatrix();









   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Light=%s Texture=%s",th,ph,dim,light?"On":"Off",mode?"Replace":"Modulate");
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

 /*
  *  GLUT calls this routine when an arrow key is pressed
  */
 void special(int key,int x,int y)
 {
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
       th += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
       th -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
       ph += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
       ph -= 5;
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_DOWN)
       dim += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_UP && dim>1)
       dim -= 0.1;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Update projection
    Project(45,asp,dim);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
 }

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle texture mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle textures mode
   else if (ch == 't')
      ntex = 1-ntex;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Repitition
   else if (ch=='+')
      rep++;
   else if (ch=='-' && rep>1)
      rep--;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(45,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("finalProject");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures
   texture[0] = LoadTexBMP("crate.bmp");
   texture[1] = LoadTexBMP("img1.bmp");
   texture[2] = LoadTexBMP("img2.bmp");
   texture[3] = LoadTexBMP("img3.bmp");
   texture[4] = LoadTexBMP("img4.bmp");
   texture[5] = LoadTexBMP("img5.bmp");
   texture[6] = LoadTexBMP("img6.bmp");
   texture[7] = LoadTexBMP("twobytwoRoad.bmp");
   texture[8] = LoadTexBMP("Grass.bmp");

   flameEye[0] = LoadTexBMP("flame.bmp");
   flameEye[1] = LoadTexBMP("flame3.bmp");

   alienShip[0] = LoadTexBMP("alienmetal.bmp");
   alienShip[1] = LoadTexBMP("steelcirclebrushed.bmp");

   houses[0] = LoadTexBMP("brick.bmp");
   houses[1] = LoadTexBMP("brick.bmp");
   houses[2] = LoadTexBMP("brick.bmp");
   houses[3] = LoadTexBMP("brick.bmp");
   houses[4] = LoadTexBMP("brick.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
