#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void main( void )
{
    srand(time(0));
    int n = rand() % 10, i;
    FILE* f = fopen("IN.txt", "w");
    for (i = 0; i < n; i++) fprintf(f, "%d%c", rand() % 1000, " \n"[i == n - 1]);

    fclose(f);

}