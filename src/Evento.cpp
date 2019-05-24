#include "Evento.hpp"

Evento::Evento(double tempo, TipoEvento tipoEvento){
    this->TempoOcorrencia = tempo;
    this->Tipo = tipoEvento;
}

bool operator<(const Evento evento1, const Evento evento2){
    return evento1.TempoOcorrencia < evento2.TempoOcorrencia;
}