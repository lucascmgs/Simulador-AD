#include "Rodada.hpp"
#include <fstream>
#include <vector>
using namespace std;

//Construtor
Rodada::Rodada(int indiceRodada, int tamanhoRodada, FilaMM1 *fila){ 
    //Escritor esc = Escritor();
    this->Indice = indiceRodada;
    this->Tamanho = tamanhoRodada;
    this->Fila = fila;
    this->TempoInicio = fila->TempoAtual;
    //esc.CriaCSV();
}


void Rodada::RealizaRodada(){
    //Trata os eventos enquanto faltarem coletas para terminar a rodada
    while(this->Fila->EstatisticasColetadasTempoEspera < this->Tamanho) {
			this->Fila->TrataProximoEvento();
		}
}

//Função que coleta resultados da fila para a rodada
void Rodada::ColetaResultadosDaRodada(){
    //Tempo médio na fila de espera
    this->TempoMedioFilaDeEspera = this->Fila->EstimadorMediaTempoNaFilaDeEspera();
    this->VarianciaTempoMedioFilaDeEspera = this->Fila->EstimadorVarianciaDoTempoNaFilaDeEspera();
    //Tempo médio em serviço
    this->TempoMedioServico = this->Fila->EstimadorMediaTempoNaFilaDeEspera();
    this->VarianciaTempoMedioServico = this->Fila->EstimadorVarianciaDoTempoNaFilaDeEspera();    
    //Tempo médio no sistema
    this->TempoMedioSistema = this->Fila->EstimadorMediaTempoNaFilaDeEspera();
    this->VarianciaTempoMedioSistema = this->Fila->EstimadorVarianciaDoTempoNaFilaDeEspera();
    //Numero médio de pessoas na fila de espera
    double numeroPessoas = this->Fila->EstimadorMediaDoNumeroDePessoasNaFilaDeEspera(this->TempoInicio);
    double numeroPessoasQuadrado = this->Fila->EstimadorMediaDosQuadradosDeNumerosDePessoasNaFilaDeEspera(this->TempoInicio);
    //Escritor esc = Escritor();
    //std::fstream arquivo;
	this->EWRodada = TempoMedioFilaDeEspera;
	this->EWRodada2 = pow(TempoMedioFilaDeEspera, 2);
	this->VWRodada = VarianciaTempoMedioFilaDeEspera;
	this->VWRodada2 = pow(VarianciaTempoMedioFilaDeEspera, 2);
    //std::vector<double> linha (4, 0.0);
    //linha.at(0) = this->EWRodada; linha.at(1) = this->EWRodada2; linha.at(2) = this->VWRodada; linha.at(3) = this->VWRodada2;
    //arquivo = esc.EscreveLinhaEmCSV(4, linha);
    this->ENqRodada = numeroPessoas;
    this->ENqRodada2 = numeroPessoasQuadrado;
    this->VNqRodada = numeroPessoasQuadrado - pow(numeroPessoas, 2);
    this->VNqRodada2 = pow(this->VNqRodada, 2);

	//std::cout << "---- FIM DA RODADA " << this->Indice << " ----" << std::endl;
	//std::cout << "est coletadas: EWRodada,EWRodada2,VWRodada,VWRodada2" << std::endl;
	//std::cout << "EWRodada: " << EWRodada << std::endl;
	//std::cout << "EWRodada2: " << EWRodada2 << std::endl;
	//std::cout << "VWRodada: " << VWRodada << std::endl;
	//std::cout << "VWRodada2: " << VWRodada2 << std::endl;

    //arquivo.close();
    this->Fila->ResetaEstatisticasRodada();
}