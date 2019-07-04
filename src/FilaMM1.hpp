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
    //Número de estatísticas já coletadas numa rodada
    int EstatisticasColetadasTempoEspera = 0;

    //Fregues que está atualmente em serviço
    Fregues freguesEmServico = Fregues(-1);
    //Lista de fregueses presentes na fila de espera
    std::list<Fregues> FreguesesNaFilaDeEspera;
    //Heap de eventos que ocorrem na fila
    std::priority_queue<Evento> Eventos;

    FilaMM1(TipoFila tipo, double utilizacao);
    void InicializaFila();
    void TrataProximoEvento();
	void ReportaStatus();
    void ResetaEstatisticasRodada();
    double EstimadorMediaDoNumeroDePessoasNaFilaDeEspera(double tempoInicioRodada);
    double EstimadorMediaDosQuadradosDeNumerosDePessoasNaFilaDeEspera(double tempoInicioRodada);
    double EstimadorMediaTempoNaFilaDeEspera();
    double TempoMedioDeAtendimento();
    double TempoMedioDeEsperaTotal();
    double EstimadorVarianciaDoTempoNaFilaDeEspera();
    double EstimadorMediaTempoNoSistema();
    double EstimadorVarianciaTempoNoSistema();
    double EstimadorMediaTempoEmServico();
    double EstimadorVarianciaTempoNoServico();
    bool ChecaSeFilaInicializada();

    private:
    bool inicializada = false;
    double temposNaFilaDeEspera = 0.0;
    double quadradosDosTemposDeEsperaNaFila = 0.0;
    double numeroDePessoasNaFilaVezesTempo = 0.0;
    double quadradosDoNumeroDePessoasNaFilaVezesTempo = 0.0;
    double temposDeAtendimento = 0.0;
    double temposDeEsperaTotal = 0.0;
    double tempoOcupado = 0.0;

    void GeraProximaChegada();
    void GeraProximaSaida();
    void PreparaNovoServico();
    void GeraEstatisticaNumeroDePessoasNaFila(double novoTempo);
    void GeraEstatisticaTempoEsperaNaFila(Fregues fregues);
    bool FilaVazia();
    bool TemServico();
    bool FilaDeEsperaVazia();
};


#endif