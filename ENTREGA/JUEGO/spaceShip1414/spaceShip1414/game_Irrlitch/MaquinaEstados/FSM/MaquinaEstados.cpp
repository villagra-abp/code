#include "MaquinaEstados.h"
#include "Estados.h"
#include <iostream>

MaquinaEstados::MaquinaEstados()
{

}


MaquinaEstados::~MaquinaEstados()
{
}

void MaquinaEstados::addEstado(Estados * nombreEstados, bool activo)
{
	listaEstados.push_back(nombreEstados);

	if (activo)
	{
		nombreEstados->setEstadoActivo(true);
		nombreEstados->inicializarEstado();
	}
}

void MaquinaEstados::cambiaEstado(const std::string nombre)
{
	if (!listaEstados.empty())
	{
		for (std::size_t i = 0; i < listaEstados.size(); i++)
		{
			if (nombre == listaEstados[i]->getEstado())
			{

				if (listaEstados[i]->getEstadoActivo() != NULL)
				{
					this->getEstadoActivo()->limpiarEstado();
				}
				listaEstados[i]->inicializarEstado();
			}
		}
	}
}

Estados * MaquinaEstados::getEstadoActivo()
{
	if (!listaEstados.empty())
	{
		for (std::size_t i = 0; i < listaEstados.size(); i++)
		{
			if (listaEstados[i]->getEstadoActivo())
			{
				return listaEstados[i];
			}
		}
	}
	return NULL;
}

Estados * MaquinaEstados::getEstado(const std::string nombre)
{
	if (!listaEstados.empty())
	{
		for (std::size_t i = 0; i < listaEstados.size(); i++)
		{
			if (listaEstados[i]->getEstado() == nombre)
			{
				return listaEstados[i];
			}
		}
	}
	return NULL;
}

