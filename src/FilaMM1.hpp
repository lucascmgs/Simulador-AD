#ifndef FILAMM1
#define FILAMM1
#include<list>
#include<queue>
#include<iostream>
#include "Fregues.hpp"
#include "Evento.hpp"
#include "GeradorAleatorio.hpp"

//Atribuimos um Tipo à fila correspondente à sua política de atendimento
enum class TipoFila {
    FCFS, LCFS
};

class FilaMM1{
    public:
    TipoFila Tipo;
    //Taxa de utilização da fila, igual a λ*E[X], ou seja, ρ 
    double Utilizacao;
    //Variável que mantém o tempo da fila    
    double TempoAtual;
    //Lista de fregueses presentes na fila
    std::list<Fregues> Fregueses;
    //Heap de eventos que ocorrem na fila
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