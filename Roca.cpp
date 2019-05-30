//
// Created by santi on 29/05/19.
//

#include "Roca.h"
#include "ids.h"

Roca::Roca(Mundo &mundo, const b2Vec2 &pos) :
    Cuerpo(TAMANIO_ROCA * 2, TAMANIO_ROCA * 2) {
    b2BodyDef cuerpo_def;
    cuerpo_def.type = b2_dynamicBody;
    cuerpo_def.position.Set(pos.x, pos.y);
    cuerpo_def.fixedRotation = true;
    cuerpo = mundo.agregarBody(cuerpo_def);

    b2PolygonShape polygon_shape;
    b2FixtureDef fixture_def;
    fixture_def.shape = &polygon_shape;
    fixture_def.density = 1;

    polygon_shape.SetAsBox(TAMANIO_ROCA, TAMANIO_ROCA);
    cuerpo->CreateFixture(&fixture_def);

    cuerpo->SetUserData(this);
}

Roca::Roca(Roca &&otro) {
    if (this == &otro){
        return;
    }
    maxWidth = otro.maxWidth;
    maxHeight = otro.maxHeight;
    cuerpo = otro.cuerpo;

    otro.maxWidth = 0;
    otro.maxHeight = 0;
    otro.cuerpo = nullptr;

    cuerpo->SetUserData(this);
}

int Roca::getId() {
    return ID_ROCA;
}

const b2Vec2 &Roca::getPosition() {
    return cuerpo->GetPosition();
}

void Roca::empezarContacto(Cuerpo *otro) {
}

void Roca::terminarContacto(Cuerpo *otro) {
}

b2Body *Roca::getBody() {
    return cuerpo;
}