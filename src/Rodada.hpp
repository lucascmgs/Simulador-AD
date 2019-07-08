#ifndef RODADA
#define RODADA

#include "FilaMM1.hpp"
#include "Escritor.hpp"
#include <fstream>
#include <vector>


class Rodada{
    public:
    //numero da rodada atual
    int Indice;

	//Tamanho da rodada
	int Tamanho;

	//Tempo de início da rodada
	double TempoInicio;

	FilaMM1* Fila;
    //Estatísticas da Rodada
	//Soma das média do tempo de espera na fila da rodada
	double EWRodada = 0.0;
	//Soma das variâncias do tempo de espera na fila da rodada
	double VWRodada = 0.0;
    //Soma das média do número de pessoas na fila da rodada
	double ENqRodada = 0.0;
	//Soma das variâncias do número de pessoas na fila da rodada
	double VNqRodada = 0.0;
	
    Rodada(int indiceRodada, int tamanhoRodada, FilaMM1 *fila);
	void RealizaRodada();
    void ColetaResultadosDaRodada();
};
#endif