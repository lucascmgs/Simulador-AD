#include "Escritor.hpp"
#include<iostream>
#include<fstream>
using namespace std;

//Construtor do Escritor, essencial para manipular .csv.
Escritor::Escritor(void){ }

//Esta função é a responsável por "criar" o .csv em questão. 
//ENTRADA: string nomeDoArquivo, que será o nome do arquivo gerado. Passar o nome do arquivo sem ".csv"
//ENTRADA: string cabecalho, que será a primeira linha do arquivo.
//SAIDA: fstream, arquivo para ser manipulado pelas outras funções da classe
std::fstream Escritor::CriaCSV(std::string nomeDoArquivo, std::string cabecalho){
    std::fstream arquivo;
    std::string realNomeDoArquivo = nomeDoArquivo + ".csv";
    arquivo.open(realNomeDoArquivo.c_str(), ios::out | ios::app | ios::binary);
    //se o arquivo de nome indicado já existir no diretório do programa, o cabeçalho não será reescrito. Bom para coletar métricas de diferentes simulações.
    if(arquivo.tellg() == 0){
        arquivo << cabecalho;
        arquivo << "\n";
    }
    return arquivo;
}

//Esta função imprime um vetor de entradas por linha, que no caso do simulador são as métricas coletadas em uma simulação.
std::fstream Escritor::EscreveLinhaEmCSV(std::string nomeDoArquivo, int numeroDeMetricas, std::vector<double> metricas){
    std::fstream arquivo;
    std::string realNomeDoArquivo = nomeDoArquivo + ".csv";
    arquivo.open(realNomeDoArquivo.c_str(), ios::out | ios::app | ios::binary);
    //Escreve os parâmetros contidos no vetor de metricas, separando-os por vírgula
    for(int j = 0; j < numeroDeMetricas; j++){
        if(j == numeroDeMetricas-1){
            arquivo << metricas.at(j);
            break;
        }
        arquivo << metricas.at(j);
        arquivo << ",";
    }
    arquivo << "\n";
    return arquivo;
}

//Esta função imprime uma string em uma linha do programa. Originalmente pretendido para criar um outro cabeçalho, mas caiu em desuso rapidamente.
//Ainda pode escrever uma linha de string no csv, mas cabe a quem for passar a string usar vírgulas para dividir elementos entre células, caso assim o queira
std::fstream Escritor::EscreveCabecalhoEmCSV(std::string nomeDoArquivo, string entradas){
    std::fstream arquivo;
    std::string realNomeDoArquivo = nomeDoArquivo + ".csv";
    arquivo.open(realNomeDoArquivo.c_str(), ios::out | ios::app | ios::binary);
    arquivo << entradas;
    arquivo << "\n";
    return arquivo;
}