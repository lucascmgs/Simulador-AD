#include "Escritor.hpp"
#include<fstream>
using namespace std;

//eu não sei se precisa de fato de um construtor, mas aqui está.
Escritor::Escritor(void){ }

//Esta função é a responsável por "criar" o .csv em questão. 
//Ele escreve um cabeçalho, para que eu possa me guiar quando for gerar gráficos no google sheets a partir do csv.
std::fstream Escritor::CriaCSV(){
    std::fstream arquivo;
    arquivo.open("results.csv", ios::out | ios::app | ios::binary);
    arquivo << "EWRodada,EWRodada2,VWRodada,VWRodada2";
    arquivo << "\n";
    return arquivo;
}

//Esta função imprime um vetor de entradas por linha, que no caso são as métricas coletadas em uma rodada.
//TODO: fazer com que o numero de métricas seja definido apenas uma vez na criação da classe ao invés de *toda vez que se escreve uma linha*
//...
//ou não, talvez a gente queira essa flexibilidade pra fazer merda (???)
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