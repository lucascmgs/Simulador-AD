#ifndef GERADORALEATORIO
#define GERADORALEATORIO

#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <float.h>
#include <math.h>


namespace GeradorAleatorio {

	static bool inicializado = false;
    
	static void Inicializa(unsigned int seed) {
		if (!inicializado) {
			srand(seed);
			inicializado = true;
		}
		else {
			 std::cout << "Gerador de números aleatórios já inicializado!" << std::endl;
		}
	}

	static double Uniforme01() {
		//Obtemos um valor aleatório inteiro e realizamos cast para um número real.
		double aleatorio = (double)rand();
		//Normalizamos em relação ao maior valor possível para obter um número entre 0 e 1.
		return aleatorio / RAND_MAX;
	}

	static double Exponencial(double lambda) {
		//Variável uniforme para gerar uma amostra da exponencial invertendo a PDF
		double uniforme = Uniforme01();
		//
		if (uniforme == 0) {
			uniforme = DBL_MIN;
		}
		//A função invertida original é x = log(1-uniforme)/-lambda. Como a variável uniforme 
		//é um valor entre 0 e 1, podemos economizar a operação de subtração.
		return -log(uniforme) / lambda;
	}
}
#endif