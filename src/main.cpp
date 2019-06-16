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
    

	FilaMM1 fila = FilaMM1(TipoFila::FCFS, lambda);

	fila.InicializaFila();


	for(int i = 0; i < k; i++){
	 	fila.TrataProximoEvento();
	}

	std::cout << fila.TempoMedioDeEsperaNaFila() << std::endl;
	std::cout << fila.TempoMedioDeAtendimento() << std::endl;
	std::cout << fila.TempoMedioDeEsperaTotal() << std::endl;
}

