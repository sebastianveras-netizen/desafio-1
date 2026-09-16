#include <iostream>
#include <ctime>
#include <cstdlib>
#include "crush.h"

using namespace std;

using namespace std;
int main()
{
    srand(time(0));
    int fila=0, columna=0, seleccion_fila=0, seleccion_columna=0,eliminacionesusuario = 0;



    cout << "ingrese el valor de la fila: " << endl;
    cin>>fila;


    cout<< "ingresa el valor para las columnas "<<endl;
    cin>>columna;


    //funcion sebas

    int bytesactuales= calcularbytesnecesarios(fila, columna);

    unsigned char* memoria= new unsigned char[bytesactuales]() ;



    for (int i = 0; i < fila ; ++i) {
        for (int c = 0; c < columna; ++c) {
            int fichaaleatoria= rand()%6;
            memoria= guardarficha(memoria, columna, i, c, fichaaleatoria);
        }


    }
// aqui inician las opciones

    int opcion= -1;
    while (opcion!=0){
        cout<<"elije una de las siguientes opciones"<<endl;


         cout<<"1. ver tablero"<<endl<<"2. memoria"<<endl<<"3. eliminar casilla "<<endl<<"4. modifical el tablero"<<"0. salir"<<endl;


        cin>>opcion;
         cout<<"la opcion elijida fue: "<<opcion<<endl;


         if (opcion ==1){
             memoria =imprimirtablerofichas(memoria,fila, columna);

         }
         else if(opcion ==2){
             memoria =imprimirtablerobinario(memoria,fila,columna);
         }


         else if(opcion==3){

             cout<<"coloca la fila: "<<endl;
                     cin>>seleccion_fila;

             cout<<"coloca la columna: "<<endl;
                     cin>>seleccion_columna;


                     if (seleccion_fila>=0 && seleccion_fila < fila && seleccion_columna>=0 &&seleccion_columna < columna ){

                         memoria= guardarficha(memoria, columna, seleccion_fila, seleccion_columna, 6);
                         eliminacionesusuario++;
                         cout<<"se elimino la ficha";

                     }else{
                         cout<<"no se encuentra la posicion";
                     }



         }


    }


    return 0;
}

