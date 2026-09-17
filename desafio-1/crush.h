#ifndef CRUSH_H
#define CRUSH_H

#include <iostream>

int calcularbytesnecesarios (int filas , int columnas);
unsigned char* guardarficha ( unsigned char* memoria,int columnas, int fila,int columna, int valor);
int leerfichas( unsigned char* memoria,int columnas, int fila,int columna);
char obtenercaracter(int valor);
unsigned char* imprimirtablerofichas ( unsigned char* memoria, int filas,int columnas);
unsigned char* imprimirtablerobinario ( unsigned char* memoria, int filas,int columnas);
unsigned char* redimencionar ( unsigned char* memoria,int filas_actuales, int columnas_actuales,int filas_nuevas, int columnas_nuevas,int eliminaciones);
#endif