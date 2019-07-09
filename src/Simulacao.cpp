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

	time_t tempoAtual = time(0);
	tm * tempoDaSimulacao = localtime(&tempoAtual);

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
	std::cout << "PARAMETROS DA SIMULAÇÃO: n = " << this->n << "; k = " << this->k << "; LAMBDA = " << this->Lambda << std::endl;
    GeraEstatisticaSimulacao();
    GeraIntervaloDeConfianca();
	ColetaEstatisticasDaSimulacao(fila, tempoDaSimulacao);
}

void Simulacao::AcumulaResultadosDaRodada(Rodada rod){
	this->EWRodadas += rod.EWRodada;
	this->EWRodadas2 += rod.EWRodada2;
	this->VWRodadas += rod.VWRodada;
	this->VWRodadas2 += rod.VWRodada2;
}


void Simulacao::GeraEstatisticaSimulacao() {
    //Para W

	EEW = this->EWRodadas/n;
	VEW = this->EWRodadas2/(n-1) - pow(this->EWRodadas, 2)/(n*(n-1));
	EVW = this->VWRodadas/n;
	VVW = this->VWRodadas2/(n-1) - pow(this->VWRodadas, 2)/(n*(n-1)); 
	
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

void Simulacao::ColetaEstatisticasDaSimulacao(FilaMM1 fila, tm * simTime) {
	Escritor esc = Escritor();
	fstream file;
	file.open("results.csv");
	if(file.fail()){
		std::vector<string> linha (1);
		linha.at(0) = "Timestamp,Utilizacao,Politica,ICMediaTempoEspera,ICVarianciaTempoEspera(t-student),ICVarianciaTempoEspera(chi-square),ICMediaPessoas,ICVarianciaPessoas(t-student),ICVarianciaPessoas(chi-square)";
		esc.EscreveCabecalhoEmCSV(1, linha);	
	}
	file.close();

	string hour;
	string minute = to_string(simTime->tm_min); if(minute.size()<2) minute="0"+minute;
	string second = to_string(simTime->tm_sec); if(second.size()<2) second="0"+second;
	hour = to_string(simTime->tm_hour - 3); if(hour.size()<2) hour="0"+hour;
	hour.append(minute);
	hour.append(second);
	const char* conversao = hour.c_str();
	double timestamp = atof(conversao);


	std::vector<double> valores (9);
	/*TIMESTAMP*/						valores.at(0) = timestamp; 	
	/*UTILIZACAO*/						valores.at(1) = (Upper-Lower)/(Upper+Lower); 	
	/*ICMediaTempoEspera*/				valores.at(3) = EEW; 	
	/*tstudentICVarianciaTempoEspera*/	valores.at(4) = VEW;
	/*chisquareICVarianciaTempoEspera*/	valores.at(5) = 0; 	
	/*ICMediaPessoas*/					valores.at(6) = 0; 								
	/*tstudentICVarianciaPessoas*/		valores.at(7) = 0; 					
	/*chisquareICVarianciaPessoas*/		valores.at(8) = 0;

	/*POLITICA*/switch(fila.Tipo){
		case TipoFila::FCFS:
			valores.at(2) = 0;
			break;
		case TipoFila::LCFS:
			valores.at(2) = 1;
			break;
		default:
			valores.at(2) = 0;
			break;
	}

	esc.EscreveLinhaEmCSV(9, valores);	   
}