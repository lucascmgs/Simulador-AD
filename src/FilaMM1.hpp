#ifndef FILAMM1
#define FILAMM1
#include<list>
#include<queue>
#include "Fregues.hpp"
#include "Evento.hpp"


enum class TipoFila {
    FCFS, LCFS
};

class FilaMM1{
    TipoFila Tipo;
    double Utilizacao;
    double TempoAtual;
    std::list<Fregues> Fregueses;
    std::priority_queue<Evento> Eventos;
    FilaMM1(TipoFila tipo, double utilizacao);
};


#endif