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
    
    double novoTempo = proximoEvento.TempoOcorrencia;

    //Geramos a estatística relacionada ao número de pessoas na fila desde o último evento
    this->GeraEstatisticaNumeroDePessoasNaFila(novoTempo);

    //Avança tempo de simulação para o tempo do próximo evento
    this->TempoAtual = novoTempo;

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
                this->FreguesesNaFilaDeEspera.push_front(novoFregues);
            }
            //Gera a chegada do próximo freguês
            this->GeraProximaChegada();
            break;
        }
        case TipoEvento::SAIDA : {
            //Atribuimos um tempo de saída para o freguês que está saindo
            this->freguesEmServico.TempoSaida = this->TempoAtual;
            //Geramos as estatísticas obtidas com a saíde desse freguês
            this->GeraEstatisticaTempoEsperaNaFila(this->freguesEmServico);

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
	std::cout << "A fila possui " << this->FreguesesNaFilaDeEspera.size() << " fregueses e esta funcionando ha " << this->TempoAtual << " segundos." << std::endl;
	std::cout << "Lista com " << this->Eventos.size() << " eventos" << std::endl;
}

//Gera estatística do número de pessoas na fila
void FilaMM1::GeraEstatisticaNumeroDePessoasNaFila(double novoTempo){
    //Acumulamos a área correspondente ao gráfico (Número de pessoas) x (Tempo)
    double intervaloEntreEventos = novoTempo - this->TempoAtual;
    int tamanhoFilaDeEspera = this->FreguesesNaFilaDeEspera.size();
    this->numeroDePessoasNaFilaVezesTempo += intervaloEntreEventos * tamanhoFilaDeEspera;
    this->quadradosDoNumeroDePessoasNaFilaVezesTempo += intervaloEntreEventos * pow(tamanhoFilaDeEspera, 2);
}

//Gera estatísticas relevantes ao tempo de espera em fila do freguês
void FilaMM1::GeraEstatisticaTempoEsperaNaFila(Fregues fregues){
    //std::cout<<fregues << std::endl;
    this->EstatisticasColetadasTempoEspera++;
    double tempoDeEsperaDoFregues = fregues.TempoDeEntradaEmServico - fregues.TempoChegada;
    this->temposNaFilaDeEspera += tempoDeEsperaDoFregues;
    this->quadradosDosTemposDeEsperaNaFila += pow(tempoDeEsperaDoFregues, 2);

    this->temposDeAtendimento += (fregues.TempoSaida - fregues.TempoDeEntradaEmServico);
    this->temposDeEsperaTotal += (fregues.TempoSaida - fregues.TempoChegada);
}

double FilaMM1::EstimadorMediaDoNumeroDePessoasNaFilaDeEspera(double tempoInicioRodada){
    double tempoTotalRodada = this->TempoAtual - tempoInicioRodada;
    if(tempoTotalRodada < 0){
        throw "Tempo passado para o cálculo de pessoas na fila inválido!";
    }
    return this->numeroDePessoasNaFilaVezesTempo / tempoTotalRodada;
}

//Cálculo do estimador da média do tempo de espera na fila de uma rodada (EWi)
double FilaMM1::EstimadorMediaTempoNaFilaDeEspera(){
    return this->temposNaFilaDeEspera/this->EstatisticasColetadasTempoEspera;
}

double FilaMM1::TempoMedioDeAtendimento(){
    return this->temposDeAtendimento/this->EstatisticasColetadasTempoEspera;
}

double FilaMM1::TempoMedioDeEsperaTotal(){
    return this->temposDeEsperaTotal/this->EstatisticasColetadasTempoEspera;
}

//Cálculo do estimador da variância do tempo de espera na fila de uma rodada (VWi)
double FilaMM1::EstimadorVarianciaDoTempoNaFilaDeEspera(){
    int n = this->EstatisticasColetadasTempoEspera;
    return this->quadradosDosTemposDeEsperaNaFila/(n-1) - pow(this->temposNaFilaDeEspera, 2)/(n*(n-1));
}

//Reseta variaveis que acumulam o tempo de espera e o número de pessoas ao fim de uma rodada
void FilaMM1::ResetaEstatisticasRodada(){
    //Para W
    this->temposNaFilaDeEspera = 0.0;
    this->quadradosDosTemposDeEsperaNaFila = 0.0;
    this->temposDeAtendimento = 0.0;
    this->temposDeEsperaTotal = 0.0;
    this->tempoOcupado = 0.0;
    this->EstatisticasColetadasTempoEspera = 0;

    //TODO: Para Nq

    this->numeroDePessoasNaFilaVezesTempo = 0;

}

//Métodos privados

//Checa se a fila de fregueses está vazia e não há ninguém no servidor
bool FilaMM1::FilaVazia(){
    if(this->FreguesesNaFilaDeEspera.empty() && this->freguesEmServico.TempoChegada == -1.0) {
        return true;
    } else {
        return false;
    }
}

//Checa se a fila de fregueses está vazia
bool FilaMM1::FilaDeEsperaVazia(){
    if(this->FreguesesNaFilaDeEspera.empty()){
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
                this->freguesEmServico = this->FreguesesNaFilaDeEspera.back();
                this->FreguesesNaFilaDeEspera.pop_back();
                break;
            }
            case TipoFila::LCFS : {
                this->freguesEmServico = this->FreguesesNaFilaDeEspera.front();
                this->FreguesesNaFilaDeEspera.pop_front();
                break;
            }
        }
        this->freguesEmServico.TempoDeEntradaEmServico = this->TempoAtual;
    }
}