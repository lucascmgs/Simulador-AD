#include "Evento.hpp"

//Construtor da classe Evento
Evento::Evento(TipoEvento tipoEvento, double tempo){
    this->TempoOcorrencia = tempo;
    this->Tipo = tipoEvento;
}

//Operador para ordenação na heap de eventos. Como queremos que seja uma heap de mínimo, invertemos a função do <, que retorna 
//verdadeiro se o segundo evento ocorre antes que o primeiro
bool operator<(const Evento evento1, const Evento evento2){
    return evento2.TempoOcorrencia < evento1.TempoOcorrencia;
}