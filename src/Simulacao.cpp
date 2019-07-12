#include "Simulacao.hpp"

Simulacao::Simulacao(int n, int k, int seed, double lambda, int politicaAtendimento, std::string output) {
    this->n = n;
    this->k = k;
    this->Seed = seed;
    this->Lambda = lambda;
	this->PoliticaAtendimento = politicaAtendimento;
	this->Output = output;
}

void Simulacao::RodaSimulacao() {
	//inicializa o gerador aleatório a partir da semente, seja default ou passada por parâmetro
    GeradorAleatorio::Inicializa(Seed);

	//cria a fila de acordo com o parâmetro de política. É discutível se funciona.
	FilaMM1 fila = FilaMM1(TipoFila::FCFS, Lambda);
	if (this->PoliticaAtendimento == 1) {
		fila = FilaMM1(TipoFila::LCFS, Lambda);
	}

	//Inicializamos a fila adicionando um primeiro evento
	fila.InicializaFila();

	//Roda fase transiente, independente do gerador aleatório
	FaseTransiente trans = FaseTransiente(&fila);
	trans.RodaFaseTransiente();

	//Coleta o tempo de clock no qual a simulação sai da fase transiente e começa a rodar as rodadas
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
	//gera output visual com as métricas coletadas
	GeraEstatisticaSimulacao();
    GeraIntervalosDeConfianca();
	ColetaEstatisticasDaSimulacao(fila, tempoDaSimulacao);
}

void Simulacao::GeraEstatisticaSimulacao() {
	std::cout << "---- FIM DA SIMULAÇÂO ----" << std::endl;
	std::cout << "Parâmetros utilizados:" << std::endl;
	std::cout << "n: " << n << ", k: " << k << ", lambda/rho: " << Lambda << ", politica: " << this->PoliticaAtendimento << "\n" << std::endl;

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
	EWAnalitico = Lambda/(1-Lambda); 
	VWAnaliticoFCFS = (2*Lambda-(Lambda*Lambda))/((1-Lambda)*(1-Lambda));
	VWAnaliticoLCFS = (2*Lambda - pow(Lambda, 2) + pow(Lambda, 3))/(pow(1-Lambda, 3)); 
	ENqAnalitico = (Lambda*Lambda)/(1-Lambda);
	VNqAnalitico = (pow(Lambda, 2)+pow(Lambda, 3)-pow(Lambda, 4))/pow((1-Lambda), 2);
	std::cout << "* Valor Analítico para E[W]: " << EWAnalitico << std::endl;
	std::cout << "* Valor Analítico para V[W]: " << (PoliticaAtendimento==0 ? VWAnaliticoFCFS : VWAnaliticoLCFS) << std::endl;
	std::cout << "* Valor Analítico para E[Nq]: " << ENqAnalitico << std::endl;
	std::cout << "* Valor Analítico para V[Nq]: " << VNqAnalitico << std::endl;
}

void Simulacao::GeraIntervalosDeConfianca() {
    //Para E[W], t-student
	Lower_t_EW = Media_W - t * sqrt(Variancia_Medias_W)/sqrt(n);
	Upper_t_EW = Media_W + t * sqrt(Variancia_Medias_W)/sqrt(n);
	precisao_t_EW = (Upper_t_EW-Lower_t_EW)/(Upper_t_EW+Lower_t_EW);
	std::cout << "\n---- IC E[W] (t-Student) ----" << std::endl;
	std::cout << "[" << Lower_t_EW << ", " << Media_W << ", " << Upper_t_EW << " | Precisão: "<< precisao_t_EW << "]\n" << std::endl;

	//Para V(W), chi-quadrado
	Lower_chi_VW = ((n-1)*Variancia_W)/chiSuperior;
	Upper_chi_VW = ((n-1)*Variancia_W)/chiInferior;
	precisao_chi_VW = (Upper_chi_VW-Lower_chi_VW)/(Upper_chi_VW+Lower_chi_VW);
	centroChi_VW = (Upper_chi_VW+Lower_chi_VW)/2;
	std::cout << "---- IC V(W) (chi-quadrado) ----" << std::endl;
	std::cout << "[" << Lower_chi_VW << ", " << centroChi_VW << ", " << Upper_chi_VW << " | Precisão: "<< precisao_chi_VW << "]\n" << std::endl;

    //Para V(W), t-student
	Lower_t_VW = Variancia_W - t * sqrt(Variancia_Variancias_W)/sqrt(n);
	Upper_t_VW = Variancia_W + t * sqrt(Variancia_Variancias_W)/sqrt(n);
	precisao_t_VW = (Upper_t_VW-Lower_t_VW)/(Upper_t_VW+Lower_t_VW);
	std::cout << "---- IC V(W) (t-Student) ----" << std::endl;
	std::cout << "[" << Lower_t_VW << ", " << Variancia_W << ", " << Upper_t_VW << " | Precisão: "<< precisao_t_VW << "]\n" << std::endl;   

    //Para E[Nq] 
	Lower_t_ENq = Media_Nq - t*sqrt(Variancia_Medias_Nq)/sqrt(n);
	Upper_t_ENq = Media_Nq + t*sqrt(Variancia_Medias_Nq)/sqrt(n);
	precisao_t_ENq = (Upper_t_ENq-Lower_t_ENq)/(Upper_t_ENq+Lower_t_ENq);
	std::cout << "---- IC E[Nq] (t-Student) ----" << std::endl;
	std::cout << "[" << Lower_t_ENq << ", " << Media_Nq << ", " << Upper_t_ENq << " | Precisão: "<< precisao_t_ENq << "]\n" << std::endl;

	//Para V(Nq), chi-quadrado
	Lower_chi_VNq = ((n-1)*Variancia_Nq)/chiSuperior;
	Upper_chi_VNq = ((n-1)*Variancia_Nq)/chiInferior;
	precisao_chi_VNq = (Upper_chi_VNq-Lower_chi_VNq)/(Upper_chi_VNq+Lower_chi_VNq);
	centroChi_VNq = (Upper_chi_VNq+Lower_chi_VNq)/2;
	std::cout << "---- IC V(Nq) (chi-quadrado) ----" << std::endl;
	std::cout << "[" << Lower_chi_VNq << ", " << centroChi_VNq << ", " << Upper_chi_VNq << " | Precisão: "<< precisao_chi_VNq << "]\n" << std::endl;

    //Para V(Nq), t-student	
	Lower_t_VNq = Variancia_Nq - t * sqrt(Variancia_Variancias_Nq)/sqrt(n);
	Upper_t_VNq = Variancia_Nq + t * sqrt(Variancia_Variancias_Nq)/sqrt(n);
	precisao_t_Vnq = (Upper_t_VNq-Lower_t_VNq)/(Upper_t_VNq+Lower_t_VNq);
	std::cout << "---- IC V(Nq) (t-student) ----" << std::endl;
	std::cout << "[" << Lower_t_VNq << ", " << Variancia_Nq << ", " << Upper_t_VNq << " | Precisão: "<< precisao_t_Vnq <<"]\n" << std::endl;   
}

//Escreve as métricas de uma simulação em um arquivo .csv para futuro tratamento dos dados coletados
void Simulacao::ColetaEstatisticasDaSimulacao(FilaMM1 fila, tm * simTime) {
	//Instancia um escritor, que escreve no arquivo de saída. Nome e cabeçalho definidos por strings internas à função.
	Escritor esc = Escritor();
	std::string nomeDoArquivo = Output;
	std::string cabecalhoDoArquivo = "k,Utilização,Política,E[W] Analítico,Lower,E[W],Upper,Precisão,V(W) Analítico,Lower,V(W),Upper,Precisão,Lower,Centro,Upper,Precisão";
	fstream file = esc.CriaCSV(nomeDoArquivo,cabecalhoDoArquivo);
	
	/*string hour;
	string minute = to_string(simTime->tm_min); if(minute.size()<2) minute="0"+minute;
	string second = to_string(simTime->tm_sec); if(second.size()<2) second="0"+second;
	hour = to_string(simTime->tm_hour - 3); if(hour.size()<2) hour="0"+hour;
	hour.append(minute);
	hour.append(second);
	const char* conversao = hour.c_str();
	double timestamp = atof(conversao);*/

	//Vetor de métricas coletadas em uma simulação com posições correspondentes aos títulos da primeira linha
	std::vector<double> valores (17);
	//Carrega o vetor, posição a posição, com as métricas desejadas.
	/*kmin*/									valores.at(0) = k; 	
	/*valor de rho*/							valores.at(1) = Lambda; 
	/*Valor analítico para E[W]*/				valores.at(3) = EWAnalitico; 
	/*Lower IC t-student E[W] */				valores.at(4) = Lower_t_EW;
	/*Centro IC t-student E[W] */				valores.at(5) = Media_W;
	/*Upper IC t-student E[W] */				valores.at(6) = Upper_t_EW;
	/*Precisão IC t-student E[W] */				valores.at(7) = precisao_t_EW;
	/*Lower IC t-student V(W)*/					valores.at(9) = Lower_t_VW;
	/*Lower IC t-student V(W)*/					valores.at(10) = Variancia_W;
	/*Upper IC t-student V(W)*/					valores.at(11) = Upper_t_VW; 	
	/*Precisão IC t-student V(W)*/				valores.at(12) = precisao_t_VW; 								
	/*Lower IC Chi-Square V(W)*/				valores.at(13) = Lower_chi_VW; 					
	/*Variância estimada Chi-Square (centro)*/	valores.at(14) = centroChi_VW;				
	/*Upper IC Chi-Square V(W)*/				valores.at(15) = Upper_chi_VW;				
	/*Precisão do IC com Chi-Square*/			valores.at(16) = precisao_chi_VW;
	
	//Política em vigor na simulação
	switch(fila.Tipo){
		case TipoFila::FCFS:
			valores.at(2) = 0;
			valores.at(8) = VWAnaliticoFCFS;
			break;
		case TipoFila::LCFS:
			valores.at(2) = 1;
			valores.at(8) = VWAnaliticoLCFS;
			break;
		default:
			valores.at(2) = 0;
			valores.at(8) = VWAnaliticoFCFS;
			break;
	}

	file = esc.EscreveLinhaEmCSV(nomeDoArquivo, 17, valores);	   
}