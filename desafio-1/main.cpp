#include <iostream>
#include <ctime>
#include <cstdlib>
#include "crush.h"

using namespace std;


int main()
{
    srand(time(0));


    int fila=0, columna=0, seleccion_fila=0, seleccion_columna=0, eliminacionesusuario=0, nuevosbytes=0;  //declaramos las variables a utilizar




    cout << "ingrese el valor de la fila: " << endl;
    cin>>fila;


    cout<< "ingresa el valor para las columnas "<<endl;
    cin>>columna;




    int bytesactuales= calcularbytesnecesarios(fila, columna); //lo datos suministrados van para funcion calcularbytesnecesarios

    unsigned char* memoria= new unsigned char[bytesactuales]();
    //declaramos un puntero y reservamos espacio en memoria dinamico en el heap en donde vamos a guardar los valores de bytesactuales




    for (int i = 0; i < fila ; ++i) {//recorre en cada interacion las filas
        for (int c = 0; c < columna; ++c) {//recorre en cada interacion las columnas
            int fichaaleatoria= rand()%6; //aqui se generara un numero aleatorio entre 0 y 5 para luego poder definir la ficha
            memoria= guardarficha(memoria, columna, i, c, fichaaleatoria);// se llama funcion guardarficha en donde los valores de entrada iran cambiando en cada interacion

        }




    }  




// aqui inician las opciones

int opcion= -1;
    while (opcion!=0){
        cout<<"elije una de las siguientes opciones"<<endl;


         cout<<"1. ver tablero"<<endl<<"2. memoria"<<endl<<"3. eliminar casilla "<<endl<<"4. modifical el tablero"<<endl<<"0. salir"<<endl<<endl;


        cin>>opcion;
         cout<<"la opcion elijida fue: "<<opcion<<endl;


         if (opcion ==1){


             memoria =imprimirtablerofichas(memoria,fila, columna);//llamamos esta funcion para traer los valores almacenados de las fichasaleatorias

         }
         else if(opcion ==2){
             memoria =imprimirtablerobinario(memoria,fila,columna);/*cuando se ejecutamos esta opcion esta imprime la representacion directa
                                                                       en bits o numeros binarios que almacenamos en memoria*/


         }


         else if(opcion==3){

             /*seleccionamos las coordenadas fila y columnas*/

             cout<<"coloca la fila: "<<endl;
                     cin>>seleccion_fila;

             cout<<"coloca la columna: "<<endl;
                     cin>>seleccion_columna;

                     /*se evalua 4 condiciones con && con lo que no aseguramos que todas deben ser verdaderas, donde no dejamos ingrear
                      indices negativos y que la selecion de filas y columnas no superen el tamaño del tablero, si todo cuple se llama la funcion
                       la funcion guardarficha.  */

                     if (seleccion_fila>=0 && seleccion_fila < fila && seleccion_columna>=0 &&seleccion_columna < columna ){

                         memoria= guardarficha(memoria, columna, seleccion_fila, seleccion_columna, 6);
                         eliminacionesusuario++;
                         cout<<"se elimino la ficha" <<endl;

                     }else{

                         cout<<"no se encuentra la posicion"; //se activa cuando la selecion de fila como columnas no coinciden con el tablero//

                     }
         }

         else if(opcion==4){

           int fila1=0, columna1=0;
             cout << "ingrese el valor de la fila: " << endl;
             cin>>fila;


             cout<< "ingresa el valor para las columnas "<<endl;
             cin>>columna;


             unsigned char* nueva_dir= redimencionar(memoria, fila, columna, fila1,columna1, eliminacionesusuario); //lo datos suministrados van para funcion calcularbytesnecesarios


             if (nueva_dir!=memoria){
                 memoria=nueva_dir;
                fila=fila1;
                 columna=columna1;
                eliminacionesusuario=0;

             }

             cout<<fila<<" "<<columna<<endl;


         }
         else if(opcion==5){

             int sub_menu=0, posicion=0;

             cout<<"elije una de las siguientes opciones: "<<endl<<endl;

             cout<<"1. agregar fila "<<endl<<"2. eliminar fila"<<endl<<"3. agregar columna  "<<endl<<"4. eliminar columna "<<endl<<endl;
             cin>>sub_menu<<endl;

             cout<<"la opcion ingresada fue: "<<sub_menu<<endl;


             if(sub_menu==1){

                 cout<<"indique en donde colocar la nueva fila de la 0 a la "<<fila<<": ";

                 cin>>posicion;

                 memoria=agregar_fila(memoria, fila, columna, posicion);
                 }

             else if(sub_menu==2){
                 cout<<"indique que fila va a eliminar de la 0 a la "<<fila<<": ";
                     cin>>posicion;

                 memoria=eliminar_fila(memoria, fila, columna, posicion, eliminacionesusuario);

                 }
                 else if(sub_menu==3){
                     cout<<"indique en donde colocar la nueva columna de la 0 a la "<<columna<<": ";
                     cin>>posicion;
                     memoria=agregar_columna(memoria, fila, columna, posicion);
                 }


                 else if(sub_menu==4){
                     cout<<"indique que columna va a eliminar de la 0 a la "<<columna<<": ";
                     cin>>posicion;

                     memoria=eliminar_columna(memoria, fila, columna, posicion, eliminacionesusuario);
                 }else{
                     cout<<"opcion invalida"
                 }

                 memoria=procesar_cascadas(memoria, fila, columna, posicion, eliminacionesusuario);

         }//cierra opcion 5

         else if (opcion==0){
             cout<<"juego finalizado";
         }
         else{
             cout<<"opcion invalida "<<endl<<endl;}


    }

    delete[]memoria;
    return 0;
}

