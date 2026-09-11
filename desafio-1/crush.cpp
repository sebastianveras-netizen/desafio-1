#include "crush.h"
#include <iostream>


using namespace std;



int calcularbytesnecesarios (int filas , int columnas) {
        int totalbits = filas * columnas * 3;
    return (totalbits + 7) /8;

unsigned char* guardarfichas ( unsigned char* memoria,int columnas, int filas,int columna, int valor){
valor =  valor & 7 ;
 int indiceficha = filas * columnas + columna;
int bitinicial = indiceficha * 3;
 int numerobytes = bitinicial/8;
int posicionenbyte = bitinicial % 8;

 if (posicionenbyte <= 5) {
    (memoria[numerobytes] &= ~(7 << posicionenbyte);
 }
    else if ( posicionenbyte == 6) {
    int parte1 = valor & 3;
    int parte2 = (valor >> 2) & 1;
        (memoria[numerobytes] &= ~(3 << 6);
        (memoria[numerobytes] |= (parte1 << 6);

        (memoria[numerobytes + 1] &= ~1;
        (memoria[numerobytes + 1] |= parte2;
    }
        else {
    int parte1 = valor & 1;
    int parte2 = (valor >> 1 ) & 3;
                int parte2 = (memoria [numerobytes + 1] & 3 )<< 1;
        return parte1 | parte2;



        }
}

}
