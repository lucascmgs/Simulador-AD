#include "Rodada.hpp"
#include "FilaMM1.hpp"
#include "Escritor.hpp"
#include <fstream>
#include <vector>
using namespace std;

//Construtor
Rodada::Rodada(){ 
    Escritor esc = Escritor();
    this->rodada = 1;
    esc.CriaCSV();
}

//Função que coleta resultados da fila e aumenta o contador de rodada em +1
void Rodada::ColetaResultadosDaRodada(double TempoMedioDeEsperaNaFila, double VarianciaDoTempoDeEsperaNaFila){
    Escritor esc = Escritor();
    std::fstream arquivo;
	this->EWRodada += TempoMedioDeEsperaNaFila;
	this->EWRodada2 += TempoMedioDeEsperaNaFila*TempoMedioDeEsperaNaFila;
	this->VWRodada += VarianciaDoTempoDeEsperaNaFila;
	this->VWRodada2 += pow(VarianciaDoTempoDeEsperaNaFila, 2);
    std::vector<double> linha (4, 0.0);
    linha.at(0) = this->EWRodada; linha.at(1) = this->EWRodada2; linha.at(2) = this->VWRodada; linha.at(3) = this->VWRodada2;
    arquivo = esc.EscreveLinhaEmCSV(4, linha);

	std::cout << "---- FIM DA RODADA " << this->rodada << " ----" << std::endl;
	std::cout << "est coletadas: EWRodada,EWRodada2,VWRodada,VWRodada2" << std::endl;
	std::cout << "EWRodada: " << EWRodada << std::endl;
	std::cout << "EWRodada2: " << EWRodada2 << std::endl;
	std::cout << "VWRodada: " << VWRodada << std::endl;
	std::cout << "VWRodada2: " << VWRodada2 << std::endl;

    this->rodada+=1;
    arquivo.close();
}

void Rodada::ColetaResultadosDaSimulacao(int n){
    Escritor esc = Escritor();
    std::cout << "---- FIM DA SIMULACAO " << " ----" << std::endl;
    std::fstream arquivo;
    //Para W
	this->EEW = EWRodada/n;
	this->VEW = EWRodada2/(n-1) - pow(EWRodada, 2)/(n*(n-1));
	this->EVW = VWRodada/n;
	this->VVW = VWRodada2/(n-1) - pow(VWRodada, 2)/(n*(n-1));
    std::vector<std::string> cabecalho (4);
    std::cout << cabecalho.size() << "!!!" << std::endl;
    cabecalho.at(0) = "EEW"; cabecalho.at(1) = "VEW"; cabecalho.at(2) = "EVW"; cabecalho.at(3) = "VVW";
    arquivo = esc.EscreveCabecalhoEmCSV(4, cabecalho);
    std::vector<double> linha (4, 0.0);
    linha.at(0) = this->EEW; linha.at(1) = this->VEW; linha.at(2) = this->EVW; linha.at(3) = this->VVW;
    arquivo = esc.EscreveLinhaEmCSV(4, linha);

    //TODO: Para Nq
    /* ... */

    arquivo.close();
}

void Rodada::CalculaIntervalosDeConfianca(int t, int n){
    //Para E[W]
	this->Lower = this->EEW - t * sqrt(this->VEW)/sqrt(n);
	this->Upper = this->EEW + t * sqrt(this->VEW)/sqrt(n);
	// std::cout << "IC E[W]: [" << this->Lower << ", " << this->EEW << ", " << this->Upper << "]" << std::endl;

    //Para V(W)
	this->Lower = this->EVW - t * sqrt(this->VVW)/sqrt(n);
	this->Upper = this->EVW + t * sqrt(this->VVW)/sqrt(n);
	// std::cout << "IC V[W]: [" << this->Lower << ", " << this->EVW << ", " << this->Upper << "]" << std::endl;   

    //TODO: Para E[Nq] 

    //TODO: Para V(Nq) 
}

 int main(){
    int n=5; int t=10;
    Rodada rod = Rodada();
    for(int i = 0; i < n; i++){
        rod.ColetaResultadosDaRodada(1.0, 2.0);
    }
    rod.ColetaResultadosDaSimulacao(n);
    rod.CalculaIntervalosDeConfianca(t,n);
    return 0;
} 