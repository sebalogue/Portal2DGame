#include "mapa_editor.h"
#include <SDL2/SDL.h>
#include <fstream>
#include <iostream>

bool detectar_colision(SDL_Rect& A, SDL_Rect& B){
	if ((A.x >= (B.x + B.w)) || ((A.x + A.w - 1) < B.x)){
		return false;
	}
	if ((A.y >= (B.y + B.h)) || ((A.y + A.h - 1) < B.y)){
		return false;
	}
	return true;
}
int MapaEditor::convertirPosicion(int a){
	return a >= 0 ? (a / celda) : ((a - celda + 1) / celda); //Guarda en posiciones unitarias
}
MapaEditor::MapaEditor(int ancho_cam, int alto_cam, int celda, Cursor& cursor) :
					   celda(celda), cursor(cursor){
	camara = {0, 0, ancho_cam, alto_cam};
	estado_logico = false;
}

void MapaEditor::agregar(int x, int y, Pieza pieza){
	mapa[x][y] = std::move(pieza);
}

void MapaEditor::quitar(int x, int y){
	//Observacion: El mapa de la posicion x no se borra, a pesar de que quede vacio
	auto it = mapa.find(x);
	if (it == mapa.end()){
		return;
	}
	auto it_interno = mapa[x].find(y);
	if (it_interno == mapa[x].end()){
		return;
	}
	mapa[x].erase(it_interno);
}

void MapaEditor::render(){
	//Recorro el mapa de mapas...
	for (auto it = mapa.begin(); it != mapa.end(); ++it){
		for (auto it_interno = (it->second).begin(); it_interno != (it->second).end(); ++it_interno){
			if (!estado_logico || (it_interno->second.esLogica()) == estado_logico){
				SDL_Rect destino = {it->first * celda /*X*/, it_interno->first * celda /*Y*/,
									it_interno->second.getAncho() * celda,
									it_interno->second.getAlto() * celda};
				if (detectar_colision(camara, destino)){
					destino.x -= camara.x;
					destino.y -= camara.y;
					it_interno->second.render(destino);
				}
			}
		}
	}
}

void MapaEditor::recibirEvento(SDL_KeyboardEvent& evento){
	switch (evento.keysym.sym){
		case(SDLK_UP):
			camara.y -= 10;
			break;
		case(SDLK_DOWN):
			camara.y += 10;
			break;
		case(SDLK_RIGHT):
			camara.x += 10;
			break;
		case(SDLK_LEFT):
			camara.x -= 10;
			break;
		case(SDLK_l):
			estado_logico = !estado_logico;
			break;
		case(SDLK_c):
			if (estado_logico){
				conectar();
				cursor.resetClick();
			}
	}
}

void MapaEditor::recibirEvento(SDL_MouseButtonEvent& evento){
	int x = evento.x + camara.x;
	int y = evento.y + camara.y;
	int j = convertirPosicion(x);
	int k = convertirPosicion(y);
	if (estado_logico){
		if (mapa[j][k].esLogica()){
			cursor.click(mapa[j][k].getIdentificador());
		}
		return;
	} else {
		if (evento.button == SDL_BUTTON_LEFT){
			agregar(j, k, cursor.actual());
		} else if (evento.button == SDL_BUTTON_RIGHT){
			quitar(j, k);
		}
	}
}

void MapaEditor::conectar(){
	if (cursor.anteultimo_click != -1 && cursor.ultimo_click != -1){
		conexiones[cursor.anteultimo_click] = cursor.ultimo_click;
	}
}

/*
Se guarda con el formato:
<Cantidad de bloques>
<id>
<x y>
.
.
.
<id>
<x y>

*/
void MapaEditor::guardar(std::string nombre){
	/*std::ofstream out(nombre);
	out << 0 << '\n';*/
	unsigned int contador = 0;
	for (auto it = mapa.begin(); it != mapa.end(); ++it){
		for (auto it_interno = (it->second).begin(); it_interno != (it->second).end(); ++it_interno){
			std::cout << (it_interno->second).id << '\n';
			std::cout << it->first << ' ' << it_interno->first << '\n';
			contador++;
		}
	}
	for (auto it = conexiones.begin(); it != conexiones.end(); ++it){
		std::cout << (it->first) << " : " << (it->second) << '\n';
	}
	//out.seekp(std::ios_base::beg);
	std::cout << contador << '\n';
	//out.close();
}
