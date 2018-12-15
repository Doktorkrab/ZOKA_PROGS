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

byte *Field1, *Field2; /* Здесь храним два поля: новое, которое создадим и старое, которое вывводим на экран */
const int W = FRAME_W, H = FRAME_H; /* W - Ширина поля, H - высота, изначально равны размерам Фрейма */

int GetCell( byte *F, int X, int Y ) /* Функция, выводящая значение в клетке X, Y */
{
  X = (X + W) % W; /* Если мы вышли за границы, вернемся с другой стороны */
  Y = (Y + H) % H;
  return F[Y * W + X];
}


void SetCell( byte *F, int X, int Y, int Value ) /* Делаем значение в клетке X, Y равное Value */
{
  F[Y * W + X] = Value;
}

void FieldInit( byte *F ) /* Первичная инициализация поля. 10000 раз сделаем случайную клетку живой */
{
  int n = 10000;
  memset(F, 0x00, W * H);
  for (; n >= 0; n--)
    SetCell(F, rand() % W, rand() % H, 1);
}

void PutPixel( int X, int Y, int Value ) /* Красит в клетку X, Y в цвет (Value, Value, Value) */
{
  if (X < 0 || Y < 0 || X >= FRAME_H || Y >= FRAME_W) /* Если не в границах полях, то не будем красить */
    return;
  Frame[Y][X][0] = Value;
  Frame[Y][X][1] = Value;
  Frame[Y][X][2] = Value;
}

void FieldDraw( byte *F ) /* Рисуем поле на экран */
{
  int Y = 0;
  int X = 0;
  for (Y = 0; Y < H; Y++) /* Сначала закрасим всё после в черный цвет */
    for (X = 0; X < W; X++)
      PutPixel(X, Y, 0);

  for (Y = 0; Y < H; Y++)
    for (X = 0; X < W; X++)
      if (GetCell(F, X, Y))
        PutPixel(X, Y, 255); /* Все живые клетки покрасим в белый цвет */
}

int GetNeighbours( byte *F, int X, int Y ) /* Проверяем всех соседей клетки X, Y */
{
  int sum = 0, shiftX, shiftY;
  for (shiftY = -1; shiftY <= 1; shiftY++)
    for (shiftX = -1; shiftX <= 1; shiftX++)
    {
      if (shiftX == 0 && shiftY == 0) /* Саму клетку не проверяем */
        continue;
      sum += GetCell(F, X + shiftX, Y + shiftY);
    }
  return sum;
}

void NewGeneration( byte *F1, byte *F2 ) /* Делаем новое поколение клетки */
{
  int n, value, X, Y;

  for (Y = 0; Y < H; Y++)
    for (X = 0; X < W; X++)
    {
      n = GetNeighbours(F1, X, Y); /* Для каждой клетки находим кол-во живых соседей */
      if ((value = GetCell(F1, X, Y)) == 1) /* Если клетка живая */
      {
        if (n < 2 || n > 3) /* Какие-то правила, которые нам давали */
          value = 0;
      }
      else if (n == 3) /* Какие-то правила, которые нам давали */
        value = 1;
      SetCell(F2, X, Y, value); /* Красим клетку */
    }
}

void Display( void )
{
  byte *F3; /* Переменная, через которую будем менять местами поля */
  glClearColor(0.3, 0.5, 0.7, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  FieldDraw(Field1); /* Рисуем нынешнее поле */
  NewGeneration(Field1, Field2); /* Строим новое поле */
  F3 = Field1;
  Field1 = Field2;
  Field2 = F3; /* Меняем поля местами */

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



