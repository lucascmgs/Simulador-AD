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