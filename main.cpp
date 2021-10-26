#include "Blocking_queue_template.h"
#include <iostream>
int main(){
    Cola<int> cola_int;
    int numero=10;
    cola_int.encolar(numero);
    int resultado = cola_int.desencolar();
    return 0;
}
