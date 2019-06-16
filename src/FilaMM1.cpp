#include "FilaMM1.hpp"

//Construtor para a fila M/M/1
FilaMM1::FilaMM1(TipoFila tipo, double utilizacao){
    this->Tipo = tipo;
    this->Utilizacao = utilizacao;
}

void FilaMM1::TrataProximoEvento(){
	//this->ReportaStatus();
    //Obtemos o próximo evento a ser tratado e o removemos da heap de eventos.
    Evento proximoEvento = this->Eventos.top();
    this->Eventos.pop();
    //Avançamos o tempo de simulação para o tempo de ocorrência do evento sendo tratado
    this->TempoAtual = proximoEvento.TempoOcorrencia;

    //Realizamos o tratamento referente ao tipo específico de evento, seja chegada ou saída da fila.
    switch (proximoEvento.Tipo)
    {
        case TipoEvento::CHEGADA :
            //Se a fila estiver vazia quando um freguês chega, devemos gerar o tempo que o freguês irá levar para sair.
            if(this->Fregueses.empty()){
                this->GeraProximaSaida();
            }
            //Acrescentamos o freguês à lista de fregueses
            this->Fregueses.push_front(Fregues(this->TempoAtual));
            //Se a fila estava vazia quando o freguês chegou, ou seja, se após uma chegada o número de pessoas no sistema é 1,
            //o freguês entra em serviço imediatamente.
			if (this->Fregueses.size() == 1) {
				this->Fregueses.front().TempoDeEntradaEmServico = this->TempoAtual;
			}
            //Geramos a próxima chegada.
			this->GeraProximaChegada();
            break;
    
        case TipoEvento::SAIDA:
            //Criamos um freguês que será uma cópia do freguês a sair da fila
            Fregues freguesDeSaida = Fregues(0);
            //Para cada política de atendimento removemos um freguês da fila do lado correspondente da fila de fregueses
            switch(this->Tipo){
                case TipoFila::FCFS :
                    //No caso FCFS, removemos o freguês da parte de trás da fila, que é o que está a mais tempo na fila.
                    freguesDeSaida = this->Fregueses.back();
                    this->Fregueses.pop_back();
                    //Se após uma saída ainda tem fregueses na fila, colocamos em serviço o próximo freguês que está há mais tempo na fila. 
					if (this->Fregueses.size() > 0) {
						this->Fregueses.back().TempoDeEntradaEmServico = this->TempoAtual;
					}
                    break;
                case TipoFila::LCFS :
                    //No caso LCFS, removemos o freguês da frente da fila, ou seja, o que está em espera há menos tempo.
                    freguesDeSaida = this->Fregueses.front();
                    this->Fregueses.pop_front();
                    //De forma análoga à FCFS, atribuímos um tempo de entrada em serviço ao próximo freguês, caso haja algum
					if (this->Fregueses.size() > 0) {
						this->Fregueses.front().TempoDeEntradaEmServico = this->TempoAtual;
					}
                    break;
            }

            //Atribuímos um tempo de saída ao freguês removido.
            freguesDeSaida.TempoSaida = this->TempoAtual;
            
            //Geramos estatísticas relevantes.
            this->GeraEstatistica(freguesDeSaida);

            //Se a fila não está vazia, devemos gerar o tempo de saída do freguês que está sendo servido atualmente.
            if(!this->Fregueses.empty()){
                this->GeraProximaSaida();
            }
            break;
    }
}

void FilaMM1::InicializaFila(){
    this->GeraProximaChegada();
}

//Gera o próximo evento de chegada
void FilaMM1::GeraProximaChegada(){
    //Geramos o intervalo da próxima chegada com a taxa de chegada e o acrescentamos ao tempo atual para obtermos o tempo da próxima chegada.
	double tempoProximaChegada = GeradorAleatorio::Exponencial(this->Utilizacao) + this->TempoAtual;
    //Adicionamos à heap de eventos um novo evento de chegada acontecendo no tempo obtido.
	this->Eventos.push(Evento(TipoEvento::CHEGADA, tempoProximaChegada));
}

//Gera o próximo evento de saída
void FilaMM1::GeraProximaSaida(){
    //Calcula de forma análoga à chegada o tempo da próxima saída, com taxa igual a 1 e acrescentamos um novo evento de saída à heap
	double tempoProximaSaida = GeradorAleatorio::Exponencial(1) + this->TempoAtual;
	this->Eventos.push(Evento(TipoEvento::SAIDA, tempoProximaSaida));
}

//Gera estatísticas relevantes do freguês (a implementar)
void FilaMM1::GeraEstatistica(Fregues fregues){
    // //tempo de espera (média e variância)
    // Wij = fregues.TempoDeEntradaEmServico - fregues.TempoChegada;

    
    // //número de pessoas na fila (média e variância)
    
}

//Imprime informações sobre o estado da fila (apenas pra efeitos de debug, depois será removido)
void FilaMM1::ReportaStatus() {
	std::cout << "A fila possui " << this->Fregueses.size() << " fregueses e esta funcionando ha " << this->TempoAtual << " segundos." << std::endl;
	std::cout << "Lista com " << this->Eventos.size() << " eventos" << std::endl;
}