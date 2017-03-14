
#include "Objetos.h"
#include "../../Fisicas/Entity2D.h"


Objetos::Objetos(const vector3df & posicion, const vector3df & rotacion, const vector3df & escala, const int & identificacion, IMeshSceneNode *nodo)
{
	pos = posicion;
	rot = rotacion;
	scale = escala;
	ID = identificacion;
	maya = nodo;


}

Objetos::~Objetos() {

}

vector3df Objetos::getPos()
{
	return pos;
}

vector3df Objetos::getRot()
{
	return rot;
}

vector3df Objetos::getEscala()
{
	return scale;
}

int Objetos::getID()
{
	return ID;
}

void Objetos::setFisica(b2World * world)
{

}
