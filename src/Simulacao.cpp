#include "Simulacao.hpp"

Simulacao::Simulacao(int n, int k, int seed, double lambda, int politicaAtendimento) {
    this->n = n;
    this->k = k;
    this->Seed = seed;
    this->Lambda = lambda;
	this->PoliticaAtendimento = politicaAtendimento;
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

//Escreve as métricas de uma simulação em um arquivo .csv para futuro tratamento dos dados coletados
void Simulacao::ColetaEstatisticasDaSimulacao(FilaMM1 fila, tm * simTime) {
	//Instancia um escritor, que escreve no arquivo de saída. Nome e cabeçalho definidos por strings internas à função.
	Escritor esc = Escritor();
	std::string nomeDoArquivo = "metricas";
	std::string cabecalhoDoArquivo = "k,rho,politica,tamanho da fase trans.,E[W] Analítico,E[W] Amostral,V[W] Analítico,V[W]Amostral,E[Nq] Analítico,E[Nq] Amostral,V[Nq] Analítico,V[Nq] Amostral";
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
	std::vector<double> valores (12);
	//Carrega o vetor, posição a posição, com as métricas desejadas.
	/*kmin*/									valores.at(0) = k; 	
	/*valor de rho*/							valores.at(1) = Lambda; 	
	/*lower IC pela T-Student*/					valores.at(2) = PoliticaAtendimento; 	
	/*Variância do tempo de espera na fila*/	valores.at(3) = 0;
	/*Upper IC com T-Student*/					valores.at(4) = Lambda/(1-Lambda); 	
	/*Precisão do IC com T-Student*/			valores.at(5) = Media_W; 								
	/*lower IC pela Chi-Square*/				valores.at(6) = (PoliticaAtendimento==0 ? (2*Lambda-(Lambda*Lambda))/((1-Lambda)*(1-Lambda)) : (2*Lambda - pow(Lambda, 2) + pow(Lambda, 3))/(pow(1-Lambda, 3))) ; 					
	/*Variância estimada pela Chi-Square*/		valores.at(7) = Variancia_W;				
	/*upper IC com Chi-Square*/					valores.at(8) = (Lambda*Lambda)/(1-Lambda);				
	/*Precisão do IC com Chi-Square*/			valores.at(9) = Media_Nq;				
	/*Precisão do IC com Chi-Square*/			valores.at(10) = (pow(Lambda, 2)+pow(Lambda, 3)-pow(Lambda, 4))/pow((1-Lambda), 2);
	/*Precisão do IC com Chi-Square*/			valores.at(11) = Variancia_Nq;
	

	file = esc.EscreveLinhaEmCSV(nomeDoArquivo, 12, valores);	   
}