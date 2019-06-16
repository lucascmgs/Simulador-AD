#ifndef EVENTO
#define EVENTO
#include<iostream>

//Temos dois tipos de eventos: A chegada na fila e a saída da fila
enum class TipoEvento{
    CHEGADA, SAIDA
};

class Evento {
    public:
    //Tempo em que o evento ocorreu
    double TempoOcorrencia;
    TipoEvento Tipo;

    Evento(TipoEvento tipoEvento, double tempo);
    //Operador para definir qual evento ocorre antes, usado para ordenação da heap de eventos
    friend bool operator<(Evento evento1, Evento evento2);
    friend std::ostream& operator<<(std::ostream& os, const Evento& evento);
};

#endif