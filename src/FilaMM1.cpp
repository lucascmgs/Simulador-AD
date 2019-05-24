#include "FilaMM1.hpp"

FilaMM1::FilaMM1(TipoFila tipo, double utilizacao){
    this->Tipo = tipo;
    this->Utilizacao = utilizacao;
}

void FilaMM1::TrataProximoEvento(){
	this->ReportaStatus();
    Evento proximoEvento = this->Eventos.top();
    this->Eventos.pop();
    this->TempoAtual = proximoEvento.TempoOcorrencia;

    switch (proximoEvento.Tipo)
    {
        case TipoEvento::CHEGADA :
            if(this->Fregueses.empty()){
                this->GeraProximaSaida();
            }
            this->Fregueses.push_front(Fregues(this->TempoAtual));
			if (this->Fregueses.size() == 1) {
				this->Fregueses.front().TempoDeEntradaEmServico = TempoAtual;
			}
			this->GeraProximaChegada();
            break;
    
        case TipoEvento::SAIDA:
            Fregues freguesDeSaida = Fregues(0);
            switch(this->Tipo){
                case TipoFila::FCFS :
                    freguesDeSaida = this->Fregueses.back();
                    this->Fregueses.pop_back();
					if (this->Fregueses.size() > 0) {
						this->Fregueses.back().TempoDeEntradaEmServico = this->TempoAtual;
					}
                    break;
                case TipoFila::LCFS :
                    freguesDeSaida = this->Fregueses.front();
                    this->Fregueses.pop_front();
					if (this->Fregueses.size() > 0) {
						this->Fregueses.front().TempoDeEntradaEmServico = this->TempoAtual;
					}
                    break;
            }
            
            this->GeraEstatistica(freguesDeSaida);
            if(!this->Fregueses.empty()){
                this->GeraProximaSaida();
            }
            break;
    }
}

void FilaMM1::InicializaFila(){
    this->GeraProximaChegada();
}

void FilaMM1::GeraProximaChegada(){
	double tempoProximaChegada = GeradorAleatorio::Exponencial(this->Utilizacao) + this->TempoAtual;
	this->Eventos.push(Evento(TipoEvento::CHEGADA, tempoProximaChegada));
}

void FilaMM1::GeraProximaSaida(){
	double tempoProximaSaida = GeradorAleatorio::Exponencial(1) + this->TempoAtual;
	this->Eventos.push(Evento(TipoEvento::SAIDA, tempoProximaSaida));
}

void FilaMM1::GeraEstatistica(Fregues fregues){
    
}

void FilaMM1::ReportaStatus() {
	std::cout << "A fila possui " << this->Fregueses.size() << " fregueses e esta funcionando ha " << this->TempoAtual << " segundos." << std::endl;
	std::cout << "Lista com " << this->Eventos.size() << " eventos" << std::endl;
}