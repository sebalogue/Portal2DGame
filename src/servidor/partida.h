#ifndef PARTIDA_H
#define PARTIDA_H

#include "ColaBloqueante.h"
#include "cola_bloqueante_cuerpos.h"
#include "fisica.h"
#include "Thread.h"
#include <vector>
#include "SktAceptador.h"
#include "cola_protegida.h"

class Partida{
private:
    //ColaBloqueante<Input> cola_input;
    ColaProtegidaInput cola_input;
    Fisica& fisica;
    std::vector<Thread*> threads_clientes;
    std::vector<ColaBloqueanteCuerpos*> colas_clientes;
    SktAceptador skt_aceptador;
    bool continuar_juego;
    bool recibir_clientes;
    InfoCuerpoBox2D obtenerInfo(Cuerpo *cuerpo);
public:
    Partida(Fisica& fisica, SktAceptador skt);
    void correrPartida();
    void terminarPartida();
    int recibirClientes();
    void terminarRecibirClientes();
    void comenzar();
    ~Partida();
};

#endif


