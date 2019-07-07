#ifndef SIMULACAO
#define SIMULACAO

#include <iostream>
#include "FilaMM1.hpp"
#include "Escritor.hpp"
#include "Rodada.hpp"
#include "FaseTransiente.hpp"
#include "GeradorAleatorio.hpp"
#include <stdlib.h>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <ctime>

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
	//Política de atendimento (0 para FCFS e 1 para LCFS)
	int PoliticaAtendimento = 0;
    
    //Estatísticas da Simulação
	//Para W
	//Estimador da média do tempo de espera na fila (centro do IC da média de W)
	double EEW = 0.0;
	//Variância da média do tempo de espera na fila (sigma^2 do IC da média de W)
	double VEW = 0.0;
	//Estimador da variância do tempo de espera na fila (centro do IC da variãncia de W)
	double EVW = 0.0;
	//Variância da variância do tempo de espera na fila (sigma^2 do IC da variãncia de W)
	double VVW = 0.0;

	//Para Nq
	//Estimador da média do numero de pessoas na fila de espera (centro do IC da média de W)
	double EENq = 0.0;
	//Variância da média do numero de pessoas na fila de espera (sigma^2 do IC da média de W)
	double VENq = 0.0;
	//Estimador da variância do numero de pessoas na fila de espera (centro do IC da variãncia de W)
	double EVNq = 0.0;
	//Variância da variância do numero de pessoas na fila de espera (sigma^2 do IC da variãncia de W)
	double VVNq = 0.0;

    //Estatísticas das Rodadas
	//Soma das média do tempo de espera na fila de uma rodada
	double EWRodadas = 0.0;
	//Soma dos quadrados das médias do tempo de espera na fila de uma rodada
	double EWRodadas2 = 0.0;
	//Soma das variâncias do tempo de espera na fila de uma rodada
	double VWRodadas = 0.0;
	//Soma dos quadrados das variâncias do tempo de espera na fila de uma rodada
	double VWRodadas2 = 0.0;
    //Soma das média do número de pessoas na fila de uma rodada
	double ENqRodadas = 0.0;
	//Soma dos quadrados das médias do número de pessoas na fila de uma rodada
	double ENqRodadas2 = 0.0;
	//Soma das variâncias do número de pessoas na fila de uma rodada
	double VNqRodadas = 0.0;
	//Soma dos quadrados das variâncias do número de pessoas na fila de uma rodada
	double VNqRodadas2 = 0.0;

	//Variáveis referentes ao cálculo dos ICs
	//Percentil da t-Student para confiança de 95% e número grande de graus de liberdade. Obtido em https://www.medcalc.org/manual/t-distribution.php
	double t = 1.960; 
	//Percentis da chi-Quadrado para confiança de 95% e 3200 - 1 = 3199 graus de liberdade. Obtido em https://keisan.casio.com/exec/system/1180573197
	double chiInferior = 3044.130201771;
	double chiSuperior = 3357.658239650;
	//Limite inferior do IC
	double Lower = 0.0;
	//Limite superior do IC
	double Upper = 0.0;

    Simulacao(int n, int k, int seed, double lambda, int politicaAtendimento);
    void RodaSimulacao();
    void GeraEstatisticaSimulacao();
    void GeraIntervalosDeConfianca();
	void ColetaEstatisticasDaSimulacao(FilaMM1 fila, tm * simTime);
	void AcumulaResultadosDaRodada(Rodada rod);
};


#endif