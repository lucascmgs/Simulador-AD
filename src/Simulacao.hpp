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
	//Vetor com referência para as rodadas da simulação
	std::vector<Rodada> rodadas;

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
	//Nome do arquivo de saída com resultados
	std::string Output;
    
    //Estatísticas da Simulação
	//Para W
	//Estimador da média do tempo de espera na fila (centro do IC da média de W)
	double Media_W = 0.0;
	//Variância da média do tempo de espera na fila (sigma^2 do IC da média de W)
	double Variancia_Medias_W = 0.0;
	//Estimador da variância do tempo de espera na fila (centro do IC da variãncia de W)
	double Variancia_W = 0.0;
	//Variância da variância do tempo de espera na fila (sigma^2 do IC da variãncia de W)
	double Variancia_Variancias_W = 0.0;

	//Para Nq
	//Estimador da média do numero de pessoas na fila de espera (centro do IC da média de W)
	double Media_Nq = 0.0;
	//Variância da média do numero de pessoas na fila de espera (sigma^2 do IC da média de W)
	double Variancia_Medias_Nq = 0.0;
	//Estimador da variância do numero de pessoas na fila de espera (centro do IC da variãncia de W)
	double Variancia_Nq = 0.0;
	//Variância da variância do numero de pessoas na fila de espera (sigma^2 do IC da variãncia de W)
	double Variancia_Variancias_Nq = 0.0;

	//Variáveis referentes ao cálculo dos ICs
	//Percentil da t-Student para confiança de 95% e número grande de graus de liberdade. Obtido em https://www.medcalc.org/manual/t-distribution.php
	double t = 1.960; 
	//Percentis da chi-Quadrado para confiança de 95% e 3200 - 1 = 3199 graus de liberdade. Obtido em https://keisan.casio.com/exec/system/1180573197
	double chiInferior = 3044.130201771;
	double chiSuperior = 3357.658239650;

	//Variáveis referentes ao cálculo dos ICs
	//Para t-Student, IC de E[W]
	double Lower_t_EW, Upper_t_EW, precisao_t_EW;
	//Para chi-square, IC de V[W]
	double Lower_chi_VW, Upper_chi_VW, precisao_chi_VW, centroChi_VW;
	//Para t-Student, IC de V[W]
	double Lower_t_VW, Upper_t_VW, precisao_t_VW;
	//Para t-Student, IC de E[Nq]
	double Lower_t_ENq, Upper_t_ENq, precisao_t_ENq;
	//Para chi-square, IC de V(Nq)
	double Lower_chi_VNq, Upper_chi_VNq, precisao_chi_VNq, centroChi_VNq;
	//Para t-Student, IC de V(Nq)
	double Lower_t_VNq, Upper_t_VNq, precisao_t_Vnq;

	//Valores Analíticos
	double EWAnalitico, VWAnaliticoFCFS, VWAnaliticoLCFS, ENqAnalitico, VNqAnalitico;
	
    Simulacao(int n, int k, int seed, double lambda, int politicaAtendimento, std::string output);
    void RodaSimulacao();
    void GeraEstatisticaSimulacao();
    void GeraIntervalosDeConfianca();
	void ColetaEstatisticasDaSimulacao(FilaMM1 fila, tm * simTime);
};


#endif