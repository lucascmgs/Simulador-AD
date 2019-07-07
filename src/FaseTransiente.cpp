#include "FaseTransiente.hpp"
#include <iostream>

FaseTransiente::FaseTransiente(FilaMM1 *fila){
    this->Fila = fila;
}

void FaseTransiente::RodaFaseTransiente(){
    //Se a fila não está inicializada, não podemos estabelecer uma fase transiente
    if(!this->Fila->ChecaSeFilaInicializada()){
        throw "Fila ainda não inicializada!";
    }
    //Indica que a fila está em fase transiente
    bool transiente = true;

    //Estas duas variáveis serão usadas para armazenar as métricas antes de um evento ser tratado
    double tempoEsperaAnterior = 0.0;
    double numeroPessoasAnterior = 0.0;

    //Guardaremos a variação das métricas entre eventos. 
    //Toda vez que a variação for menor que a variação mínima, atualizamos a variação mínima
    double minDeltaTempoEspera = DBL_MAX;
    double minDeltaNumeroPessoas = DBL_MAX;

    //Quantidades de eventos tratados em que os deltas mínimos não mudaram
    int deltaEstavelTempoEspera = 0;
    int deltaEstavelNumeroPessoas = 0;

    //Enquanto estivermos em fase transiente, tratamos o próximo evento
    while(transiente){
        this->Fila->TrataProximoEvento();

        //Pegamos as métricas após tratado o último evento
        double tempoEsperaAtual = this->Fila->EstimadorMediaTempoNaFilaDeEspera();
        double numeroPessoasAtual = this->Fila->EstimadorMediaDoNumeroDePessoasNaFilaDeEspera(0.0);

        //Calculamos os deltas entre eventos
        double deltaTempoEsperaAtual = abs(tempoEsperaAtual - tempoEsperaAnterior);
        double deltaNumeroPessoasAtual = abs(numeroPessoasAtual - numeroPessoasAnterior);

        //Atualizamos os deltas mínimos (desconsiderando a primeira iteração em que os deltas serão 0)
        if(minDeltaTempoEspera > deltaTempoEsperaAtual && deltaTempoEsperaAtual > 0){
            minDeltaTempoEspera = deltaTempoEsperaAtual;
            //Se houve alteração no delta, resetamos a contagem de deltas estáveis em sequência
            deltaEstavelTempoEspera = 0;
        } else{
            deltaEstavelTempoEspera++;
        }
        if(minDeltaNumeroPessoas > deltaNumeroPessoasAtual && deltaNumeroPessoasAtual > 0){
            minDeltaNumeroPessoas = deltaNumeroPessoasAtual;
            deltaEstavelNumeroPessoas = 0;
        } else{
            deltaEstavelNumeroPessoas++;
        }
        //Se, em 1000 eventos tratados em sequência, nenhum delta mudar, teremos terminado nossa fase transiente
        if(deltaEstavelTempoEspera > 1000 && deltaEstavelNumeroPessoas > 1000){
            transiente = false;
        } else {
            //Atualizamos as métricas para os eventos anteriores
            tempoEsperaAnterior = tempoEsperaAtual;
            numeroPessoasAnterior = numeroPessoasAtual;
        }
    }

	std::cout << "---- FIM DA FASE TRANSIENTE ----" << std::endl;
	std::cout << "Coletas feitas: " << Fila->EstatisticasColetadasTempoEspera << "\n" << std::endl;

    //Resetamos as métricas para podermos iniciar a simulação de fato
    this->Fila->ResetaEstatisticasRodada();
}