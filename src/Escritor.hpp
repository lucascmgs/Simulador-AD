#ifndef ESCRITOR
#define ESCRITOR

#include<vector>
#include<fstream>
using namespace std;

class Escritor{
    public:
    Escritor(void);
    std::fstream CriaCSV(string nomeDoArquivo, string cabecalho);
    std::fstream EscreveLinhaEmCSV(string nomeDoArquivo, int numeroDeMetricas, std::vector<double> entradas);
    std::fstream EscreveCabecalhoEmCSV(string nomeDoArquivo, string entradas);
};
#endif