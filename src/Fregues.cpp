#include "Fregues.hpp"

//Contrutor para a classe Fregues. Passamos a ele o tempo que o freguês chegou na fila
Fregues::Fregues(double tempoChegada, int IDRodada){
    this->TempoChegada = tempoChegada;
    this->IDRodada = IDRodada;
}


std::ostream& operator<<(std::ostream& os, const Fregues& fregues)
{
  return os << "Fregues { chegada: " << fregues.TempoChegada 
        << ", entrada em serviço: " << fregues.TempoDeEntradaEmServico
        << ", saída da fila: " << fregues.TempoSaida << " }";
}