#ifndef ESCRITOR
#define ESCRITOR

#include<vector>
#include<fstream>
using namespace std;

class Escritor{
    public:
    Escritor(void);
    std::fstream CriaCSV();
    std::fstream EscreveLinhaEmCSV(int numeroDeMetricas, std::vector<double> entradas);
    std::fstream EscreveCabecalhoEmCSV(int numeroDeMetricas, std::vector<std::string> entradas);
};
#endif