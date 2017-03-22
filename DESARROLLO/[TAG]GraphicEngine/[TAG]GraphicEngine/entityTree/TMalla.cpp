#include "TMalla.h"
#include <iostream>
#include "../resourceManager/TRecursoMalla.h"

TMalla::TMalla()
{
	
}

TMalla::TMalla(std::string path)
{
	this->cargarMalla(path);
}

TMalla::~TMalla()
{
	std::cout << "TMalla Destroyed" << std::endl;
}

void TMalla::cargarMalla(std::string path)
{
	if(this->malla == nullptr)
	{
		this->malla = new TRecursoMalla();
		this->malla->cargarFichero(path);
	}
	else
	{
		this->malla->cargarFichero(path);
	}
}


void TMalla::beginDraw()
{
	this->malla->draw();
}

void TMalla::endDraw()
{
}
