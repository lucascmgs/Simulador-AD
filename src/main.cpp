#include "string.h"
#include <iostream>
#include "FilaMM1.hpp"
#include "GeradorAleatorio.hpp"
#include "Simulacao.hpp"

int main(int argc, char* argv[])
{
	//Número default de rodadas de simulação
	int n = 3200;
	//Número default de coletas feitas (k coletas = 1 rodada de simulação)
	int k = 100;
	//Valor default para a semente
	int seed = time(NULL);
	//Valor default para a utilização/taxa de chegada (rho = lambda*E[X], mas como E[X] = 1, então rho = lambda)
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
	Simulacao sim = Simulacao(n, k, seed, lambda);
	sim.RodaSimulacao();
}
