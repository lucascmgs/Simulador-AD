#include "Simulacao.hpp"

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

		Rodada rod = Rodada(i, this->k, &fila);
		
		rod.RealizaRodada();
		rod.ColetaResultadosDaRodada();
		this->AcumulaResultadosDaRodada(rod);

	}
	//fim da simulação
	std::cout << "---- FIM DA SIMULACAO " << " ----" << std::endl;
    GeraEstatisticaSimulacao();
    GeraIntervaloDeConfianca();
}

void Simulacao::AcumulaResultadosDaRodada(Rodada rod){
	this->EWRodadas += rod.EWRodada;
	this->EWRodadas2 += rod.EWRodada2;
	this->VWRodadas += rod.VWRodada;
	this->VWRodadas2 += rod.VWRodada2;
}


void Simulacao::GeraEstatisticaSimulacao() {
	Escritor esc = Escritor();
    //Para W

	EEW = this->EWRodadas/n;
	VEW = this->EWRodadas2/(n-1) - pow(this->EWRodadas, 2)/(n*(n-1));
	EVW = this->VWRodadas/n;
	VVW = this->VWRodadas2/(n-1) - pow(this->VWRodadas, 2)/(n*(n-1));

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