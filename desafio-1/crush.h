#ifndef CRUSH_H
#define CRUSH_H

#include <iostream>

int calcularbytesnecesarios (int filas , int columnas);
unsigned char* guardarficha ( unsigned char* memoria,int columnas, int fila,int columna, int valor);
int leerficha( unsigned char* memoria,int columnas, int fila,int columna);
#endif