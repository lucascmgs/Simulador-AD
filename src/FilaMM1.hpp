#ifndef FILAMM1
#define FILAMM1
#include<list>
#include<queue>
#include<iostream>
#include "Fregues.hpp"
#include "Evento.hpp"
#include "GeradorAleatorio.hpp"


enum class TipoFila {
    FCFS, LCFS
};

class FilaMM1{
    public:
    TipoFila Tipo;
    double Utilizacao;    
    double TempoAtual;
    std::list<Fregues> Fregueses;
    std::priority_queue<Evento> Eventos;

    FilaMM1(TipoFila tipo, double utilizacao);
    void InicializaFila();
    void TrataProximoEvento();
    void GeraProximaChegada();
    void GeraProximaSaida();
    void GeraEstatistica(Fregues fregues);
	void ReportaStatus();
};


#endif