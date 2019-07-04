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
    GeraEstatisticaSimulacao();
    GeraIntervaloDeConfianca();
	ColetaEstatisticasDaSimulacao(fila, tempoDaSimulacao);
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

	//Tempos em fila
	this->Trodadas += rod.TempoMedioSistema;
	this->Wrodadas += rod.TempoMedioFilaDeEspera;
	this->Xrodadas += rod.TempoMedioServico;
	this->VTrodadas += rod.VarianciaTempoMedioSistema;
	this->VWrodadas += rod.VarianciaTempoMedioFilaDeEspera;
	this->VXrodadas += rod.VarianciaTempoMedioServico;
}


void Simulacao::GeraEstatisticaSimulacao() {
	std::cout << "VALOR DE LAMBDA VIGENTE: " << Lambda << std::endl;
	std::cout << "\n";

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
	std::cout << "\n";

	//Para Nq
	EENq = this->ENqRodadas/n;
	VENq = this->ENqRodadas2/(n-1) - pow(this->ENqRodadas, 2)/(n*(n-1));
	EVNq = this->VNqRodadas/n;
	VVNq = this->VNqRodadas2/(n-1) - pow(this->VNqRodadas, 2)/(n*(n-1));
	
	std::cout << "EENq estimado: " << EENq << std::endl;
	std::cout << "VENq: " << VENq << std::endl;
	std::cout << "EVNq: " << EVNq << std::endl;
	std::cout << "VVNq: " << VVNq << std::endl;
	std::cout << "\n";

	//Tempos associados
	Tbarra = Trodadas/n;
	VTbarra = VTrodadas/n;
	Wbarra = Wrodadas/n;
	VWbarra = VWrodadas/n;
	Xbarra = Xrodadas/n;
	VXbarra = VXrodadas/n;

	std::cout << "Tempo médio no sistema (Xbarra + Wbarra): " << Tbarra << std::endl;
	std::cout << "Tempo médio em serviço: " << Xbarra << std::endl;
	std::cout << "Tempo médio na fila: " << Wbarra << std::endl;
	std::cout << "\n";
}

void Simulacao::GeraIntervaloDeConfianca() {
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