#include <iostream>
#include <ctime>
#include <cstdlib>.
#define "crush.h"

using namespace std;


int main()
{
    srand(time(0));
    int fila=0, columna=0;



    cout << "ingrese el valor de la fifa: " << endl;
    cin>>fila;

    cout<< "ingresa el valor para las columnas "<<endl;
    cin>>columna;


    //funcion sebas

    int bytesactuales= calcularbytesnecesarios(fila, columna,);

    unsigned char* memoria= new unsigned char[bytesactuales]() ;



    for (int i = 0; i < fila ; ++i) {
        for (int c = 0; c < columna; ++c) {
            int fichaaleatoria= rand()%6;
            memoria= guardarficha(memoria, columna, i, c, fichaaleatoria);
        }


    }



//ensayo



    return 0;
}
