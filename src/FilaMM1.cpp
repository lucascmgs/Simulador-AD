#include "FilaMM1.hpp"

//Construtor para a fila M/M/1
FilaMM1::FilaMM1(TipoFila tipo, double utilizacao){
    this->Tipo = tipo;
    this->Utilizacao = utilizacao;
}

void FilaMM1::TrataProximoEvento(){
    //Pega próximo evento da heap de eventos e o remove da heap
    Evento proximoEvento = this->Eventos.top();
    this->Eventos.pop();
    
    //Avança tempo de simulação para o tempo do próximo evento
    this->TempoAtual = proximoEvento.TempoOcorrencia;

    //Tratamos o evento de maneira correspondente ao seu tipo
    switch(proximoEvento.Tipo){
        case TipoEvento::CHEGADA : {
            //Criamos um novo freguês 
            Fregues novoFregues = Fregues(this->TempoAtual);

            //Se a fila está vazia, colocamos o freguês em serviço e geramos sua saída. Caso contrário, colocamos ele na fila de espera.
            if(this->FilaVazia()){
                novoFregues.TempoDeEntradaEmServico = this->TempoAtual;
                this->freguesEmServico = novoFregues;
                this->GeraProximaSaida();
            } else {
                this->Fregueses.push_front(novoFregues);
            }
            //Gera a chegada do próximo freguês
            this->GeraProximaChegada();
            break;
        }
        case TipoEvento::SAIDA : {
            //Atribuimos um tempo de saída para o freguês que está saindo
            this->freguesEmServico.TempoSaida = this->TempoAtual;
            //Geramos as estatísticas obtidas com a saíde desse freguês
            this->GeraEstatistica(this->freguesEmServico);

            //Retiramos o freguês de serviço
            this->freguesEmServico = Fregues(-1);

            //Checamos se há mais freguêses para serem servidos e colocamos o próximo freguês em serviço se for o caso
            this->PreparaNovoServico();

            //Se colocamos um novo freguês em serviço, geramos o evento de saída dele.
            if(this->TemServico()){
                this->GeraProximaSaida();
            }

            break;
        }
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
	double tempoProximaSaida = GeradorAleatorio::Exponencial(1.0) + this->TempoAtual;
	this->Eventos.push(Evento(TipoEvento::SAIDA, tempoProximaSaida));
}

//Imprime informações sobre o estado da fila (apenas pra efeitos de debug, depois será removido)
void FilaMM1::ReportaStatus() {
	std::cout << "A fila possui " << this->Fregueses.size() << " fregueses e esta funcionando ha " << this->TempoAtual << " segundos." << std::endl;
	std::cout << "Lista com " << this->Eventos.size() << " eventos" << std::endl;
}

//Gera estatísticas relevantes do freguês (a implementar)
void FilaMM1::GeraEstatistica(Fregues fregues){
    //std::cout<<fregues << std::endl;
    this->quantidadeSaidas++;
    this->temposDeEsperaNaFila += (fregues.TempoDeEntradaEmServico - fregues.TempoChegada);
    this->temposDeAtendimento += (fregues.TempoSaida - fregues.TempoDeEntradaEmServico);
    this->temposDeEsperaTotal += (fregues.TempoSaida - fregues.TempoChegada);
}

double FilaMM1::TempoMedioDeEsperaNaFila(){
    return this->temposDeEsperaNaFila/this->quantidadeSaidas;
}

double FilaMM1::TempoMedioDeAtendimento(){
    return this->temposDeAtendimento/this->quantidadeSaidas;
}

double FilaMM1::TempoMedioDeEsperaTotal(){
    return this->temposDeEsperaTotal/this->quantidadeSaidas;
}



//Métodos privados

//Checa se a fila de fregueses está vazia e não há ninguém no servidor
bool FilaMM1::FilaVazia(){
    if(this->Fregueses.empty() && this->freguesEmServico.TempoChegada == -1.0) {
        return true;
    } else {
        return false;
    }
}

//Checa se a fila de fregueses está vazia
bool FilaMM1::FilaDeEsperaVazia(){
    if(this->Fregueses.empty()){
        return true;
    } else {
        return false;
    }
}

//Checa se tem alguém em serviço
bool FilaMM1::TemServico(){
    if(this->freguesEmServico.TempoChegada == -1){
        return false;
    } else {
        return true;
    }
}

//Escolhe novo freguês para ser servido de acordo com a política de atendimento
void FilaMM1::PreparaNovoServico(){
    if(!this->FilaDeEsperaVazia()){
        switch(this->Tipo){
            case TipoFila::FCFS : {
                this->freguesEmServico = this->Fregueses.back();
                this->Fregueses.pop_back();
                break;
            }
            case TipoFila::LCFS : {
                this->freguesEmServico = this->Fregueses.front();
                this->Fregueses.pop_front();
                break;
            }
        }
        this->freguesEmServico.TempoDeEntradaEmServico = this->TempoAtual;
    }
}