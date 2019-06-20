#include "Escritor.hpp"
#include<fstream>
using namespace std;

//eu não sei se precisa de fato de um construtor, mas aqui está.
Escritor::Escritor(){ }

//Esta função é a responsável por "criar" o .csv em questão. 
//Ele escreve um cabeçalho, para que eu possa me guiar quando for gerar gráficos no google sheets a partir do csv.
std::fstream CriaCSV(){
    std::fstream arquivo;
    arquivo.open("results.csv", ios::out | ios::app | ios::binary);
    arquivo << "ESTColetadas,EWRodada,EWRodada2,VWRodada,VWRodada2,VAnaliticoEEW,EEWEstimado";
    arquivo << "\n";
    return arquivo;
}

//Esta função imprime um vetor de entradas por linha, que no caso são as métricas coletadas em uma rodada.
//TODO: fazer com que o numero de métricas seja definido apenas uma vez na criação da classe ao invés de *toda vez que se escreve uma linha*
//...
//ou não, talvez a gente queira essa flexibilidade pra fazer merda (???)
std::fstream EscreveLinhaEmCSV(int numeroDeMetricas, std::vector<double> entradas){
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