
#include <iostream>
#include "servidor.h"
#include "SktAceptador.h"
#include "partida.h"
#include "Box2D/Box2D.h"
#include "disparo.h"
#include "BloqueRoca.h"
#include "BloqueMetal.h"
#include "bloque_metal_diagonal.h"
#include "chell.h"
#include "mundo.h"
#include "estado_teclado.h"
#include "personajes.h"
#include "cuerpo.h"
#include "contact_listener.h"
#include "CoordConverter.h"
#include "portal.h"
#include "boton.h"
#include "estado_logico.h"
#include "compuerta.h"
#include "ids.h"
#include "or.h"
#include "and.h"
#include "Roca.h"
#include "BolaEnergia.h"
#include "EmisorArriba.h"
#include "angulo_cero.h"
#include "angulo_noventa.h"
#include "angulo_ciento_ochenta.h"
#include "angulo_dos_setenta.h"
#include "EstadoChell.h"
#include "Protocolo.h"
#include "fisica.h"
#include "EmisorAbajo.h"
#include "EmisorDerecha.h"
#include "EmisorIzquierda.h"
#include "ReceptorArriba.h"
#include "ReceptorAbajo.h"
#include "ReceptorDerecha.h"
#include "ReceptorIzquierda.h"
#include "Acido.h"
#include "BarraDeEnergia.h"
#include "config.h"
#include "th_aceptador_clientes.h"

#define TECLA_TERMINAR 'q'

Servidor::Servidor(){
}

void Servidor::iniciarMultiPartidas(std::string puerto){
    SktAceptador skt_aceptador(puerto);
    ThAceptadorClientes aceptador_clientes(std::move(skt_aceptador), puerto);
    aceptador_clientes.start();
    
    char tecla = 0;
    while (tecla != TECLA_TERMINAR){
        std::cin >> tecla;
    }
    aceptador_clientes.terminar();
    aceptador_clientes.join();
}


Servidor::~Servidor(){}
