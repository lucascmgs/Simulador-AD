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
	//Inicializamos a fila adicionando um primeiro evento
	fila.InicializaFila();

	FaseTransiente trans = FaseTransiente(&fila);
	trans.RodaFaseTransiente();

	for (int i = 0; i < this->n; i++){

		Rodada rod = Rodada(i, this->k, &fila);
		
		//Realizamos uma rodada da simulação até o fim
		rod.RealizaRodada();
		//Coletamos os resultados da rodada e resetamos os valores das métricas na fila
		rod.ColetaResultadosDaRodada();
		//Acumulamos os resultados da rodada em nossa simulação
		this->AcumulaResultadosDaRodada(rod);

	}
	//fim da simulação
	std::cout << "---- FIM DA SIMULACAO " << " ----" << std::endl;
    GeraEstatisticaSimulacao();
    GeraIntervalosDeConfianca();
}

void Simulacao::AcumulaResultadosDaRodada(Rodada rod){
	//Para W
	this->EWRodadas += rod.EWRodada;
	this->EWRodadas2 += rod.EWRodada2;
	this->VWRodadas += rod.VWRodada;
	this->VWRodadas2 += rod.VWRodada2;
	
	//Para Nq
	this->ENqRodadas += rod.ENqRodada;
	this->ENqRodadas2 += rod.ENqRodada2;
	this->VNqRodadas += rod.VNqRodada;
	this->VNqRodadas2 += rod.VNqRodada2;
}


void Simulacao::GeraEstatisticaSimulacao() {
	Escritor esc = Escritor();
    //Para W

	EEW = this->EWRodadas/n;
	VEW = this->EWRodadas2/(n-1) - pow(this->EWRodadas, 2)/(n*(n-1));
	EVW = this->VWRodadas/n;
	VVW = this->VWRodadas2/(n-1) - pow(this->VWRodadas, 2)/(n*(n-1));

	//Para Nq

	EENq = this->ENqRodadas/n;
	VENq = this->ENqRodadas2/(n-1) - pow(this->ENqRodadas, 2)/(n*(n-1));
	EVNq = this->VNqRodadas/n;
	VVNq = this->VNqRodadas2/(n-1) - pow(this->VNqRodadas, 2)/(n*(n-1));

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

void Simulacao::GeraIntervalosDeConfianca() {
    //Para E[W]
	Lower = EEW - t * sqrt(VEW)/sqrt(n);
	Upper = EEW + t * sqrt(VEW)/sqrt(n);

	double precisao = (Upper-Lower)/(Upper+Lower);

	std::cout << "IC E[W]: [" << Lower << ", " << EEW << ", " << Upper << " | Precisão: "<< precisao <<"]" << std::endl;

	//Para V(W), chi-quadrado
	Lower = (k * (n-1)*VEW)/chiSuperior;
	Upper = (k * (n-1)*VEW)/chiInferior;

	precisao = (Upper-Lower)/(Upper+Lower);

	std::cout << "IC V(W) chi-quadrado: [" << Lower << ", " << VEW*k << ", " << Upper << " | Precisão: "<< precisao <<"]" << std::endl;


    //Para V(W), t-student
	Lower = EVW - t * sqrt(VVW)/sqrt(n);
	Upper = EVW + t * sqrt(VVW)/sqrt(n);

	precisao = (Upper-Lower)/(Upper+Lower);

	std::cout << "IC V(W) t-Student: [" << Lower << ", " << EVW << ", " << Upper << "] |Precisão: "<< precisao <<"|" << std::endl;   

    //Para E[Nq] 
	Lower = EENq - t*sqrt(VENq)/sqrt(n);
	Upper = EENq + t*sqrt(VENq)/sqrt(n);

	precisao = (Upper-Lower)/(Upper+Lower);

	std::cout << "IC E[Nq]: [" << Lower << ", " << EENq << ", " << Upper << " ] |Precisão: "<< precisao << "|" << std::endl;

	//Para V(Nq), chi-quadrado

	Lower = (k * (n-1)*VENq)/chiSuperior;
	Upper = (k * (n-1)*VENq)/chiInferior;

	precisao = (Upper-Lower)/(Upper+Lower);

	std::cout << "IC V(Nq) chi-quadrado: [" << Lower << ", " << VENq*k << ", " << Upper << " | Precisão: "<< precisao <<"]" << std::endl;


    //Para V(Nq), t-student	
	Lower = EVNq - t * sqrt(VVNq)/sqrt(n);
	Upper = EVNq + t * sqrt(VVNq)/sqrt(n);

	precisao = (Upper-Lower)/(Upper+Lower);

	std::cout << "IC V(Nq) t-Student: [" << Lower << ", " << EVNq << ", " << Upper << "] |Precisão: "<< precisao <<"|" << std::endl;   


}