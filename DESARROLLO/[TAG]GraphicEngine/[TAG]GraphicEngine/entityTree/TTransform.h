#pragma once
#include "TEntidad.h"
#include <stack>
class TTransform :
	public TEntidad
{
public:
	TTransform();
	~TTransform();
	glm::mat4 identidad();
	void cargar(glm::mat4&);
	glm::mat4 trasponer(glm::mat4&);
	glm::mat4 invertir(glm::mat4&);
	void trasladar(float, float, float);
	void escalar( float, float, float);
	void rotar(float, float, float, float);
	glm::mat4 multiplicarMatriz(const glm::mat4&, const  glm::mat4&);
	glm::vec4 multiplicarVector(const glm::mat4&, float, float, float);
	void apilar(glm::mat4);
	void desapilar();
	void beginDraw() override;
	void beginDraw(openGLShader&, const glm::mat4&, const glm::mat4&) override;
	void endDraw() override;
	glm::mat4 getMatriz();
private:
	glm::mat4 matriz;
	static std::stack<glm::mat4> pilaMatrices;
};

