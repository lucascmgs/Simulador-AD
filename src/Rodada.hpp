#ifndef RODADA
#define RODADA

#include "FilaMM1.hpp"
#include "Escritor.hpp"
#include <fstream>
#include <vector>
using namespace std;

class Rodada{
    public:
    //numero da rodada atual
    int rodada;

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
	
    Rodada();
    void ColetaResultadosDaRodada(double TempoMedioDeEsperaNaFila, double VarianciaDoTempoDeEsperaNaFila);
};
#endif