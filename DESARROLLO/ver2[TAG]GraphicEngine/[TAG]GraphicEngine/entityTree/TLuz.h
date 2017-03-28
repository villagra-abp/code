#pragma once
#include "TEntidad.h"
#include <glm\vec3.hpp>

class TLuz :
	public TEntidad
{
public:
	TLuz(glm::vec3, glm::vec3);
	TLuz(glm::vec3);
	~TLuz();
	void setIntensidad(glm::vec3);
	glm::vec3 getIntensidad();
	void beginDraw();
	void beginDraw(unsigned int) override;
	void endDraw();
private:
	glm::vec3 intensidad;
};

