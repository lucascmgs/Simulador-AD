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
		while(fila.EstatisticasColetadasTempoEspera < k) {
			fila.TrataProximoEvento();
		}
		//TODO: Criar Rodada.hpp e Rodada.cpp pra isolar lógica e estatísticas da rodada

		//fim da rodada
		EWRodada += fila.EstimadorMediaTempoNaFilaDeEspera();
		EWRodada2 += fila.EstimadorMediaTempoNaFilaDeEspera()*fila.EstimadorMediaTempoNaFilaDeEspera();
		VWRodada += fila.EstimadorVarianciaDoTempoNaFilaDeEspera();
		VWRodada2 += pow(fila.EstimadorVarianciaDoTempoNaFilaDeEspera(), 2);

		std::cout << "---- FIM DA RODADA " << i << " ----" << std::endl;
		std::cout << "est coletadas: " << fila.EstatisticasColetadasTempoEspera << std::endl;
		std::cout << "EWRodada: " << EWRodada << std::endl;
		std::cout << "EWRodada2: " << EWRodada2 << std::endl;
		std::cout << "VWRodada: " << VWRodada << std::endl;
		std::cout << "VWRodada2: " << VWRodada2 << std::endl;
		fila.ResetaEstatisticasRodada();

		//TODO: determinar fim do período transiente (plotar e ver no olhômetro?)
	}
	//fim da simulação
	std::cout << "---- FIM DA SIMULACAO " << " ----" << std::endl;
    GeraEstatisticaSimulacao();
    GeraIntervaloDeConfianca();
}

void Simulacao::GeraEstatisticaSimulacao() {
    //Para W
	EEW = EWRodada/n;
	VEW = EWRodada2/(n-1) - pow(EWRodada, 2)/(n*(n-1));
	EVW = VWRodada/n;
	VVW = VWRodada2/(n-1) - pow(VWRodada, 2)/(n*(n-1));
    
	std::cout << "Valor Analítico para EEW: " << Lambda/(1-Lambda) << std::endl;
	std::cout << "EEW estimado: " << EEW << std::endl;
	std::cout << "VEW: " << VEW << std::endl;
	std::cout << "EVW: " << EVW << std::endl;
	std::cout << "VVW: " << VVW << std::endl;

    //TODO: Para Nq
}

void Simulacao::GeraIntervaloDeConfianca() {
    //Para E[W]
	Lower = EEW - t * sqrt(VEW)/sqrt(n);
	Upper = EEW + t * sqrt(VEW)/sqrt(n);
	// std::cout << "IC E[W]: [" << Lower << ", " << EEW << ", " << Upper << "]" << std::endl;

    //Para V(W)
	Lower = EVW - t * sqrt(VVW)/sqrt(n);
	Upper = EVW + t * sqrt(VVW)/sqrt(n);
	// std::cout << "IC V[W]: [" << Lower << ", " << EVW << ", " << Upper << "]" << std::endl;   

    //TODO: Para E[Nq] 

    //TODO: Para V(Nq) 

}