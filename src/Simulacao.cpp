#include "Simulacao.hpp"
#include "Rodada.hpp"
#include "Escritor.hpp"
#include "FilaMM1.hpp"

Simulacao::Simulacao(int n, int k, int seed, double lambda) {
    this->n = n;
    this->k = k;
    this->Seed = seed;
    this->Lambda = lambda;
}

void Simulacao::RodaSimulacao() {
    GeradorAleatorio::Inicializa(Seed);

	Rodada rod = Rodada();
    FilaMM1 fila = FilaMM1(TipoFila::FCFS, Lambda);
	fila.InicializaFila();

	for (int i = 0; i < this->n; i++){
		while(fila.EstatisticasColetadas < k) {
			fila.TrataProximoEvento();
		}
		//TODO: Criar Rodada.hpp e Rodada.cpp pra isolar lógica e estatísticas da rodada

		//fim da rodada
		rod.ColetaResultadosDaRodada(fila.TempoMedioDeEsperaNaFila(),fila.VarianciaDoTempoDeEsperaNaFila());
		fila.ResetaEstatisticasRodada();

		//TODO: determinar fim do período transiente (plotar e ver no olhômetro?)
	}
	//fim da simulação
	std::cout << "---- FIM DA SIMULACAO " << " ----" << std::endl;
    GeraEstatisticaSimulacao(rod);
    GeraIntervaloDeConfianca();
}

void Simulacao::GeraEstatisticaSimulacao(Rodada rod) {
	Escritor esc = Escritor();
    //Para W
	EEW = rod.EWRodada/n;
	VEW = rod.EWRodada2/(n-1) - pow(rod.EWRodada, 2)/(n*(n-1));
	EVW = rod.VWRodada/n;
	VVW = rod.VWRodada2/(n-1) - pow(rod.VWRodada, 2)/(n*(n-1));

    std::vector<string> linha (1);
    linha.at(0) = "VA,EEW,VEW,EVW,VVW";
    esc.EscreveCabecalhoEmCSV(1, linha);
	std::vector<double> valores (5);
    valores.at(0) = Lambda/(1-Lambda); valores.at(1) = EEW; valores.at(2) = VEW; valores.at(3) = EVW; valores.at(4) = VVW;
	esc.EscreveLinhaEmCSV(5, valores);
	
    
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
	std::cout << "IC E[W]: [" << Lower << ", " << EEW << ", " << Upper << "]" << std::endl;

    //Para V(W)
	Lower = EVW - t * sqrt(VVW)/sqrt(n);
	Upper = EVW + t * sqrt(VVW)/sqrt(n);
	std::cout << "IC V[W]: [" << Lower << ", " << EVW << ", " << Upper << "]" << std::endl;   

    //TODO: Para E[Nq] 

    //TODO: Para V(Nq) 

}