#include "crush.h"
#include <iostream>
#include <ctime>
#include <cstdlib>


using namespace std;


int calcularbytesnecesarios (int filas , int columnas) {
    int totalbits = filas * columnas * 3;/*verificamos la cantidad de bits necesarios al multiplicar la cantidad de filas y columnas
                                         y luego multiplicamos x 3 que son la cantidad de bits que va a llevar cada ficha */
    return (totalbits + 7) /8;/*sumamos 7 bits con la intencion de completar si llega a faltar para completar la ultima ficha
                                posteriormente dividimos entre 8 para obtener la cantidad de bytes que van a requerir estas fichas*/
}




/*devuelve la direccion de memoria donde se almacena el tablero en el heap para realizar los diferentes cambios necesarios apuntando
 siempre a la localizacion donde se deben realizar los cambios o la lectura de este */
unsigned char* guardarficha ( unsigned char* memoria,int columnas, int fila,int columna, int valor){
    valor =  valor & 7 ;
    int indiceficha = fila * columnas + columna;/*se multiplica la cantidad de columnas por la fila y columna en la que
                                                vamos guardar cada ficha dandonos la pocision donde se va a guardar cada
                                                ficha*/
    int bitinicial = indiceficha * 3; //se multiplica por la cantidad de bits que requiere cada ficha//
    int numerobytes = bitinicial/8;/*se divide por la cantidad de bits que tiene cada byte para obtener
                                    la posicion donde vamos a guardar los bits */
    int posicionenbyte = bitinicial % 8;/*nos da la posicion desde la cual vamos a empezar
                                            a guardar los bits siendo este el residuo*/

    if (posicionenbyte <= 5) {/*si la posicion de guardado es menor a 5 sabemos que va a quedar en ese byte y no requerira
                                pegarse a otro y entra*/
        memoria[numerobytes] &= ~(7 << posicionenbyte);/* realizamos una limpieza con el operador de negacion en
                                                        bits, invirtiendo cada 0 a 1  y viceversa realiza una mascara
                                                        al 7 pero primero lo desplaza hacia la izquierda la cantidad
                                                        de pocision en byte */
        memoria[numerobytes] |= (valor << posicionenbyte);/* se aplica el operador or| para introducir la ficha (valor)
                                        pero primero se desplaza  hacia la isquierda la cantidad de posicionenbyte*/
    }
    else if (posicionenbyte == 6) {/*si la posicion de guardado es igual a 6 requerira quedar con 2 bits dentro de este byte
                                    y 1 bit dentro del otro byte siendo necesario hacer lo siguiente */
        int parte1 = valor & 3;  /*se le aplica mascara 3 a la ficha que esta en la variable valor para extraer solo los 2
                                    ultimos bits de mas a la derecha */
        int parte2 = (valor >> 2) & 1;/* desplazamos los bits 2 espacios hacia la derecha quedando solo
                                    el 3 bit de derecha a izquierda y aplicamos mascara 1 para quedarnos con ese bit restante */
        memoria[numerobytes] &= ~(3 << 6);/*seleccionamos el byte donde va a quedar guardado se desplaza 6 espacios hacia la
                                izquierda aplicamos mascara 3 y utilizamos el operador de negacion para invertir cada bit
                                que este en 0 queda en uno y viceversa con la intencion de limpiar el espacio de guardado */
        memoria[numerobytes] |= (parte1 << 6);/*seleccionamos el espacio de guardado en memoria coje el valor de parte 1
                                            y lo desplaza 6 puestos hacia la izquierda se aplica el operador or | para poder colocar
                                            los bits en la pocision 6 y 7 */

        memoria[numerobytes + 1] &= ~1;/*seleccionamos el byte +1 posicion para guardar la continuacion de los bits
                                        y guardar el bit sobrante que tenemos en la parte 2 para esto aplicamos mascara 1
                                        y utilizamos el operador nulo para limpiar la posicion donde vamos a guardar bit 0*/
        memoria[numerobytes + 1] |= parte2;/*seleccionamos el bite +1 y le añadimos la parte 2 con el operador or */
    }
    else {/*si la posicion de guardado es superior a 6  tomara este camino siendo esta como maximo 7 ya que
                                esto lo determina el modulo 8 que aplicamos en un principio quedando de reciduo del 0 al 7*/
        int parte1 = valor & 1; /*esto nos indica que nos va a quedar solo un espacio por tomar de los bits y vamos a requerir
                                tomar los otros 2 de el siguiente byte asi que empezamos aplicando a la ficha mascara 1
                                quedandonos con la  posicion 7 del byte*/
        int parte2 = (valor >> 1) & 3;/*guardamos en la variable parte2 la ficha pero primero la desplazamos 1 posicion hacia
                                        la derecha y le aplicamos mascara 3 dejandonos asi 2 posiciones de la ficha en el bit 0 y 1*/

        memoria[numerobytes] &= ~(1 << 7);/*seleccionamos la posicion de guardado para este bit le aplicamos un desplazamiento
                                        de 7 posiciones hacia la izquierda y le aplicamos mascara 1 y utilizamos el operador
                                        nulo para limpiar la posicion donde vamos a guardar el bit de la parte1*/
        memoria[numerobytes] |= (parte1 << 7);/* seleccionamos la posicion de guardado aplicamos el operador or para poner los bits
                                            de la parte1 pero previo a esto nos desplazamos 7 puestos hacia la izquierda quedando en
                                            el bit 1 */

        memoria[numerobytes + 1] &= ~3;/*seleccionamos la posicion de guardado para estos bits siendo la posicion+1 le aplicamos
                                        a la posicion mascara 3 y aplicamos el operador nulo coon la intencion de limpiar los bits
                                        donde se va a guardar los bits que van a complementar la ficha juntando la parte1 y parte2 */
        memoria[numerobytes + 1] |= parte2;/*seleccionamos la posicion de guardado para los bits y le aplicamos el operador or con
                                            la intencion de descargar la informacion de la parte 1 */


    }
    return memoria;/*se guardan las posiciones de memoria con sus respectivos binarios asignados para cada situacion. */
}

int leerfichas( unsigned char* memoria,int columnas, int fila,int columna) {

    int indiceficha = fila*columnas+columna;/*determinamos el indice desde el cual vamos a empezar a leerlas fichas*/
    int bitinicial = indiceficha*3;/*calculamos la posicion de bits que utiliza cada fila  para ser leida */
    int numerobytes = bitinicial/8;/*calculamos el byt en el que va a empezar a leer */
    int posicionenbyte = bitinicial % 8;/*calculamos el byte en el que van a ser leidas las fichas */


    if (posicionenbyte <=5){ /*se verifica la posion en la que empiza a leer para poder imprimir mas adelante*/
        return (memoria[numerobytes]>> posicionenbyte) & 7;/*seleccionamos desde donde vamos a empezar a leer se desplaza hacia la
        derecha la cantidad de posicionenbyte y le aplica mascara 7 esto con la intencion de garantizar siempre que se desplacen
        hasta la posicion derecha y pueda retornar el numero exacto realizando una limpieza con mascara 7*/
    }

    else if (posicionenbyte == 6 ){ /*verificamos si la posicion en bytes es igual a 6 para continuar con su respectiva operacion*/
        int parte1=( memoria [numerobytes] >> 6)&3;/*guardamos en el entero parte1 lo que tenemos en la memoria en la posicion
                                                    numerobytes y lo desplazamos 6 pocisiones hacia la derecha y le aplicamos mascara
                                                    3 para seleccionar los ultimos 3 bits*/
        int parte2 = (memoria [numerobytes + 1] & 1)<<2;/*guardamos en el entero parte2 lo que tenemos en la memoria en la posicion
                                                    numerobytes + 1 le aplicamos mascara 1 y nos quedamos con el ultimo bite
                                                    y lo desplazamos 2 bits hacia la izquierda */
        return parte1|parte2; /*retornamos parte1 or parte2 lo cual ejecuta el uno encima del otro logrando esto que queden unidos los
                            ultimos 3 bits en un solo byte arrojando el numero resultante ejm 00000011
                                                                                              00000100
                                                                                            = 00000111 */

    }
    else{/*si es ==7  entre y realice la siguiente operacion*/
        int parte1 = (memoria [numerobytes]>>7)&1;/*en el entero parte1 guardamos lo que tenemos en memoria en la posicion numerobytes
                                                realizamos un desplazamiento hacia la derecha de 7 pocisiones y le aplicamos
                                                mascara 1 con la intencion de tener solo el ultimo bit de esa posicion*/
        int parte2 = (memoria[numerobytes + 1] & 3)<<1;/*en el entero parte2 guardamos lo que tenemos en la posicion numerobytes+1
                                                    realizamos una mascara 3 y un desplazamiento de 1 bit hacia la izquierda*/
        return parte1|parte2;/*retornamos parte1 or parte2 lo cual ejecuta uno encima del otro logrando esto que queden unidos los
                            ultimos 3 bits en un solo byte de 8 bits arrojando el numero resultante ejm 00000001
                                                                                                        00000110
                                                                                                       =00000111 */

    }

}
char obtenercaracter(int valor){
    int codigo = valor & 7;/*toma los bits que estan en (valor) y les aplica mascara 7 */
    if(codigo <=5 ){/*si el resultante en bits es menor o igual a 5 entre esto para limitar las letras a utilizar de A-F*/
        char caracter = 'A' + codigo;/*en este char realizamos una suma de el codigo en binario +A logrando asi que se empiece en A
                    y dependiendo de el numero que se sume avanza a la siguiente letra ejemplo A+1=B A+2=C A+3=D......*/
        return caracter;/*retorna el caracter final */
    }
    return '$' ;/*en caso de que el valor sea superior a 5 retorna el caracter de $*/
}

unsigned char* imprimirtablerofichas ( unsigned char* memoria, int filas,int columnas)
/*devuelve la direccion de memoria donde se almacena el tablero en el heap para realizar los diferentes cambios necesarios apuntando
 siempre a la localizacion donde se deben realizar los cambios o la lectura de este */
{

    cout << "      TABLERO     " << endl;
    for (int f=0;f<filas; ++f) {
        cout << " FILA "<< f << " ";/*va a imprimir en que fila esta cada una empezando desde 0*/
        for (int c=0;c<columnas;++c){
            int valor = leerfichas(memoria,columnas,f,c);/*va a retornar cada ficha en cada posicion que se solicite de acuerdo
                                                            al indice que se pida en fila y columna*/
            cout << obtenercaracter(valor) << " " ;/*imprime el caracter de acuerdo a los bits que retorne de la funcion leerfichas
                                                    entrando en la funcion obtenercaracter retornando cada letra de acuerdo a
                                                    el valor que se le entregue  */

        }
        cout << endl;
    }
    return memoria;

}
/*devuelve la direccion de memoria donde se almacena el tablero en el heap para realizar los diferentes cambios necesarios apuntando
 siempre a la localizacion donde se deben realizar los cambios o la lectura de este */
unsigned char* imprimirtablerobinario ( unsigned char* memoria, int filas,int columnas) {
    int totalbytes = calcularbytesnecesarios(filas,columnas);/*ejecutamos la funcion calcularbytesnecesarios*/
    cout << "ram en binario memoria fisica" << endl;
    for (int b=0;b<totalbytes; ++b) {
        cout << "byte"<< b << " : ";/*imprime el byte donde esta guardado la cadena de bits */
        for (int bit = 7 ; bit >= 0; --bit ){
            cout <<((memoria[b]>> bit) & 1);/*accede a la cadena de bits que esta en la posicion de el byte
                                                en la variable memoria*/


        }
        cout << endl;

    }
    return memoria;

}
/*devuelve la direccion de memoria donde se almacena el tablero en el heap para realizar los diferentes cambios necesarios apuntando
 siempre a la localizacion donde se deben realizar los cambios o la lectura de este */
unsigned char* redimencionar ( unsigned char* memoria,int filas_actuales, int columnas_actuales,int filas_nuevas, int columnas_nuevas,int eliminaciones){
    int total_casillas= filas_actuales*columnas_actuales;/*verificamos la cantidad de fichas que se implementaron en un principio*/
    int casillas_activas = total_casillas - eliminaciones;/*le restamos la cantidad de fichas eliminadas*/

    double porcentaje=((double)casillas_activas/total_casillas)*100.0;/*sacamos el porcentaje que queda de fichas*/
    if (porcentaje < 65.0){/*si el porcentaje es inferior a 65% entra */
        delete [] memoria;/*eliminamos la memoria dinamica que existe en la variable memoria*/

        int nuevos_bytes = calcularbytesnecesarios(filas_nuevas,columnas_nuevas);/*verificamos cuantos bytes requiere nuestra cuadricula
                                                                                de acuerdo a nuestras nuevas dimenciones*/
        unsigned char* nueva_memoria = new unsigned char [nuevos_bytes]();/*asignamos un puntero a nuestra nueva memoria con un arreglo
                                                                        que varia de acuerdo a la cantidad de bytes que hayamos calculado
                                                                        con la funcion anterior*/

        for(int f=0; f < filas_nuevas; ++f ){/*realizamos un recorridos fila por fila */
            for (int c=0;c< columnas_nuevas; ++c){/*entramos en cada columna de las diferentes filas*/
                int fichaaleatoria= rand() %6;/*asignamos un numero aleatorio gracias a la semilla que implementa rand y aplicamos modulo 6
                                            para limitar la cantidad de letras a un maximo de 5 siendo desde la A hasta la F*/
                nueva_memoria = guardarficha(nueva_memoria,columnas_nuevas,f,c,fichaaleatoria);/*de acuerdo a la fila y columna en la que
                                                                                entramos guardamos una determinada ficha que sera almacenada
                                                                                de acuerdo a ala estructura y cantidad de cada byte siendo
                                                                                explicado en la funcion guardarficha */

}
/*devuelve la direccion de memoria donde se almacena el tablero en el heap para realizar los diferentes cambios necesarios apuntando
 siempre a la localizacion donde se deben realizar los cambios o la lectura de este */
unsigned char* redimencionar ( unsigned char* memoria,int filas_actuales, int columnas_actuales,int filas_nuevas, int columnas_nuevas,int eliminaciones){
    int total_casillas= filas_actuales*columnas_actuales;/*verificamos la cantidad de fichas que se implementaron en un principio*/
    int casillas_activas = total_casillas - eliminaciones;/*le restamos la cantidad de fichas eliminadas*/

    double porcentaje=((double)casillas_activas/total_casillas)*100.0;/*sacamos el porcentaje que queda de fichas*/
    if (porcentaje < 65.0){/*si el porcentaje es inferior a 65% entra */
        delete [] memoria;/*eliminamos la memoria dinamica que existe en la variable memoria*/

        int nuevos_bytes = calcularbytesnecesarios(filas_nuevas,columnas_nuevas);/*verificamos cuantos bytes requiere nuestra cuadricula
                                                                                de acuerdo a nuestras nuevas dimenciones*/
        unsigned char* nueva_memoria = new unsigned char [nuevos_bytes]();/*asignamos un puntero a nuestra nueva memoria con un arreglo
                                                                        que varia de acuerdo a la cantidad de bytes que hayamos calculado
                                                                        con la funcion anterior*/

        for(int f=0; f < filas_nuevas; ++f ){/*realizamos un recorridos fila por fila */
            for (int c=0;c< columnas_nuevas; ++c){/*entramos en cada columna de las diferentes filas*/
                int fichaaleatoria= rand() %6;/*asignamos un numero aleatorio gracias a la semilla que implementa rand y aplicamos modulo 6
                                            para limitar la cantidad de letras a un maximo de 5 siendo desde la A hasta la F*/
                nueva_memoria = guardarficha(nueva_memoria,columnas_nuevas,f,c,fichaaleatoria);/*de acuerdo a la fila y columna en la que
                                                                                entramos guardamos una determinada ficha que sera almacenada
                                                                                de acuerdo a ala estructura y cantidad de cada byte siendo
                                                                                explicado en la funcion guardarficha */

            }


        }

        cout << "redimensionamiento completado dimencion antigua"<< porcentaje << "% " <<endl;
        return nueva_memoria;/*se almacena la informacion en nueva memoria para poder ser invocado mas adelante*/

    }
    cout << "no puede ser redimencionado el porcentaje es "<< porcentaje << "% el porcentaje debe ser inferior a 65%"<<endl;
    return memoria;/*en caso de que no se redimencione simplemente retorna la memoria original*/

}
int combos ( unsigned char* memoria, int filas,int columnas,int& fichas_eliminadas){
    int total_fichas= filas*columnas;
    bool * marcados = new bool [total_fichas]();
    int combos_encontrados=0;
    fichas_eliminadas=0;

    //horizontales
    for(int f=0; f < filas; ++f ){/*realizamos un recorridos fila por fila */
        for (int c=0;c< columnas-2; ++c){/*entramos en cada columna de las diferentes filas*/
            int v1 = leerfichas(memoria,columnas,f,c);
            int v2 = leerfichas(memoria,columnas,f,c+1);
            int v3 = leerfichas(memoria,columnas,f,c+2);

            if (v1<=5 && v1 == v2 && v1 == v3){
                combos_encontrados++;
                int k = c;
                while (k < columnas && leerfichas(memoria,columnas,f,k) == v1){
                    marcados[f*columnas+k]=true;
                    k++;

                }
            }

        }

    }
    //verticales
    for(int c=0; c < columnas ; ++c ){/*realizamos un recorridos fila por fila */
        for (int f=0;f< filas-2; ++f){/*entramos en cada columna de las diferentes filas*/
            int v1 = leerfichas(memoria,columnas,f,c);
            int v2 = leerfichas(memoria,columnas,f+1,c);
            int v3 = leerfichas(memoria,columnas,f+2,c);

            if (v1<=5 && v1 == v2 && v1 == v3){
                combos_encontrados++;
                int k = f;
                while (k < filas && leerfichas(memoria,columnas,k,c) == v1){
                    marcados[k*columnas+c]=true;
                    k++;
                }
            }
        }
    }
    for(int f=0; f < filas; ++f ){/*realizamos un recorridos fila por fila */
        for (int c=0;c< columnas; ++c){/*entramos en cada columna de las diferentes filas*/

            if (marcados [f * columnas + c]) {
                fichas_eliminadas++;
                guardarficha(memoria,columnas,f,c,6);
            }
        }
    }
    delete [] marcados;
    return combos_encontrados;

}

}

unsigned char* gravedad_rellenar (unsigned char* memoria,int filas,int columnas){
    for (int c = 0; c < columnas; c++){
        int escritura=filas-1;
        for (int f=filas-1;f>= 0;f--){
            int v = leerfichas(memoria,columnas,f,c);
            if (v <=5){
                guardarficha(memoria,columnas,escritura,c,v);
                escritura--;
            }
        }
        while (escritura>=0){
            int ficha_nueva= rand()%6;
            guardarficha(memoria,columnas,escritura,c,ficha_nueva);
            escritura--;

        }
    }

    return memoria;
}


unsigned char* cascada(unsigned char* memoria,int filas,int columnas,int& puntaje,int& total_combos){

    while (true){
        int fichas_eliminadas=0;
        int combos1= combos ( memoria,filas,columnas,fichas_eliminadas);
        if (combos1 == 0){
            break;
        }

        total_combos +=combos1;
        puntaje += (fichas_eliminadas*10);

        memoria = gravedad_rellenar(memoria,filas,columnas);
    }
    return memoria;
}

unsigned char* agregar_fila(unsigned char* memoria,int& filas,int columnas,int indice){

    int nuevas_filas= filas+1;
    int bytes = calcularbytesnecesarios(nuevas_filas,columnas);
    unsigned char* nueva = new unsigned char[bytes]();


    for (int f=0; f<nuevas_filas;f++){
        for (int c=0; c<columnas;c++){
            if (f<indice){
                int valor=leerfichas(memoria,columnas,f,c);
                guardarficha(nueva,columnas,f,c,valor);
            } else if (f==indice){
                guardarficha(nueva,columnas,f,c,rand()%6);
            }else {int valor=leerfichas(memoria,columnas,f-1,c);
                guardarficha(nueva,columnas,f,c,valor);

            }
        }

    }
    delete[] memoria;
    filas= nuevas_filas;
    return nueva;
}

unsigned char* eliminar_fila(unsigned char* memoria,int& filas,int columnas,int indice,int& eliminacionesusuario){

    if (filas<=1) return memoria;
    int nuevas_filas =filas-1;
    int bytes = calcularbytesnecesarios(nuevas_filas,columnas);
    unsigned char* nueva = new unsigned char[bytes]();


    for (int f=0; f< filas;f++){
        if (f==indice) continue;
        int f_destino = (f<indice) ? f : f-1;
        for (int c=0; c<columnas;c++){
            int valor=leerfichas(memoria,columnas,f,c);
            guardarficha(nueva,columnas,f_destino,c,valor);


        }

    }
    delete[] memoria;
    filas= nuevas_filas;
    eliminacionesusuario=0;
    return nueva;
}


unsigned char* agregar_columna(unsigned char* memoria,int filas,int& columnas,int indice){

    int nuevas_columnas= columnas+1;
    int bytes = calcularbytesnecesarios(filas,nuevas_columnas);
    unsigned char* nueva = new unsigned char[bytes]();


    for (int f=0; f<filas;f++){
        for (int c=0; c<nuevas_columnas;c++){
            if (c<indice){
                int valor=leerfichas(memoria,columnas,f,c);
                guardarficha(nueva,nuevas_columnas,f,c,valor);
            } else if (c==indice){
                guardarficha(nueva,nuevas_columnas,f,c,rand()%6);
            }else {int valor=leerfichas(memoria,columnas,f,c-1);
                guardarficha(nueva,nuevas_columnas,f,c,valor);

            }
        }

    }
    delete[] memoria;
    columnas= nuevas_columnas;
    return nueva;
}



unsigned char* eliminar_columna(unsigned char* memoria,int filas,int& columnas,int indice,int& eliminacionesusuario){

    if (columnas<=1) return memoria;
    int nuevas_columnas =columnas-1;
    int bytes = calcularbytesnecesarios(filas,nuevas_columnas);
    unsigned char* nueva = new unsigned char[bytes]();


    for (int f=0; f< filas;f++){
        for (int c=0; c<columnas;c++){
            if (c==indice) continue;
            int c_destino = (c<indice) ? c : c-1;
            int valor=leerfichas(memoria,columnas,f,c);
            guardarficha(nueva,nuevas_columnas,f,c_destino,valor);


        }

    }
    delete[] memoria;
    columnas= nuevas_columnas;
    eliminacionesusuario=0;
    return nueva;
}



















