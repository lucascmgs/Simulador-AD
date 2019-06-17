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
	//Valor default para a utilização/taxa de chegada (rho = lambda*E[X], mas como E[X] = 1, então rho = lambda)
    double lambda = 0.2;

	//Estatísticas
	//Estimador da média do tempo de espera na fila (centro do IC da média de W)
	double EEW = 0.0;
	//Variância da média do tempo de espera na fila (sigma^2 do IC da média de W)
	double VEW = 0.0;
	//Estimador da variância do tempo de espera na fila (centro do IC da variãncia de W)
	double EVW = 0.0;
	//Variância da variância do tempo de espera na fila (sigma^2 do IC da variãncia de W)
	double VVW = 0.0;
	//Soma das média do tempo de espera na fila de uma rodada
	double EWRodada = 0.0;
	//Soma dos quadrados das médias do tempo de espera na fila de uma rodada
	double EWRodada2 = 0.0;
	//Soma das variâncias do tempo de espera na fila de uma rodada
	double VWRodada = 0.0;
	//Soma dos quadrados das variâncias do tempo de espera na fila de uma rodada
	double VWRodada2 = 0.0;

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

	for (int i = 0; i < n; i++){
		for(int j = 0; j < k; j++){ //TODO: esse k nao é atualmente igual ao numero de estatisticas coletadas!!!
			fila.TrataProximoEvento();
		}
		//fim da rodada
		EWRodada += fila.TempoMedioDeAtendimento();
		EWRodada2 += pow(fila.TempoMedioDeAtendimento(), 2);
		VWRodada += fila.VarianciaDoTempoDeEsperaNaFila();
		VWRodada2 += pow(fila.VarianciaDoTempoDeEsperaNaFila(), 2);

		//TODO: zerar as estatísticas referentes a uma rodada ao trocar de rodada
		//TODO: determinar fim do período transiente (plotar e ver no olhômetro?)
	}
	//fim da simulação
	EEW = EWRodada/n;
	VEW = EWRodada2/(n-1) - pow(EWRodada, 2)/(n*(n-1));
	EVW = VWRodada/n;
	VVW = VWRodada2/(n-1) - pow(VWRodada, 2)/(n*(n-1));

	std::cout << "EEW: " << EEW << std::endl;
	std::cout << "VEW: " << VEW << std::endl;
	std::cout << "EVW: " << EVW << std::endl;
	std::cout << "VVW: " << VVW << std::endl;

	//IC para E[W]
	double Lower = 0.0;
	double Upper = 0.0;
	std::cout << "IC E[W]: [" << Lower << ", " << EEW << ", " << Upper << "]" << std::endl;

	//IC para V(W)
	std::cout << "IC V[W]: [" << Lower << ", " << EVW << ", " << Upper << "]" << std::endl;

}
