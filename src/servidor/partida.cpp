#include "partida.h"
#include "proceso_cliente.h"
#include "SocketError.h"
#include "cuerpo_a_enviar.h"
#include "ids.h"
#include "Timer.h"
#include <iostream>

#define FPS 60
#define TICKS_PER_FRAME 1340/FPS

Partida::Partida(Fisica& fisica, SktAceptador skt): 
                fisica(fisica), skt_aceptador(std::move(skt)){
    this->continuar_juego = true;
    this->recibir_clientes = true;
}

InfoCuerpoBox2D Partida::obtenerInfo(Cuerpo* cuerpo){
    InfoCuerpoBox2D info;
    info.id = cuerpo->getId();
    info.angulo = cuerpo->getAngle();
    info.pos = cuerpo->getPosition();
    info.ancho = cuerpo->getMaxWidth();
    info.alto = cuerpo->getMaxHeight();
    if (info.id == ID_CHELL){
        info.estado = ((Chell*)cuerpo)->obtenerEstado();
        info.orientacion = ((Chell*)cuerpo)->obtenerOrientacion();
    } else {
        info.estado = 0;
        info.orientacion = 0;
    }
    return info;
}


void Partida::correrPartida(){ //
    Timer capTimer;

    while (this->continuar_juego){
        capTimer.start();

        if (!(this->cola_input.empty())){
            Input input = std::move(this->cola_input.front()); // habria que mantener el estado teclado anterior, etc.
            this->cola_input.pop();
            this->fisica.actualizarChell(input.id, input.estado_teclado, input.estado_mouse);
        }/* else {
            Input input;
            this->fisica.actualizar(input.estado_teclado, input.estado_mouse);
        }*/
        this->fisica.actualizar();

        std::vector<Cuerpo*> cuerpos = this->fisica.obtenerCuerpos();

        for (auto it=cuerpos.begin(); it!=cuerpos.end(); it++){
            for (auto c=colas_clientes.begin(); c!=colas_clientes.end(); c++){
                CuerpoAEnviar cuerpo_a_enviar;
                cuerpo_a_enviar.info_cuerpo = obtenerInfo(*it);
                cuerpo_a_enviar.ultimo = false;
                (*c)->push(cuerpo_a_enviar); // aca se esta copiando
            }
        }
        CuerpoAEnviar cuerpo_a_enviar;
        cuerpo_a_enviar.ultimo = true;
        for (auto c=colas_clientes.begin(); c!=colas_clientes.end(); c++){
            (*c)->push(cuerpo_a_enviar); // aca se esta copiando
        }

        int frameTicks = capTimer.getTicks();
        if (frameTicks < TICKS_PER_FRAME) {
            SDL_Delay(TICKS_PER_FRAME - frameTicks);
        }
    }

    for (auto it=colas_clientes.begin(); it!=colas_clientes.end(); it++){
        (*it)->finalizado();
    }

    for (size_t i = 0; i < this->threads_clientes.size(); ++i){
        this->threads_clientes[i]->join();
        delete this->threads_clientes[i];
    }
}

void Partida::terminarPartida(){
    this->continuar_juego = false;
    for (auto it=threads_clientes.begin(); it!=threads_clientes.begin(); it++){
        (*it)->terminar();
    }
}


int Partida::recibirClientes(){ 
    int i = 0;
    while (this->recibir_clientes){
        try{
            Skt acept_skt = std::move(this->skt_aceptador.aceptarCliente());
            ColaBloqueanteCuerpos* c = new ColaBloqueanteCuerpos();
            colas_clientes.push_back(c);
            ProcesoCliente* proceso = new ProcesoCliente(
                                            std::move(acept_skt),
                                            this->cola_input,
                                            colas_clientes[i], i);
            this->threads_clientes.push_back(proceso);

            proceso->start();

        } catch(const SocketError &e){
            if (this->recibir_clientes){ // si no fue forzado
                std::cerr << "Error accepting client:\n";
                std::cerr << e.what();
            }
            break;
        }
        i++;
    }
    return 0;
}

void Partida::terminarRecibirClientes(){
    this->recibir_clientes = false;
    this->skt_aceptador.cerrarCanales();
    this->skt_aceptador.cerrarSocket();
}


void Partida::comenzar(){
    this->recibirClientes();
    this->correrPartida();
}

Partida::~Partida(){}



