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
		EWRodadas += fila.EstimadorMediaTempoNaFilaDeEspera();
		EWRodadas2 += fila.EstimadorMediaTempoNaFilaDeEspera()*fila.EstimadorMediaTempoNaFilaDeEspera();
		VWRodadas += fila.EstimadorVarianciaDoTempoNaFilaDeEspera();
		VWRodadas2 += pow(fila.EstimadorVarianciaDoTempoNaFilaDeEspera(), 2);

		//std::cout << "---- FIM DA RODADA " << i << " ----" << std::endl;
		//std::cout << "est coletadas: " << fila.EstatisticasColetadasTempoEspera << std::endl;
		//std::cout << "EWRodada: " << EWRodadas << std::endl;
		//std::cout << "EWRodada2: " << EWRodadas2 << std::endl;
		//std::cout << "VWRodada: " << VWRodadas << std::endl;
		//std::cout << "VWRodada2: " << VWRodadas2 << std::endl;
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
	EEW = EWRodadas/n;
	VEW = EWRodadas2/(n-1) - pow(EWRodadas, 2)/(n*(n-1));
	EVW = VWRodadas/n;
	VVW = VWRodadas2/(n-1) - pow(VWRodadas, 2)/(n*(n-1));
    
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

	double precisao = (Upper-Lower)/(Upper+Lower);

	std::cout << "IC E[W]: [" << Lower << ", " << EEW << ", " << Upper << " | Precisão: "<< precisao <<"]" << std::endl;

    //Para V(W)
	Lower = EVW - t * sqrt(VVW)/sqrt(n);
	Upper = EVW + t * sqrt(VVW)/sqrt(n);

	precisao = (Upper-Lower)/(Upper+Lower);

	std::cout << "IC V[W]: [" << Lower << ", " << EVW << ", " << Upper << " | Precisão: "<< precisao <<"]" << std::endl;   

    //TODO: Para E[Nq] 

    //TODO: Para V(Nq) 

}