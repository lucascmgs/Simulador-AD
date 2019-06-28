#include "Rodada.hpp"
#include <fstream>
#include <vector>
using namespace std;

//Construtor
Rodada::Rodada(int indiceRodada, int tamanhoRodada, FilaMM1 *fila){ 
    Escritor esc = Escritor();
    this->Indice = indiceRodada;
    this->Tamanho = tamanhoRodada;
    this->Fila = fila;
    esc.CriaCSV();
}


void Rodada::RealizaRodada(){
    while(this->Fila->EstatisticasColetadas < this->Tamanho) {
			this->Fila->TrataProximoEvento();
		}
}

//Função que coleta resultados da fila para a rodada
void Rodada::ColetaResultadosDaRodada(){
    double tempoMedio = this->Fila->TempoMedioDeEsperaNaFila();
    double varianciaTempoMedio = this->Fila->VarianciaDoTempoDeEsperaNaFila();
    Escritor esc = Escritor();
    std::fstream arquivo;
	this->EWRodada = tempoMedio;
	this->EWRodada2 = pow(tempoMedio, 2);
	this->VWRodada = varianciaTempoMedio;
	this->VWRodada2 = pow(varianciaTempoMedio, 2);
    std::vector<double> linha (4, 0.0);
    linha.at(0) = this->EWRodada; linha.at(1) = this->EWRodada2; linha.at(2) = this->VWRodada; linha.at(3) = this->VWRodada2;
    arquivo = esc.EscreveLinhaEmCSV(4, linha);

	//std::cout << "---- FIM DA RODADA " << this->Indice << " ----" << std::endl;
	//std::cout << "est coletadas: EWRodada,EWRodada2,VWRodada,VWRodada2" << std::endl;
	//std::cout << "EWRodada: " << EWRodada << std::endl;
	//std::cout << "EWRodada2: " << EWRodada2 << std::endl;
	//std::cout << "VWRodada: " << VWRodada << std::endl;
	//std::cout << "VWRodada2: " << VWRodada2 << std::endl;

    arquivo.close();
    this->Fila->ResetaEstatisticasRodada();
}