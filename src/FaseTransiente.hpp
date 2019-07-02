#ifndef FASETRANSIENTE
#define FASETRANSIENTE
#include "FilaMM1.hpp"

class FaseTransiente {
    public:
    FilaMM1 *Fila;

    FaseTransiente(FilaMM1 *fila);
    void RodaFaseTransiente();
    void TerminaFaseTransiente();
};



#endif