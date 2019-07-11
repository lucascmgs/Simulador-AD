#include "string.h"
#include <stdlib.h>
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
	//Valor default para a semente (utiliza o horário atual do sistema)
	int seed = time(NULL);
	//Valor default para a utilização/taxa de chegada (rho = lambda*E[X], mas como E[X] = 1, então rho = lambda)
    double rho = 0.2;
	//Valor default para a política de atendimento (0 para FCFS, 1 para LCFS)
    int politicaAtendimento = 0;

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
				seed = atoi(argv[i + 1]);
			}
			if (strcmp(argv[i], "-rho") == 0) {
				rho = atof(argv[i + 1]);
			}
			if (strcmp(argv[i], "-politica") == 0) {
				politicaAtendimento = atof(argv[i + 1]);
				if (politicaAtendimento != 0 && politicaAtendimento != 1) {
					std::cout << "Parâmetro política de atendimento passado incorretamente! Use 0 para FCFS ou 1 para LCFS." << std::endl;
					abort();
				}
			}
		}
	}
	Simulacao sim = Simulacao(n, k, seed, rho, politicaAtendimento);
	sim.RodaSimulacao();
}
