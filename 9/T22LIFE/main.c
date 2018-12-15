#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#define FRAME_W 256
#define FRAME_H 256
typedef unsigned char byte;

byte Frame[FRAME_H][FRAME_W][3];
double Zoom = 3;

void FieldInit( byte *F );
void FieldDraw( byte *F );
void NewGeneration( byte *F );
void SetCell( byte *F, int X, int Y, int Value );
void GetCell( byte *F, int X, int Y );
void GetNeighbours( byte *F, int X, int Y );

byte *Field1, *Field2;
const int W = FRAME_W, H = FRAME_H;


void Display( void )
{
  glClearColor(0.3, 0.5, 0.7, 1);
  glClear(GL_COLOR_BUFFER_BIT);


  glRasterPos2d(-1, 1);
  glPixelZoom(Zoom, -Zoom);
  glDrawPixels(FRAME_W, FRAME_H, GL_BGR_EXT, GL_UNSIGNED_BYTE, Frame);

  glFinish();
  glutSwapBuffers();
  glutPostRedisplay();
}

void Keyboard( byte Key, int X, int Y )
{
  if (Key == 27)
    exit(0);
  if (Key == 'a') {
    X /= Zoom;
    Y /= Zoom;
    if (X >= 0 && Y >= 0 && X < FRAME_H && Y < FRAME_W) {
      Frame[Y][X][0] = 255;
      Frame[Y][X][1] = 255;
      Frame[Y][X][2] = 255;
    }
  }
}

int main( int argc, char *argv[] )
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);

  glutCreateWindow("Я НЕ СПИСЫВАЛ ЭТУ ПРОГУ");

  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);

  glutMainLoop();
  return 0;
}