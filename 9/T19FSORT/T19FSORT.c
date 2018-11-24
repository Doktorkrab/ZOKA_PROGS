#include <stdio.h>
#include <stdlib.h>

void Swap( int *A, int *B )
{
    int tmp = *A;

    *A = *B;
    *B = tmp;
}

void QuickSort( int *mas, int size )
{
    int x = mas[size / 2];
    int b, e;

    if (size < 2)
        return;

    b = 0;
    e = size - 1;

    while (b <= e) {
        while (mas[b] < x)
            b++;
        while (mas[e] > x)
            e--;
        if (b <= e) {
            if (b < e)
                Swap(&mas[b], &mas[e]);
            b++;
            e--;
        }
    }

    QuickSort(mas, e + 1);
    QuickSort(mas + b, size - b);

}

int Count( void )
{
    int x, cnt = 0;
    FILE *f = fopen("IN.txt", "r");

    if (f == NULL)
        return 0;
    while (fscanf(f, "%d", &x) == 1)
        cnt++;

    fclose(f);
    return cnt;
}

void ReadMas( int *mas, int size )
{
    int i;
    FILE *f = fopen("IN.txt", "r");

    if (f == NULL)
        return;
    for (i = 0; i < size; i++)
        fscanf(f, "%i", &mas[i]);

    fclose(f);
}

void WriteMas( int *mas, int size )
{
    int i;
    FILE *f = fopen("RES.txt", "w");

    if (f == NULL)
        return;
    for (i = 0; i < size; i++)
        fprintf(f, "%i%c", mas[i], " \n"[i == size - 1]);

    fclose(f);
}

void main( void )
{
    int *mas, n = Count();

    mas = malloc(sizeof(int) * n);
    ReadMas(mas, n);
    QuickSort(mas, n);
    WriteMas(mas, n);

    free(mas);
}