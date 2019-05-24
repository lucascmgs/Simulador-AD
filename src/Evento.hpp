#ifndef EVENTO
#define EVENTO
#include "Fregues.hpp"

enum class TipoEvento{
    CHEGADA, SAIDA
};

class Evento {
    public:
    double TempoOcorrencia;
    TipoEvento Tipo;

    Evento(double tempo, TipoEvento tipoEvento);
    friend bool operator<(Evento evento1, Evento evento2);
};

#endif