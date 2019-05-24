#include <string.h>
#include <iostream>
#include "GeradorAleatorio.hpp"


int main(int argc, char* argv[])
{
	int n = 3200;
	int k = 100;
	int seed = time(NULL);
    double lambda = 1.0;
    

    //Permite testar o simulador com diferentes parâmetros
	for (int i = 1; i < argc; i++) {
		if (i < argc - 1) {
			if (strcmp(argv[i], "-n") == 0) {
				n = atoi(argv[i + 1]);
			}
			if (strcmp(argv[i], "-k") == 0) {
				k = atoi(argv[i + 1]);
			}
            if (strcmp(argv[i], "-seed") == 0) {
				lambda = atoi(argv[i + 1]);
			}
			if (strcmp(argv[i], "-lambda") == 0) {
				lambda = atof(argv[i + 1]);
			}
		}
	}

    GeradorAleatorio::Inicializa(seed);
    
	double total = 0;
   	for (int i = 0; i < n; i++) {
   		double totalInterno = 0;
   		for (int j = 0; j < k; j++) {
   			double amostraExp = GeradorAleatorio::Exponencial(lambda);   			
   			totalInterno += amostraExp;
   		}
   		total += totalInterno/k;
   	}
   	double media = (double)total / n;
   	std::cout << "Media da exponencial: " << media << std::endl;

}

