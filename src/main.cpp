#include <string.h>
#include <iostream>
#include "FilaMM1.hpp"
#include "GeradorAleatorio.hpp"

int main(int argc, char* argv[])
{
	int n = 3200;
	int k = 100;
	int seed = time(NULL);
    double lambda = 0.2;
    
    //Permite testar o simulador com diferentes parâmetros
	for (int i = 1; i < argc; i++) {
		if (i < argc - 1) {
			if (strcmp(argv[i], "-n") == 0) {
				n = atoi(argv[i + 1]);
			}
			if (strcmp(argv[i], "-k") == 0) {
				k = atoi(argv[i + 1]);
			}
            if (strcmp(argv[i], "-seed") == 0) {
				lambda = atoi(argv[i + 1]);
			}
			if (strcmp(argv[i], "-lambda") == 0) {
				lambda = atof(argv[i + 1]);
			}
		}
	}

    GeradorAleatorio::Inicializa(seed);
    


	
	
	FilaMM1 fila(TipoFila::FCFS, lambda);
	fila.InicializaFila();
	int iVazio;

	double tempoMedioEspera = 0.0;

	double vazio;
	double vazioTotal = 0.0;

	double numPessoas = 0.0;
	double numPessoasTotal = 0.0;

	for (size_t i = 0; i < k; i++)
	{
		if(fila.Fregueses.size() == 0){
			iVazio = i;
			vazio = fila.TempoAtual;
		}

		if(fila.Fregueses.size() > 0){
			if(i - iVazio == 1){
				vazioTotal += fila.TempoAtual - vazio;
			}
		}

		numPessoas = fila.Fregueses.size();

		double tempo = fila.TempoAtual;
		fila.TrataProximoEvento();

		double novoTempo = fila.TempoAtual;

		numPessoasTotal += numPessoas * (novoTempo - tempo);
		
	}

	numPessoasTotal = numPessoasTotal /fila.TempoAtual;
	double ocupado = 1 - vazioTotal/fila.TempoAtual;

	std::cout<< numPessoasTotal << std::endl;
	std::cout<< ocupado << std::endl;
}

