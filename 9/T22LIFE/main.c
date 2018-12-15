#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>

#define FRAME_W 255
#define FRAME_H 255
typedef unsigned char byte;

byte Frame[FRAME_H][FRAME_W][3];
double Zoom = 2;

int GetCell( byte *F, int X, int Y );
int GetNeighbours( byte *F, int X, int Y );
void FieldInit( byte *F );
void FieldDraw( byte *F );
void NewGeneration( byte *F1, byte *F2 );
void SetCell( byte *F, int X, int Y, byte Value );
void PutPixel( int X, int Y, byte R, byte G, byte B );

byte *Field1, *Field2;
const int W = FRAME_W, H = FRAME_H;
int paused = 0;

void Display( void )
{
  byte *F3;
  glClearColor(0.3, 0.5, 0.7, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  FieldDraw(Field1);
  if (!paused)
  {
    NewGeneration(Field1, Field2);
    F3 = Field1;
    Field1 = Field2;
    Field2 = F3;
  }

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
    SetCell(Field1, X, Y, 1);
  }
  if (Key == 'r')
    FieldInit(Field1);
  if (Key == 'p')
    paused ^= 1;
}

int main( int argc, char *argv[] )
{
  srand(time(0));
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
  int n = 10000;
  memset(F, 0x00, W * H);
  for (; n >= 0; n--)
    SetCell(F, rand() % W, rand() % H, 1);
}

void FieldDraw( byte *F )
{
  int Y = 0;
  int X = 0;
  for (Y = 0; Y < H; Y++)
    for (X = 0; X < W; X++)
      PutPixel(X, Y, 0, 0, 0);

  for (Y = 0; Y < H; Y++)
    for (X = 0; X < W; X++)
      if (GetCell(F, X, Y))
        PutPixel(X, Y, 0, 255, 0);
}

int GetNeighbours( byte *F, int X, int Y )
{
  int sum = 0, shiftX, shiftY;
  for (shiftY = -1; shiftY <= 1; shiftY++)
    for (shiftX = -1; shiftX <= 1; shiftX++)
    {
      if (shiftX == 0 && shiftY == 0)
        continue;
      sum += GetCell(F, X + shiftX, Y + shiftY);
    }
  return sum;
}

void NewGeneration( byte *F1, byte *F2 )
{
  int n, value, X, Y;

  for (Y = 0; Y < H; Y++)
    for (X = 0; X < W; X++)
    {
      n = GetNeighbours(F1, X, Y);
      if ((value = GetCell(F1, X, Y)) == 1)
      {
        if (n < 2 || n > 3)
          value = 0;
      }
      else if (n == 3)
        value = 1;
      SetCell(F2, X, Y, value);
    }
}