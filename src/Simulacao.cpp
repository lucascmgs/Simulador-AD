#include "Simulacao.hpp"
#include "FilaMM1.hpp"

Simulacao::Simulacao(int n, int k, int seed, double lambda) {
    this->n = n;
    this->k = k;
    this->Seed = seed;
    this->Lambda = lambda;
}

void Simulacao::RodaSimulacao() {
    GeradorAleatorio::Inicializa(Seed);

    FilaMM1 fila = FilaMM1(TipoFila::FCFS, Lambda);
	fila.InicializaFila();

	for (int i = 0; i < this->n; i++){
		for(int j = 0; j < this->k; j++){ //TODO: esse k nao é atualmente igual ao numero de estatisticas coletadas!!!
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
    GeraEstatisticaSimulacao();
    GeraIntervaloDeConfianca();
}

void Simulacao::GeraEstatisticaSimulacao() {
    //Para W
	EEW = EWRodada/n;
	VEW = EWRodada2/(n-1) - pow(EWRodada, 2)/(n*(n-1));
	EVW = VWRodada/n;
	VVW = VWRodada2/(n-1) - pow(VWRodada, 2)/(n*(n-1));
    
	std::cout << "EEW: " << EEW << std::endl;
	std::cout << "VEW: " << VEW << std::endl;
	std::cout << "EVW: " << EVW << std::endl;
	std::cout << "VVW: " << VVW << std::endl;

    //TODO: Para Nq
}

void Simulacao::GeraIntervaloDeConfianca() {
    //Para E[W]
	Lower = EEW - t * sqrt(VEW)/sqrt(n);
	Upper = EEW + t * sqrt(VEW)/sqrt(n);
	std::cout << "IC E[W]: [" << Lower << ", " << EEW << ", " << Upper << "]" << std::endl;

    //Para V(W)
	Lower = EVW - t * sqrt(VVW)/sqrt(n);
	Upper = EVW + t * sqrt(VVW)/sqrt(n);
	std::cout << "IC V[W]: [" << Lower << ", " << EVW << ", " << Upper << "]" << std::endl;   

    //TODO: Para E[Nq] 

    //TODO: Para V(Nq) 

}