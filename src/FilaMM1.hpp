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

    //Fregues que está atualmente em serviço
    Fregues freguesEmServico = Fregues(-1);
    //Lista de fregueses presentes na fila de espera
    std::list<Fregues> Fregueses;
    //Heap de eventos que ocorrem na fila
    std::priority_queue<Evento> Eventos;

    FilaMM1(TipoFila tipo, double utilizacao);
    void InicializaFila();
    void TrataProximoEvento();
	void ReportaStatus();
    double TempoMedioDeEsperaNaFila();
    double TempoMedioDeAtendimento();
    double TempoMedioDeEsperaTotal();
    double VarianciaDoTempoDeEsperaNaFila();

    private:
    double temposDeEsperaNaFila = 0.0;
    double quadradosDosTemposDeEsperaNaFila = 0.0;
    double temposDeAtendimento = 0.0;
    double temposDeEsperaTotal = 0.0;
    double tempoOcupado = 0.0;
    int quantidadeSaidas = 0;

    void GeraProximaChegada();
    void GeraProximaSaida();
    void PreparaNovoServico();
    void GeraEstatistica(Fregues fregues);
    bool FilaVazia();
    bool TemServico();
    bool FilaDeEsperaVazia();

};


#endif