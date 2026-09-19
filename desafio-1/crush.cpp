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
        cout << "byte"<< b << ": ";/*imprime el byte donde esta guardado la cadena de bits */
        for (int bit = 7 ; bit >= 0; --bit ){
            cout <<((memoria[b]>> bit) & 1);/*accede a la cadena de bits que esta en la posicion de el byte
                                                en la variable memoria*/

        }




        }
        cout << endl;


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


        }

        cout << "redimensionamiento completado dimencion antigua"<< porcentaje << "% " <<endl;
        return nueva_memoria;/*se almacena la informacion en nueva memoria para poder ser invocado mas adelante*/

    }
    cout << "no puede ser redimencionado el porcentaje es "<< porcentaje << "% el porcentaje debe ser inferior a 65%"<<endl;
    return memoria;/*en caso de que no se redimencione simplemente retorna la memoria original*/


}
int combos ( unsigned char* memoria, int filas,int columnas,int& fichas_eliminadas){
    int total_fichas= filas*columnas;/*verificamos el total de fichas */
    bool * marcados = new bool [total_fichas]();/*marcamos los lugares espesificos donde las fichas son iguales*/
    int combos_encontrados=0;/*contabilizamos cada combo que encontramos*/
    fichas_eliminadas=0;/*contamos las fichas eliminadas*/

    //fichas horizontales
    for(int f=0; f < filas; ++f ){/*realizamos un recorridos fila por fila */
        for (int c=0;c< columnas-2; ++c){/*entramos en cada columna de las diferentes filas teniendo en cuenta que se limita a entrar hasta 2 posiciones antes de finalizar
                                        para que asi no haya un desbordaniemto */
            int v1 = leerfichas(memoria,columnas,f,c);/*lee la ficha en la  psicion c y guarda la ficha en v1*/
            int v2 = leerfichas(memoria,columnas,f,c+1);/*lee la ficha en la  posicion c+1 y guarda la ficha en v2*/
            int v3 = leerfichas(memoria,columnas,f,c+2);/*lee la fiha en la  posicion c+2 y guarda la ficha en v3*/

            if (v1<=5 && v1 == v2 && v1 == v3){/*verifica que la ficha v1 no sea un numero superior a 5  y verifica si las fichas siguientes son iguales a la v1*/
                combos_encontrados++;/*si se cumplen las condiciones se incrementa en uno los combos encontrados*/
                int k = c;/*si se cumplen las coondiciones  anteriores realizamos un cambio de variable para la posicion c */
                while (k < columnas && leerfichas(memoria,columnas,f,k) == v1){/*mienras k sea menor que la dimencion de  columnas y al leer la ficha en esa posicion
                                                                            se determina que son iguales a v1 entra al ciclo*/
                    marcados[f*columnas+k]=true;/*se marca la posicion exacta donde se encuentran las fichas iguales */
                    k++;/*se incrementa en uno la variable para poder realizar la verificacion de las fichas iguales una a una */

                }
            }
        }

    }
    //verticales
    for(int c=0; c < columnas ; ++c ){/*realizamos un recorridos columna por columna */
        for (int f=0;f< filas-2; ++f){/*entramos en cada fila de las diferentes columnas*/
            int v1 = leerfichas(memoria,columnas,f,c);/*lee la ficha en la  psicion f y guarda la ficha en v1*/
            int v2 = leerfichas(memoria,columnas,f+1,c);/*lee la ficha en la  psicion f+1 y guarda la ficha en v2*/
            int v3 = leerfichas(memoria,columnas,f+2,c);/*lee la ficha en la  psicion f+2 y guarda la ficha en v3*/

            if (v1<=5 && v1 == v2 && v1 == v3){/*verifica que la ficha v1 no sea un numero superior a 5  y verifica si las fichas siguientes son iguales a la v1*/
                combos_encontrados++;/*si se cumplen las condiciones se incrementa en uno los combos encontrados*/
                int k = f;/*si se cumplen las coondiciones  anteriores realizamos un cambio de variable para la posicion f */
                while (k < filas && leerfichas(memoria,columnas,k,c) == v1){/*mienras k sea menor que la dimencion de  filas y al leer la ficha en esa posicion
                                                                            se determina que son iguales a v1 entra al ciclo*/
                    marcados[k*columnas+c]=true;/*se marca la posicion exacta donde se encuentran las fichas iguales */
                    k++;/*se incrementa en uno la variable para poder realizar la verificacion de las fichas iguales una a una */
                }
            }
        }
    }
    for(int f=0; f < filas; ++f ){/*realizamos un recorridos fila por fila */
        for (int c=0;c< columnas; ++c){/*entramos en cada columna de las diferentes filas*/

            if (marcados [f * columnas + c]) {/* si la posicion de marcados es true entra al ciclo */
                fichas_eliminadas++; /*se incrementa en uno la cantidad de fichas eliminadas*/
                guardarficha(memoria,columnas,f,c,6);/*como anterior mente verificamos si la posicion marcada era true entramos a esa misma posicion y lo marcamos con la ficha
                                                    de eliminacion que asignamos anteriormente $*/
            }
        }
    }
    delete [] marcados;/*finalizamos el puntero para que no hayan fugas de memoria */
    return combos_encontrados;/*retornamos la cantidad de combos encontrados */

}


unsigned char* gravedad_rellenar (unsigned char* memoria,int filas,int columnas){/*se define una funcion que por medio de un puntero apunta a la direccion de (memoria)
                                                                                    con la intencion de almacenar los diferentes cambios realizados al tablero */
    for (int c = 0; c < columnas; c++){/*realizamos un recorrido columna por columna*/
        int fila_destino=filas-1;/*marca el lugar donde se debe poner la siguiente ficha*/
        for (int f=filas-1;f>= 0;f--){/*realizamos un recorrido fila por fila desde la posicion mas baja hacia la mas alta */
            int v = leerfichas(memoria,columnas,f,c);/*guardamos la informacion que recibimos de leerfichas en la posicion que asignamos de acuerdo al recorrido*/

            if (v <=5){/*si la ficha no excede el 5 letra f entra al ciclo*/
                guardarficha(memoria,columnas,fila_destino,c,v);/*guardamos la ficha en la fila y columna asignada*/
                fila_destino--;/*reduce en uno  la fila destino para rellenar la siguiente*/
            }
        }
        while (fila_destino>=0){/*si el contador de filas vacias verifica que es superior a cero entra al ciclo*/
            int ficha_nueva= rand()%6;/*genera una nueva ficha con la semilla y a la semilla le aplica modulo 6 para limitar la cantidad de letras disponibles */
            guardarficha(memoria,columnas,fila_destino,c,ficha_nueva);/*guarda las nuevas fichas en las posiciones vacias cerrando asi las posiciones que queden vacias*/
            fila_destino--;/*reduce en uno la fila destino para rellenar la fila siguiente*/

        }
    }

    return memoria;
}


unsigned char* cascada(unsigned char* memoria,int filas,int columnas,int& puntaje,int& total_combos){

    while (true){/*mientras la posicion asignada este marcada con true ingrese al ciclo*/
        int fichas_eliminadas=0;/*incrementa de uno en uno dependdiendo de las fichas que esten marcadas con el boolleano true*/
        int combos1= combos ( memoria,filas,columnas,fichas_eliminadas);/*activa la funcion combos para verificar si existe alguna convinacion que elimine fichas*/
        if (combos1 == 0){/*si combos1 no encuentra ningun combo entra al sicl*/
            break;/*el siclo es interrumpido por un break obligando a retornar la informacion de memoria */
        }

        total_combos +=combos1;/*se incrementa la totalidad de combos  dependiendo de los combos que nos retornen la funcion combos*/
        puntaje += (fichas_eliminadas*10);/*se genera un contador de puntos que nos da 10 puntos por cada ficha eliminada*/

        memoria = gravedad_rellenar(memoria,filas,columnas);/*se invoca la funcion gravedad rellenar para tapar los espacios generados por la eliminacion de lla funcion commbos*/
    }
    return memoria;

}

unsigned char* agregar_fila(unsigned char* memoria,int& filas,int columnas,int indice){

    int nuevas_filas= filas+1;/*se incrementa en uno la cantidad de filas a tener */
    int bytes = calcularbytesnecesarios(nuevas_filas,columnas);/*calculamos cuantos bytes requiere de acuerdo ala nueva cantidad de filas */
    unsigned char* nueva = new unsigned char[bytes]();/*reasignamos la cantidad de memoria a un nuvo puntero siendo este llamado nueva */


    for (int f=0; f<nuevas_filas;f++){/*realizamos un recorrido fila por fila hasta la posicion de nuevas filas*/
        for (int c=0; c<columnas;c++){/*realizamos un recorrido columna por columna hasta haber revisado todas las columnas de la hilera de la fila determinada*/
            if (f<indice){/*si el recorrido de las filas en la posicion f es menor a el indice o posicion asignada de la nueva fila entonces entre al condicional */
                int valor=leerfichas(memoria,columnas,f,c);/*el entero valor va a guardar la informacion asignada a leerfichas en la posicion de busqueda original*/
                guardarficha(nueva,columnas,f,c,valor);/*se guardan las fichas de la posicion original en la nueva memoria */
            } else if (f==indice){/*si el recorrido de las filas en la posicion f es igual a la posicion asignada  de la nueva fila entonces entre al condicional*/
                guardarficha(nueva,columnas,f,c,rand()%6);/*asigne fichas a la nueva fila y guardelas en la memoria nueva*/
            }else {int valor=leerfichas(memoria,columnas,f-1,c);/*se guarda la informacion de las filas viejas en la variable valor*/
                guardarficha(nueva,columnas,f,c,valor);/*guardamos las fichas en la nueva memoria que ya tiene la nueva cantidad de bytes esto con la intencion de poder
                asignar el valor correspondiente de memoria a cada uno de los casos sea un incremento para esta ocacion o un decremento en otras ocaciones */

            }
        }

    }
    delete[] memoria;/*liberamos la memoria vieja para evitar fugas de memoria*/
    filas= nuevas_filas;/*asignamos la  nueva cantidad de filas que quedaron en la tabla*/
    return nueva;/*retornamos el puntero nueva*/
}

unsigned char* eliminar_fila(unsigned char* memoria,int& filas,int columnas,int indice,int& eliminacionesusuario){

    if (filas<=1) return memoria;/*si las filas son menores o iguales a uno no hay forma de eliminar filas por ende retorna memoria */
    int nuevas_filas =filas-1; /*se calcula que como se va a eliminar una fila se reduce en uno y se guarda en la nueva variable nuevas_filas*/
    int bytes = calcularbytesnecesarios(nuevas_filas,columnas);/*se calcula la cantidad de bytes que se requieren teniendo en cuenta la nueva cantidad de filas */
    unsigned char* nueva = new unsigned char[bytes]();/*creamos un nuevo puntero para señalar en que lugar de la memoria va a quedar la nueva tabla de fichas con los cambios
                                                        pertinentes*/


    for (int f=0; f< filas;f++){/*realizamos un recorrido por las filas empezando por la primera pocision*/
        if (f!=indice){/*si la fila es diferente de la fila que queremos eliminar entramos al condicional */
            int f_destino = (f<indice) ? f : f-1; /*se asigna al entero f_destino la fila que sea menor a la fila que queremos eliminar o si esta no cumple con ser menor
                                            que la fila que queremos eliminar entonces le resta 1 desplazandolas filas hacia arriba cerrando los espacios que quedan
                                            por las eliminaciones */
            for (int c=0; c<columnas;c++){/*realizamos un recorrido por las columnas empezando por la primera posicion*/
                int valor=leerfichas(memoria,columnas,f,c);/*el entero valor recibe la ficha despues de leerla en la memoria vieja*/
                guardarficha(nueva,columnas,f_destino,c,valor);/*guardamos las fichas en la nueva memoria en la posicion que designamos despues de realizar la eliminacion
                                                            de la fila respectiva en f_destino*/


            }

        }

    }
    delete[] memoria;/*liberamos la memoria vieja para evitar fugas de memoria */
    filas= nuevas_filas;/*reasignamos el nuevo valor a las filas*/
    eliminacionesusuario=0;/*reiniciamos la cantidad de eliminaciones*/
    return nueva;/*retornamos la memoria  nueva*/
}


unsigned char* agregar_columna(unsigned char* memoria,int filas,int& columnas,int indice){

    int nuevas_columnas= columnas+1;/*se incrementa en uno la cantidad de columnas a tener */
    int bytes = calcularbytesnecesarios(filas,nuevas_columnas);/*calculamos cuantos bytes requiere de acuerdo ala nueva cantidad de columnas */
    unsigned char* nueva = new unsigned char[bytes]();/*reasignamos la cantidad de memoria a un nuvo puntero siendo este llamado nueva */


    for (int f=0; f<filas;f++){/*realizamos un recorrido fila por fila hasta la posicion filas*/
        for (int c=0; c<nuevas_columnas;c++){/*realizamos un recorrido columna por columna hasta haber revisado todas las nuevas_columnas de la fila determinada*/
            if (c<indice){/*si el recorrido de las filas en la posicion c es menor a el indice o posicion asignada de la nueva columna entonces entre al condicional */
                int valor=leerfichas(memoria,columnas,f,c);/*el entero valor va a guardar la informacion asignada a leerfichas en la posicion de busqueda original*/
                guardarficha(nueva,nuevas_columnas,f,c,valor);/*guardamos las fichas en la memoria nueva con las columnas nuevas*/
            } else if (c==indice){/*si las columnas en la posicion c son iguales a la posicion donde quiere ser agregada entre al condicional*/
                guardarficha(nueva,nuevas_columnas,f,c,rand()%6);/*guardamos las fichas en la memoria nueva y generamos las fichas nuevas para esa columna nueva*/
            }else {int valor=leerfichas(memoria,columnas,f,c-1);/*guardamos en el entero valor las fichas originales de la columna*/
                guardarficha(nueva,nuevas_columnas,f,c,valor);/*guardamos las fichas en la nueva memoria que ya tiene la nueva cantidad de bytes esto con la intencion de poder
                asignar el valor correspondiente de memoria a cada uno de los casos sea un incremento para esta ocacion o un decremento en otras ocaciones */

            }
        }

    }
    delete[] memoria;/*liberamos la memoria vieja para evitar fugas de memoria */
    columnas= nuevas_columnas;/*reasignamos el nuevo valor a las columnas*/
    return nueva;/*retornamos la memoria  nueva*/
}



unsigned char* eliminar_columna(unsigned char* memoria,int filas,int& columnas,int indice,int& eliminacionesusuario){

    if (columnas<=1) return memoria;/*si las columnas son menores o iguales a uno no hay forma de eliminar columnas por ende retorna memoria */
    int nuevas_columnas =columnas-1;/*se calcula que como se va a eliminar una columna se reduce en uno y se guarda en la nueva variable nuevas_columnas*/
    int bytes = calcularbytesnecesarios(filas,nuevas_columnas);/*se calcula la cantidad de bytes que se requieren teniendo en cuenta la nueva cantidad de columnas */
    unsigned char* nueva = new unsigned char[bytes]();/*creamos un nuevo puntero para señalar en que lugar de la memoria va a quedar la nueva tabla de fichas con los cambios
                                                        pertinentes*/


    for (int f=0; f< filas;f++){/*realizamos un recorrido por las filas empezando por la primera pocision*/
        for (int c=0; c<columnas;c++){/*realizamos un recorrido columna por columna hasta haber revisado todas las columnas de la fila determinada*/
            if (c!=indice){/*si la columna es diferente de la columna que queremos eliminar entramos al condicional */
                int c_destino = (c<indice) ? c : c-1;/*se asigna al entero c_destino la columna que sea menor a la columna que queremos eliminar o si esta no cumple con ser menor
                                    que la columna que queremos eliminar entonces le resta 1, desplazando las columnas hacia la izquierda cerrando los espacios que quedan
                                    por las eliminaciones */
                int valor=leerfichas(memoria,columnas,f,c);/*el entero valor recibe la ficha despues de leerla en la memoria vieja*/
                guardarficha(nueva,nuevas_columnas,f,c_destino,valor);/*guardamos las fichas en la nueva memoria en la posicion que designamos despues de realizar la eliminacion
                                                            de la fila respectiva en c_destino*/


            }

        }

    }
    delete[] memoria;/*liberamos la memoria vieja para evitar fugas de memoria */
    columnas= nuevas_columnas;/*reasignamos el nuevo valor a las columnas*/
    eliminacionesusuario=0;/*reiniciamos la cantidad de eliminaciones*/
    return nueva;/*retornamos la memoria  nueva*/
}