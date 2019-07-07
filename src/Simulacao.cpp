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
    GeraEstatisticaSimulacao();
    GeraIntervalosDeConfianca();
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
}


void Simulacao::GeraEstatisticaSimulacao() {
	std::cout << "PARÂMETROS UTILIZADOS" << std::endl;
	std::cout << "n: " << n << ", k: " << k << ", lambda: " << Lambda << "\n" << std::endl;
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

	//Valores analíticos (desenvolvimento das contas está no relatório)
	double EWAnalitico = Lambda/(1-Lambda); 
	double VWAnalitico = (2*Lambda-(Lambda*Lambda))/(1-(Lambda*Lambda));
	double ENqAnalitico = (Lambda*Lambda)/(1-Lambda);
	double VNqAnalitico = (pow(Lambda, 2)+pow(Lambda, 3)-pow(Lambda, 4))/pow((1-Lambda), 2);

	std::cout << "---- W ----" << std::endl;
	std::cout << "Valor Analítico para E[W]: " << EWAnalitico << std::endl;
	std::cout << "E[W] estimado: " << EEW << std::endl;
	std::cout << "V[E[W]]: " << VEW << std::endl;
	std::cout << "Valor Analítico para V[W]: " << VWAnalitico << std::endl;
	std::cout << "V[W] estimado: " << EVW << std::endl;
	std::cout << "V[V[W]]: " << VVW << std::endl;

	std::cout << "\n---- Nq ----" << std::endl;
	std::cout << "Valor Analítico para E[Nq]: " << ENqAnalitico << std::endl;
	std::cout << "E[Nq] estimado: " << EENq << std::endl;
	std::cout << "V[E[Nq]]: " << VENq << std::endl;
	std::cout << "Valor Analítico para V[Nq]: " << VNqAnalitico << std::endl;
	std::cout << "V[Nq] estimado: " << EVNq << std::endl;
	std::cout << "V[V[Nq]]: " << VVNq << std::endl;
}

void Simulacao::GeraIntervalosDeConfianca() {
    //Para E[W], t-student
	Lower = EEW - t * sqrt(VEW)/sqrt(n*k);
	Upper = EEW + t * sqrt(VEW)/sqrt(n*k);
	double precisao = (Upper-Lower)/(Upper+Lower);
	std::cout << "\n---- IC E[W] (t-Student) ----" << std::endl;
	std::cout << "[" << Lower << ", " << EEW << ", " << Upper << " | Precisão: "<< precisao <<"]\n" << std::endl;

	//Para V(W), chi-quadrado
	Lower = ((n-1)*EVW)/chiSuperior;
	Upper = ((n-1)*EVW)/chiInferior;
	precisao = (Upper-Lower)/(Upper+Lower);
	std::cout << "---- IC V[W] (chi-quadrado) ----" << std::endl;
	std::cout << "[" << Lower << ", " << EVW << ", " << Upper << " | Precisão: "<< precisao <<"]\n" << std::endl;

    //Para V(W), t-student
	Lower = EVW - t * sqrt(VVW)/sqrt(n*k);
	Upper = EVW + t * sqrt(VVW)/sqrt(n*k);
	precisao = (Upper-Lower)/(Upper+Lower);
	std::cout << "---- IC V[W] (t-Student) ----" << std::endl;
	std::cout << "[" << Lower << ", " << EVW << ", " << Upper << " |Precisão: "<< precisao <<"]\n" << std::endl;   

    //Para E[Nq] 
	Lower = EENq - t*sqrt(VENq)/sqrt(n*k);
	Upper = EENq + t*sqrt(VENq)/sqrt(n*k);
	precisao = (Upper-Lower)/(Upper+Lower);
	std::cout << "---- IC E(Nq) (t-Student) ----" << std::endl;
	std::cout << "[" << Lower << ", " << EENq << ", " << Upper << " ] |Precisão: "<< precisao << "]\n" << std::endl;

	//Para V(Nq), chi-quadrado
	Lower = ((n-1)*EVNq)/chiSuperior;
	Upper = ((n-1)*EVNq)/chiInferior;
	precisao = (Upper-Lower)/(Upper+Lower);
	std::cout << "---- IC V(Nq) (chi-quadrado) ----" << std::endl;
	std::cout << "[" << Lower << ", " << EVNq << ", " << Upper << " | Precisão: "<< precisao <<"]\n" << std::endl;

    //Para V(Nq), t-student	
	Lower = EVNq - t * sqrt(VVNq)/sqrt(n*k);
	Upper = EVNq + t * sqrt(VVNq)/sqrt(n*k);
	precisao = (Upper-Lower)/(Upper+Lower);
	std::cout << "---- IC V(Nq) (t-student) ----" << std::endl;
	std::cout << "[" << Lower << ", " << EVNq << ", " << Upper << "] |Precisão: "<< precisao <<"]\n" << std::endl;   
}

void Simulacao::ColetaEstatisticasDaSimulacao(FilaMM1 fila, tm * simTime) {
	Escritor esc = Escritor();
	fstream file;
	file.open("results.csv");
	if(file.fail()){
		std::vector<string> linha (1);
		linha.at(0) = "Timestamp,Precisão,Politica,ICMediaTempoEspera,ICVarianciaTempoEspera(t-student),ICVarianciaTempoEspera(chi-square),ICMediaPessoas,ICVarianciaPessoas(t-student),ICVarianciaPessoas(chi-square)";
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