#include "crush.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
int main()
{
    srand(time(NULL));

    int calcularbytesnecesarios (int filas , int columnas);
        int totalbits = filas * columnas * 3;
    return (totalbits + 7) /8;

        int bitsactuales = calcularbytesnecesarios (filas ,columnas);
unsigned char* guardarfichas = ( unsigned char* memoria,int columnas, int filas,int columna, int valor);
valor =  valor & 7 ;
 int indiceficha = fila* columnas + columna;
int bitinicial = indiceficha * 3;
 int numerobytes = bitinicial/8;
int posicionenbyte = bitinicial % 8;

 if (posicionenbyte <= 5); {
     return (memoria[numerobytes] >> posicionenbyte) & 7;

     else if ( posicionenbyte == 6) {
         int parte1 = memoria [numerobytes]>> 6 ) & 3



     }


 }



}
