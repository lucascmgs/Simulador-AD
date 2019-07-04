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

	//Tempos coletados em uma simulação, para fins de corretude
	//Tempo médio de permanência no sistema em uma simulação
	double Tbarra = 0.0;
	//Tempo médio de espera na fila em uma simulação
	double Wbarra = 0.0;
	//Tempo médio de serviço em uma simulação
	double Xbarra = 0.0;
	//Variância do tempo médio de permanência no sistema em uma simulação
	double VTbarra = 0.0;
	//Variância do tempo médio de espera na fila em uma simulação
	double VWbarra = 0.0;
	//Variância do tempo médio de serviço em uma simulação
	double VXbarra = 0.0;

	//Tempos coletados em uma rodada
	//Tempo acumulado de permanência no sistema em uma rodada
	double Trodadas = 0.0;
	//Tempo acumulado de permanência na fila de espera em uma rodada
	double Wrodadas = 0.0;
	//Tempo acumulado em serviço em uma rodada
	double Xrodadas = 0.0;
	//Variância do tempo acumulado de permanência no sistema em uma rodada
	double VTrodadas = 0.0;
	//Variância do tempo acumulado de permanência na fila de espera em uma rodada
	double VWrodadas = 0.0;
	//Variância do tempo acumulado em serviço em uma rodada
	double VXrodadas = 0.0;

	//Variáveis referentes ao cálculo dos ICs
	//Percentil da t-Student para confiança de 95% e número grande de graus de liberdade. Obtido em https://www.medcalc.org/manual/t-distribution.php
	double t = 1.960; 
	double chiInferior = 3044.130201771;
	double chiSuperior = 3357.658239650;
	//Limite inferior do IC
	double Lower = 0.0;
	//Limite superior do IC
	double Upper = 0.0;

    Simulacao(int n, int k, int seed, double lambda);
    void RodaSimulacao();
    void GeraEstatisticaSimulacao();
    void GeraIntervaloDeConfianca();
	void ColetaEstatisticasDaSimulacao(FilaMM1 fila, tm * simTime);
	void AcumulaResultadosDaRodada(Rodada rod);
};


#endif