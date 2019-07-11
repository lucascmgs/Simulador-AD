#ifndef FREGUES
#define FREGUES
#include <iostream>
//Classe do Freguês, com informações pertinentes
class Fregues {
    public:
    //Tempo em que o Freguês chegou na fila
    double TempoChegada;
    //Tempo em que o Freguês entrou em serviço. Podemos calcular o tempo na fila de espera com (TempoDeEntradaEmServiço - TempoChegada)
    double TempoDeEntradaEmServico;
    //Tempo em que o Freguês saiu da fila. Podemos calcular o tempo de serviço com (TempoSaida - TempoDeEntradaEmServiço)
    double TempoSaida;
    //ID da rodada na qual ocorreu a chegada do freguês
    int IDRodada;
    Fregues(double tempoChegada, int IDRodada);
    friend std::ostream& operator<<(std::ostream& os, const Fregues& fregues);
};

#endif