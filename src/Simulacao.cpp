#include "Simulacao.hpp"

Simulacao::Simulacao(int n, int k, int seed, double lambda, int politicaAtendimento) {
    this->n = n;
    this->k = k;
    this->Seed = seed;
    this->Lambda = lambda;
	this->PoliticaAtendimento = politicaAtendimento;
}

void Simulacao::RodaSimulacao() {
    GeradorAleatorio::Inicializa(Seed);

	FilaMM1 fila = FilaMM1(TipoFila::FCFS, Lambda);
	if (this->PoliticaAtendimento == 1) {
		fila = FilaMM1(TipoFila::LCFS, Lambda);
	}

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
		//Guardamos uma referência para a rodada realizada para o cálculo dos estimadores
		this->rodadas.push_back(rod);
	}
	//fim da simulação
	GeraEstatisticaSimulacao();
    GeraIntervalosDeConfianca();
	ColetaEstatisticasDaSimulacao(fila, tempoDaSimulacao);
}

void Simulacao::GeraEstatisticaSimulacao() {
	std::cout << "---- FIM DA SIMULAÇÂO ----" << std::endl;
	std::cout << "Parâmetros utilizados:" << std::endl;
	std::cout << "n: " << n << ", k: " << k << ", lambda: " << Lambda << ", politica: " << this->PoliticaAtendimento << "\n" << std::endl;

	Media_W = 0;
	Media_Nq = 0;
	Variancia_W = 0;
	Variancia_Nq = 0;
	for (unsigned int i = 0; i < rodadas.size(); i++) {
		Media_W += rodadas[i].EWRodada;
		Media_Nq += rodadas[i].ENqRodada;
		Variancia_W += rodadas[i].VWRodada;
		Variancia_Nq += rodadas[i].VNqRodada;
	}
	Media_W /= n;
	Media_Nq /= n;
	Variancia_W /= n;
	Variancia_Nq /= n;
	std::cout << "* Média W: " << Media_W << std::endl;
	std::cout << "* Média Nq: " << Media_Nq << std::endl;
	std::cout << "* Variância W: " << Variancia_W << std::endl;
	std::cout << "* Variância Nq: " << Variancia_Nq << std::endl;

	Variancia_Medias_W = 0;
	Variancia_Medias_Nq = 0;
	Variancia_Variancias_W = 0;
	Variancia_Variancias_Nq = 0;
	for (unsigned int i = 0; i < rodadas.size(); i++) {
		Variancia_Medias_W += pow(rodadas[i].EWRodada - Media_W, 2);
		Variancia_Medias_Nq += pow(rodadas[i].ENqRodada - Media_Nq, 2);
		Variancia_Variancias_W += pow(rodadas[i].VWRodada - Variancia_W, 2);
		Variancia_Variancias_Nq += pow(rodadas[i].VNqRodada - Variancia_Nq, 2);
	}
	Variancia_Medias_W /= (n-1);
	Variancia_Medias_Nq /= (n-1);
	Variancia_Variancias_W /= (n-1);
	Variancia_Variancias_Nq /= (n-1);
	std::cout << "* Variância das Médias de W: " << Variancia_Medias_W << std::endl;
	std::cout << "* Variância das Médias de Nq: " << Variancia_Medias_Nq << std::endl;
	std::cout << "* Variância das Variâncias de W: " << Variancia_Variancias_W << std::endl;
	std::cout << "* Variância das Variâncias de Nq: " << Variancia_Variancias_Nq << std::endl;

	//Valores analíticos (desenvolvimento das contas está no relatório)
	double EWAnalitico = Lambda/(1-Lambda); 
	double VWAnaliticoFCFS = (2*Lambda-(Lambda*Lambda))/((1-Lambda)*(1-Lambda));
	double VWAnaliticoLCFS = (2*Lambda - pow(Lambda, 2) + pow(Lambda, 3))/(pow(1-Lambda, 3)); 
	double ENqAnalitico = (Lambda*Lambda)/(1-Lambda);
	double VNqAnalitico = (pow(Lambda, 2)+pow(Lambda, 3)-pow(Lambda, 4))/pow((1-Lambda), 2);
	std::cout << "* Valor Analítico para E[W]: " << EWAnalitico << std::endl;
	std::cout << "* Valor Analítico para V[W]: " << (PoliticaAtendimento==0 ? VWAnaliticoFCFS : VWAnaliticoLCFS) << std::endl;
	std::cout << "* Valor Analítico para E[Nq]: " << ENqAnalitico << std::endl;
	std::cout << "* Valor Analítico para V[Nq]: " << VNqAnalitico << std::endl;
}

void Simulacao::GeraIntervalosDeConfianca() {
    //Para E[W], t-student
	double Lower = Media_W - t * sqrt(Variancia_Medias_W)/sqrt(n);
	double Upper = Media_W + t * sqrt(Variancia_Medias_W)/sqrt(n);
	double precisao = (Upper-Lower)/(Upper+Lower);
	std::cout << "\n---- IC E[W] (t-Student) ----" << std::endl;
	std::cout << "[" << Lower << ", " << Media_W << ", " << Upper << " | Precisão: "<< precisao << "]\n" << std::endl;

	//Para V(W), chi-quadrado
	Lower = ((n-1)*Variancia_W)/chiSuperior;
	Upper = ((n-1)*Variancia_W)/chiInferior;
	precisao = (Upper-Lower)/(Upper+Lower);
	double centroChi = (Upper+Lower)/2;
	std::cout << "---- IC V(W) (chi-quadrado) ----" << std::endl;
	std::cout << "[" << Lower << ", " << centroChi << ", " << Upper << " | Precisão: "<< precisao << "]\n" << std::endl;

    //Para V(W), t-student
	Lower = Variancia_W - t * sqrt(Variancia_Variancias_W)/sqrt(n);
	Upper = Variancia_W + t * sqrt(Variancia_Variancias_W)/sqrt(n);
	precisao = (Upper-Lower)/(Upper+Lower);
	std::cout << "---- IC V(W) (t-Student) ----" << std::endl;
	std::cout << "[" << Lower << ", " << Variancia_W << ", " << Upper << " | Precisão: "<< precisao << "]\n" << std::endl;   

    //Para E[Nq] 
	Lower = Media_Nq - t*sqrt(Variancia_Medias_Nq)/sqrt(n);
	Upper = Media_Nq + t*sqrt(Variancia_Medias_Nq)/sqrt(n);
	precisao = (Upper-Lower)/(Upper+Lower);
	std::cout << "---- IC E[Nq] (t-Student) ----" << std::endl;
	std::cout << "[" << Lower << ", " << Media_Nq << ", " << Upper << " | Precisão: "<< precisao << "]\n" << std::endl;

	//Para V(Nq), chi-quadrado
	Lower = ((n-1)*Variancia_Nq)/chiSuperior;
	Upper = ((n-1)*Variancia_Nq)/chiInferior;
	precisao = (Upper-Lower)/(Upper+Lower);
	centroChi = (Upper+Lower)/2;
	std::cout << "---- IC V(Nq) (chi-quadrado) ----" << std::endl;
	std::cout << "[" << Lower << ", " << centroChi << ", " << Upper << " | Precisão: "<< precisao << "]\n" << std::endl;

    //Para V(Nq), t-student	
	Lower = Variancia_Nq - t * sqrt(Variancia_Variancias_Nq)/sqrt(n);
	Upper = Variancia_Nq + t * sqrt(Variancia_Variancias_Nq)/sqrt(n);
	precisao = (Upper-Lower)/(Upper+Lower);
	std::cout << "---- IC V(Nq) (t-student) ----" << std::endl;
	std::cout << "[" << Lower << ", " << Variancia_Nq << ", " << Upper << " | Precisão: "<< precisao <<"]\n" << std::endl;   
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
	/*UTILIZACAO*/						valores.at(1) = Lambda; 	
	/*ICMediaTempoEspera*/				valores.at(3) = Media_W; 	
	/*tstudentICVarianciaTempoEspera*/	valores.at(4) = Variancia_W;
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