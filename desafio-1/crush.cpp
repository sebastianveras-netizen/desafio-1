#include "crush.h"
#include <iostream>


using namespace std;


<<<<<<< HEAD
=======
        }
}

>>>>>>> 5ce510a3ee520250e04c4f2e7545dd9bdda0af46
int calcularbytesnecesarios (int filas , int columnas) {
    int totalbits = filas * columnas * 3;
    return (totalbits + 7) /8;
}
unsigned char* guardarficha ( unsigned char* memoria,int columnas, int fila,int columna, int valor){
    valor =  valor & 7 ;
    int indiceficha = fila * columnas + columna;
    int bitinicial = indiceficha * 3;
    int numerobytes = bitinicial/8;
    int posicionenbyte = bitinicial % 8;

    if (posicionenbyte <= 5) {
        memoria[numerobytes] &= ~(7 << posicionenbyte);
        memoria[numerobytes] |= (valor << posicionenbyte);
    }
    else if (posicionenbyte == 6) {
        int parte1 = valor & 3;
        int parte2 = (valor >> 2) & 1;
        memoria[numerobytes] &= ~(3 << 6);
        memoria[numerobytes] |= (parte1 << 6);

        memoria[numerobytes + 1] &= ~1;
        memoria[numerobytes + 1] |= parte2;
    }
    else {
        int parte1 = valor & 1;
        int parte2 = (valor >> 1) & 3;

        memoria[numerobytes] &= ~(1 << 7);
        memoria[numerobytes] |= (parte1 << 7);

        memoria[numerobytes + 1] &= ~3;
        memoria[numerobytes + 1] |= parte2;


    }
    return memoria;
}

int leerficha( unsigned char* memoria,int columnas, int fila,int columna) {

    int indiceficha = fila*columnas+columna;
    int bitinicial = indiceficha*3;
    int numerobytes = bitinicial/8;
    int posicionenbyte = bitinicial % 8;


    if (posicionenbyte <=5){
        return (memoria[numerobytes]>> posicionenbyte) & 7;
         }

    else if (posicionenbyte == 6 ){
        int parte1=( memoria [numerobytes] >> 6)&3;
        int parte2 = (memoria [numerobytes + 1] & 1)<<2;
        return parte1|parte2;

 }
         else{
    int parte1 = (memoria [numerobytes]>>7)&1;
    int parte2 = (memoria[numerobytes + 1] & 3)<<1;
    return parte1|parte2;

 }

}
char obtenercaracter(int valor){
    int codigo = valor & 7;
    if(codigo <=5 ){
    char caracter = 'A' + codigo;
        return caracter;
    }
    return '$' ;


}






























