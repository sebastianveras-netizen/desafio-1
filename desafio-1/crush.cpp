#include "crush.h"
#include <iostream>


using namespace std;


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

int leerfichas( unsigned char* memoria,int columnas, int fila,int columna) {

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

unsigned char* imprimirtablerofichas ( unsigned char* memoria, int filas,int columnas) {

    cout << "TABLERO" << endl;
    for (int f=0;f<filas; ++f) {
            cout << "FILA"<< f << " " << endl;
    for (int c=0;c<columnas;++c){
            int valor = leerfichas(memoria,columnas,f,c);
            cout << obtenercaracter(valor) << " " ;

        }
    cout << endl;
        }
    return memoria;

}

    unsigned char* imprimirtablerobinario ( unsigned char* memoria, int filas,int columnas) {
        int totalbytes = calcularbytesnecesarios(filas,columnas);
        cout << "ram en binario memoria fisica" << endl;
        for (int b=0;b<totalbytes; ++b) {
            cout << "byte"<< b << " : " << endl;
            for (int bit = 7 ; bit >= 0; --bit ){
                cout <<((memoria[b]>> bit) & 1);


            }
            cout << endl;

    }
return memoria;




}































