#ifndef ANGULO_DOSSETENTA_H
#define ANGULO_DOSSETENTA_H

#include "angulo.h"
#include "Box2D/Box2D.h"

class AnguloDosSetenta : public Angulo{
public:
	AnguloDosSetenta();
	b2Vec2 obtenerPosPortal(b2Vec2& dif_choque);
	b2Vec2 obtenerNormalPortal(b2Vec2& dif_choque);
	b2PolygonShape obtenerTriangulo();
	int getId();
	~AnguloDosSetenta() = default;
};

#endif