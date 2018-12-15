#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#define FRAME_W 256
#define FRAME_H 256
typedef unsigned char byte;

byte Frame[FRAME_H][FRAME_W][3];
double Zoom = 3;

int GetCell( byte *F, int X, int Y );
int GetNeighbours( byte *F, int X, int Y );
void FieldInit( byte *F );
void FieldDraw( byte *F );
void NewGeneration( byte *F );
void SetCell( byte *F, int X, int Y, int Value );
void PutPixel( int X, int Y, byte R, byte G, byte B );

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
  {
    free(Field1);
    free(Field2);
    exit(0);
  }
  if (Key == 'a')
  {
    X /= Zoom;
    Y /= Zoom;
    PutPixel(X, Y, 11, 102, 135);
  }
  if (Key == 'r')
    FieldInit(Field1);
}

int main( int argc, char *argv[] )
{
  Field1 = malloc(W * H);
  Field2 = malloc(W * H);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);

  glutCreateWindow("Я НЕ СПИСЫВАЛ ЭТУ ПРОГУ");

  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);

  FieldInit(Field1);
  glutMainLoop();
  return 0;
}

void PutPixel( int X, int Y, byte R, byte G, byte B )
{
  if (X < 0 || Y < 0 || X >= FRAME_H || Y >= FRAME_W)
    return;
  Frame[Y][X][0] = B;
  Frame[Y][X][1] = G;
  Frame[Y][X][2] = R;
}

int GetCell( byte *F, int X, int Y )
{
  X = (X + W) % W;
  Y = (Y + H) % H;
  return F[Y * W + X];
}

void SetCell( byte *F, int X, int Y, byte Value )
{
  F[Y * W + X] = Value;
}

void FieldInit( byte *F )
{
  int n = 100;

  for (; n >= 0; n--)
    SetCell(F, rand() % W, rand() % H, 1);
}


