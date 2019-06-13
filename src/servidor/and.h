#ifndef AND_H
#define AND_H

#include <vector>
#include "compuerta_logica.h"
#include "boton.h"

class And : public CompuertaLogica {
	std::vector<CompuertaLogica*> compuertas;
public:
	And();
	void agregar(CompuertaLogica* compuerta);

	bool encendida();
	~And() = default;
};

#endif
