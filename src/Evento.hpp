#ifndef EVENTO
#define EVENTO


enum class TipoEvento{
    CHEGADA, SAIDA
};

class Evento {
    public:
    double TempoOcorrencia;
    TipoEvento Tipo;

    Evento(TipoEvento tipoEvento, double tempo);
    friend bool operator<(Evento evento1, Evento evento2);
};

#endif