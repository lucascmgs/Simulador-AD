#include "Rodada.hpp"
#include "FilaMM1.hpp"
#include "Escritor.hpp"
#include <fstream>
#include <vector>
using namespace std;

//Construtor
Rodada::Rodada(){ 
    this->rodada = 1;
    Escritor().CriaCSV();
}

//Função que coleta resultados da fila e aumenta o contador de rodada em +1
void Rodada::ColetaResultadosDaRodada(double TempoMedioDeEsperaNaFila, double VarianciaDoTempoDeEsperaNaFila){
    std::fstream arquivo;
	this->EWRodada += TempoMedioDeEsperaNaFila;
	this->EWRodada2 += TempoMedioDeEsperaNaFila*TempoMedioDeEsperaNaFila;
	this->VWRodada += VarianciaDoTempoDeEsperaNaFila;
	this->VWRodada2 += pow(VarianciaDoTempoDeEsperaNaFila, 2);
    std::vector<double> linha (4, 0.0);
    linha.at(0) = this->EWRodada; linha.at(1) = this->EWRodada2; linha.at(2) = this->VWRodada; linha.at(3) = this->VWRodada2;
    arquivo = Escritor().EscreveLinhaEmCSV(4, linha);
    this->rodada+=1;
    arquivo.close();
}

void Rodada::ColetaResultadosDaSimulacao(int n){
    std::fstream arquivo;
    //Para W
	this->EEW = EWRodada/n;
	this->VEW = EWRodada2/(n-1) - pow(EWRodada, 2)/(n*(n-1));
	this->EVW = VWRodada/n;
	this->VVW = VWRodada2/(n-1) - pow(VWRodada, 2)/(n*(n-1));
    std::vector<std::string> cabecalho;
    cabecalho.at(0) = "EEW"; cabecalho.at(1) = "VEW"; cabecalho.at(2) = "EVW"; cabecalho.at(3) = "VVW";
    arquivo = Escritor().EscreveCabecalhoEmCSV(4, cabecalho);
    std::vector<double> linha (4, 0.0);
    linha.at(0) = this->EEW; linha.at(1) = this->VEW; linha.at(2) = this->EVW; linha.at(3) = this->VVW;
    arquivo = Escritor().EscreveLinhaEmCSV(4, linha);

    //TODO: Para Nq
    /* ... */

    arquivo.close();
}

 int main(){
    int n=5;
    for(int i = 1; i < n; i++){
        Rodada().ColetaResultadosDaRodada(1.0, 2.0);
    }
    Rodada().ColetaResultadosDaSimulacao(n);
    return 0;
} 