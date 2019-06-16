#include "string.h"
#include <iostream>
#include "FilaMM1.hpp"
#include "GeradorAleatorio.hpp"

int main(int argc, char* argv[])
{
	//Número default de rodadas de simulação
	int n = 3200;
	//Número default de coletas feitas (k coletas = 1 rodada de simulação)
	int k = 100;
	//Valor default para a semente
	int seed = time(NULL);
	//Valor default para a taxa de chegada
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
<<<<<<< HEAD
	
	FilaMM1 fila(TipoFila::FCFS, lambda);
=======
    

	FilaMM1 fila = FilaMM1(TipoFila::FCFS, lambda);

>>>>>>> 00066ac3e218aa5e0fbab121841f36f068d5d524
	fila.InicializaFila();


	for(int i = 0; i < k; i++){
	 	fila.TrataProximoEvento();
	}

	std::cout << fila.TempoMedioDeEsperaNaFila() << std::endl;
	std::cout << fila.TempoMedioDeAtendimento() << std::endl;
	std::cout << fila.TempoMedioDeEsperaTotal() << std::endl;
}

// //duas listas
// //1: dura a simulação inteira.
// //2: dura uma rodada. no fim da rodada calculo E[wi], coloco E[wi] na lista da simulação e esvazio essa lista 2.

// void geraEstatisticaRodada () {
// 	//Estimador da média da rodada i
// 	//E[wi] = (wi0 + wi1 + wi2 + ... + win)/n
// 	EWi = 

// 	//Estimador da variância da rodada i
// 	//V(wi) = ((wi0 - E[w0]) + (wi1 - E[w0]) + (wi2 - E[w0]) + ... + (win - E[w0]))/(n-1)
// }

// void geraEstatisticaSimulacao() {
// 	//E[W] = (E[w0] + E[w1] + E[w2] + ... + E[wk])/k
// 	for (size_t i = 0; i < k; i++) {

// 	}

// 	//V(W) = ((w0 - E[W]) + (w1 - E[W]) + ... + (wk - E[W]))/(k-1)
// }

