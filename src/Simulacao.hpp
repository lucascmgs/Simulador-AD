#ifndef SIMULACAO
#define SIMULACAO
#include <iostream>
#include "GeradorAleatorio.hpp"

class Simulacao {
    public:
    //Inputs para a simulação
    //Número de rodadas de simulação
	int n = 0;
	//Número de coletas feitas por rodada
	int k = 0;
	//Valor da semente
	int Seed = 0;
	//Valor da utilização/taxa de chegada
    double Lambda = 0;
    
    //Estatísticas da Simulação
	//Estimador da média do tempo de espera na fila (centro do IC da média de W)
	double EEW = 0.0;
	//Variância da média do tempo de espera na fila (sigma^2 do IC da média de W)
	double VEW = 0.0;
	//Estimador da variância do tempo de espera na fila (centro do IC da variãncia de W)
	double EVW = 0.0;
	//Variância da variância do tempo de espera na fila (sigma^2 do IC da variãncia de W)
	double VVW = 0.0;

    //Estatísticas da Rodada
	//Soma das média do tempo de espera na fila de uma rodada
	double EWRodada = 0.0;
	//Soma dos quadrados das médias do tempo de espera na fila de uma rodada
	double EWRodada2 = 0.0;
	//Soma das variâncias do tempo de espera na fila de uma rodada
	double VWRodada = 0.0;
	//Soma dos quadrados das variâncias do tempo de espera na fila de uma rodada
	double VWRodada2 = 0.0;
    //Soma das média do número de pessoas na fila de uma rodada
	double ENqRodada = 0.0;
	//Soma dos quadrados das médias do número de pessoas na fila de uma rodada
	double ENqRodada2 = 0.0;
	//Soma das variâncias do número de pessoas na fila de uma rodada
	double VNqRodada = 0.0;
	//Soma dos quadrados das variâncias do número de pessoas na fila de uma rodada
	double VNqRodada2 = 0.0;

	//Variáveis referentes ao cálculo dos ICs
	//t-Student para precisão de 5% e n-1 graus de liberdade
	double t = 0.0; 
	//Limite inferior do IC
	double Lower = 0.0;
	//Limite superior do IC
	double Upper = 0.0;

    Simulacao(int n, int k, int seed, double lambda);
    void RodaSimulacao();
    void GeraEstatisticaSimulacao();
    void GeraIntervaloDeConfianca();
};


#endif