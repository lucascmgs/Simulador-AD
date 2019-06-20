#ifndef ESCRITOR
#define ESCRITOR

#include<vector>
#include<fstream>
using namespace std;

class Escritor{
    Escritor();
    std::fstream CriaCSV();
    std::fstream EscreveLinhaEmCSV(int numeroDeMetricas, std::vector<double> entradas);
};
#endif