#include "Escritor.hpp"
#include<iostream>
#include<fstream>
using namespace std;

//eu não sei se precisa de fato de um construtor, mas aqui está.
Escritor::Escritor(void){ }

//Esta função é a responsável por "criar" o .csv em questão. 
//ENTRADA: string nomeDoArquivo, que será o nome do arquivo gerado. Passar o nome do arquivo sem ".csv"
//ENTRADA: string cabecalho, que será a primeira linha do arquivo.
//SAIDA: fstream, arquivo para ser manipulado pelas outras funções da classe
std::fstream Escritor::CriaCSV(std::string nomeDoArquivo, std::string cabecalho){
    std::fstream arquivo;
    std::string realNomeDoArquivo = nomeDoArquivo + ".csv";
    arquivo.open(nomeDoArquivo.c_str(), ios::out | ios::app | ios::binary);
    std::cout << "sim" << "\n";
    arquivo << cabecalho;
    arquivo << "\n";
    return arquivo;
}

//Esta função imprime um vetor de entradas por linha, que no caso são as métricas coletadas em uma rodada.
std::fstream Escritor::EscreveLinhaEmCSV(int numeroDeMetricas, std::vector<double> entradas){
    std::fstream arquivo;
    arquivo.open("results.csv", ios::out | ios::app | ios::binary);
    for(int j = 0; j < numeroDeMetricas; j++){
        if(j == numeroDeMetricas-1){
            arquivo << entradas.at(j);
            break;
        }
        arquivo << entradas.at(j);
        arquivo << ",";
    }
    arquivo << "\n";
    return arquivo;
}

std::fstream Escritor::EscreveCabecalhoEmCSV(int numeroDeMetricas, std::vector<std::string> entradas){
    std::fstream arquivo;
    arquivo.open("results.csv", ios::out | ios::app | ios::binary);
    for(int j = 0; j < numeroDeMetricas; j++){
        if(j == numeroDeMetricas-1){
            arquivo << entradas.at(j);
            break;
        }
        arquivo << entradas.at(j);
        arquivo << ",";
    }
    arquivo << "\n";
    return arquivo;
}