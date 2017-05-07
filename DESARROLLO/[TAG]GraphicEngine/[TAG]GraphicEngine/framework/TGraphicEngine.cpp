#include "TGraphicEngine.h"
#include <iostream>
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "..\resourceManager\TGestorRecursos.h"
#include "..\entityTree\TEntidad.h"
#include "..\entityTree\TNodo.h"
#include "..\entityTree\TTransform.h"
#include "..\entityTree\TCamara.h"
#include "..\entityTree\TLuz.h"
#include "..\entityTree\TMalla.h"
#include "movimentHandler.h"


TGraphicEngine::TGraphicEngine() : shader(), aspect_ratio{}, window{}, registroCamaras(), registroLuces(), lastTime{ 0 }
{
	escena = new TNodo(nullptr);
	gestorRecursos = new TGestorRecursos();
	move = new movimentHandler();
}

TGraphicEngine::~TGraphicEngine()
{
	delete escena;
	escena = nullptr;
	delete gestorRecursos;
	gestorRecursos = nullptr;
	std::cout << "Facade Destroted" << std::endl;
}

TNodo * TGraphicEngine::crearNodo(TNodo* padre, TEntidad* entidad)
{
	return new TNodo(padre, entidad);
}

TTransform * TGraphicEngine::crearTransform()
{
	return new TTransform();
}

TCamara * TGraphicEngine::crearCamara(bool pe, float left, float right, float bottom, float top, float near, float far, bool a)
{
	TCamara* c = new TCamara(pe, left, right, bottom, top, near, far);
	if (a) 
	{
		c->activar();
	}
	return c;
}

TCamara * TGraphicEngine::crearCamaraS(bool pe, float left, float right, float bottom, float top, float near, float far, bool a)
{
	TCamara* c = new TCamara(pe, left, right, bottom, top, near, far);
	if (a)
	{
		c->activar();
	}
	c->setTipo(2);
	return c;
}

TCamara * TGraphicEngine::crearCamara(bool pe, float fovy, float aspect, float near, float far, bool a)
{
	TCamara* c = new TCamara(pe, fovy, aspect, near, far);
	if (a)
	{
		c->activar();
	}
	return c;
}

TCamara * TGraphicEngine::crearCamaraS(bool pe, float fovy, float aspect, float near, float far, bool a)
{
	TCamara* c = new TCamara(pe, fovy, aspect, near, far);
	if (a)
	{
		c->activar();
	}
	c->setTipo(2);
	return c;
}

TCamara * TGraphicEngine::crearCamara(float fovy, float aspect, float near, float far)
{
	return new TCamara(fovy, aspect, near, far);
}

TLuz * TGraphicEngine::crearLuz(float x, float y, float z, bool a)
{
	TLuz* l = new TLuz(x, y, z);
	if (a)
	{
		l->activar();
	}

	return l;
}

TMalla * TGraphicEngine::crearMalla(std::string fichero)
{
	return new TMalla(fichero, gestorRecursos);
}

TNodo * TGraphicEngine::nodoRaiz()
{
	return escena;
}

GLFWwindow * TGraphicEngine::getGLFWwindow()
{
	return window;
}

bool TGraphicEngine::init(std::string title, int width, int height, bool full_screen)
{
	aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		return false;
	}

	window = glfwCreateWindow(width, height, title.c_str(), full_screen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowCloseCallback(window, close_callback);
	glfwSetFramebufferSizeCallback(window, resize_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return false;
	}

	glfwSetWindowUserPointer(window, this);

	return true;
}

void TGraphicEngine::run()
{
	onstart();
	glfwSetTime(0.0);

	while (!glfwWindowShouldClose(window))
	{
		draw(getLastTime());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

void TGraphicEngine::info()
{
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl << std::endl;
}

void TGraphicEngine::addRegistroLuz(TNodo* l)
{
	if (l)
	{
		registroLuces.push_back(l);
	}
}

void TGraphicEngine::addRegistroCamara(TNodo * c)
{
	if (c)
	{
		registroCamaras.push_back(c);
	}
}

movimentHandler* TGraphicEngine::getMovimentHandler()
{
	return move;
}

void TGraphicEngine::setPlayerMove(player * j)
{
	move->setPlayer(j);
}

double TGraphicEngine::getLastTime()
{
	return lastTime;
}

void TGraphicEngine::setLastTime(double t)
{
	lastTime = t;
}

void TGraphicEngine::cambiarCamaraActiva(char m)
{
	if (camaraActiva) {
		camaraActiva->desactivar();
	}
	if (registroCamaras.size() > m) {
		static_cast<TCamara*>(registroCamaras.at(m)->getEntidad())->activar();
		camaraActiva = static_cast<TCamara*>(registroCamaras.at(m)->getEntidad());
	}
}

TNodo * TGraphicEngine::addMalla(std::string path, TNodo * nodoPadre)
{
	TTransform * rotation = crearTransform();
	TTransform * scale = crearTransform();
	TTransform * translation = crearTransform();
	TNodo* nodoRotation;
	if (nodoPadre == nullptr) 
	{
		nodoRotation = crearNodo(nodoRaiz(), rotation);
	}
	else
	{
		nodoRotation = crearNodo(nodoPadre, rotation);
	}
	TNodo* nodoScale = crearNodo(nodoRotation, scale);
	TNodo* nodoTranslation = crearNodo(nodoScale, translation);
	TNodo* nodoMalla;
	if (path.empty())
	{
		nodoMalla = crearNodo(nodoTranslation, crearMalla("resourse/models/box.obj"));
	}
	else 
	{
		nodoMalla = crearNodo(nodoTranslation, crearMalla(path));
	}
	return nodoMalla;
}

TNodo * TGraphicEngine::addCamara(char tipo, bool per, bool act, TNodo * nodoPadre, float x, float y, float z, float a, float b, float c)
{
	TTransform *transfRC = crearTransform();
	TTransform *transfEC = crearTransform();
	TTransform *transfTC = crearTransform();
	TNodo* nodoTransfRC = crearNodo(nodoPadre, transfRC);
	TNodo* nodoTransfEC = crearNodo(nodoTransfRC, transfEC);
	TNodo* nodoTransfTC = crearNodo(nodoTransfEC, transfTC);
	TNodo* nodoCamara;
	if (tipo == 2) {
		if (!per) {
			nodoCamara = crearNodo(nodoTransfTC, crearCamaraS(per, x, y, z, a, b, c, act));
		}
		else {
			nodoCamara = crearNodo(nodoTransfTC, crearCamaraS(per, x, y, z, a, act));
		}
	}
	else if(tipo == 1) {
		if (!per) {
			nodoCamara = crearNodo(nodoTransfTC, crearCamaraS(per, x, y, z, a, b, c, act));
		}
		else {
			nodoCamara = crearNodo(nodoTransfTC, crearCamaraS(per, x, y, z, a, act));
		}
	}
	else 
	{
		nodoCamara = crearNodo(nodoTransfTC, crearCamara(x, y, z, a));
	}
	addRegistroCamara(nodoCamara);
	return nodoCamara;
}

TNodo * TGraphicEngine::addCamaraLibre(bool activa)
{
	TTransform *transfRC = crearTransform();
	TTransform *transfEC = crearTransform();
	TTransform *transfTC = crearTransform();
	TNodo* nodoTransfRC = crearNodo(nodoRaiz(), transfRC);
	TNodo* nodoTransfEC = crearNodo(nodoTransfRC, transfEC);
	TNodo* nodoTransfTC = crearNodo(nodoTransfEC, transfTC);
	TNodo* nodoCamara;
	nodoCamara = crearNodo(nodoTransfTC, crearCamara(45.0f, aspect_ratio, 0.1f, 1000.0f));
	addRegistroCamara(nodoCamara);
	move->setCamara(nodoCamara);
	return nodoCamara;
}

TNodo * TGraphicEngine::addCamaraParalelaFija(bool activa)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	TTransform *transfRC = crearTransform();
	TTransform *transfEC = crearTransform();
	TTransform *transfTC = crearTransform();
	TNodo* nodoTransfRC = crearNodo(nodoRaiz(), transfRC);
	TNodo* nodoTransfEC = crearNodo(nodoTransfRC, transfEC);
	TNodo* nodoTransfTC = crearNodo(nodoTransfEC, transfTC);
	TNodo* nodoCamara;
	nodoCamara = crearNodo(nodoTransfTC, crearCamara(false, 0.0f, 1.0f*(width / 4), -1.0f*(height / 4), 0.0f, -370.0f, 370.0f, activa));
	addRegistroCamara(nodoCamara);
	return nodoCamara;
}

TNodo * TGraphicEngine::addCamaraParalelaSeguidora(bool activa, TNodo * nodoPadre)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	TTransform *transfRC = crearTransform();
	TTransform *transfEC = crearTransform();
	TTransform *transfTC = crearTransform();
	TNodo* nodoTransfRC = crearNodo(nodoPadre, transfRC);
	TNodo* nodoTransfEC = crearNodo(nodoTransfRC, transfEC);
	TNodo* nodoTransfTC = crearNodo(nodoTransfEC, transfTC);
	TNodo* nodoCamara;
	nodoCamara = crearNodo(nodoTransfTC, crearCamaraS(false, 0.0f, 1.0f*(width / 4), -1.0f*(height / 4), 0.0f, -370.0f, 370.0f, activa));
	addRegistroCamara(nodoCamara);
	rotarYPR(nodoCamara, 15.0f, 0.0f, 0.0f);
	rotarYPR(nodoCamara, 0.0f, -30.0f, 0.0f);
	trasladar(nodoCamara, -240.0f, 128.0f, 0.0f);
	return nodoCamara;
}

TNodo * TGraphicEngine::addCamaraPerspectivaFija(bool activa)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	TTransform *transfRC = crearTransform();
	TTransform *transfEC = crearTransform();
	TTransform *transfTC = crearTransform();
	TNodo* nodoTransfRC = crearNodo(nodoRaiz(), transfRC);
	TNodo* nodoTransfEC = crearNodo(nodoTransfRC, transfEC);
	TNodo* nodoTransfTC = crearNodo(nodoTransfEC, transfTC);
	TNodo* nodoCamara;
	nodoCamara = crearNodo(nodoTransfTC, crearCamara(true, 45.0f, aspect_ratio, 0.1f, 1000.f, activa));
	addRegistroCamara(nodoCamara);
	return nodoCamara;
}

TNodo * TGraphicEngine::addCamaraPerspectivaSeguidora(bool activa, TNodo * nodoPadre)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	TTransform *transfRC = crearTransform();
	TTransform *transfEC = crearTransform();
	TTransform *transfTC = crearTransform();
	TNodo* nodoTransfRC = crearNodo(nodoPadre, transfRC);
	TNodo* nodoTransfEC = crearNodo(nodoTransfRC, transfEC);
	TNodo* nodoTransfTC = crearNodo(nodoTransfEC, transfTC);
	TNodo* nodoCamara;
	nodoCamara = crearNodo(nodoTransfTC, crearCamaraS(true, 45.f, aspect_ratio, 0.1f, 1000.f, activa));
	addRegistroCamara(nodoCamara);
	rotarYPR(nodoCamara, 10.0f, -30.0f, 0.0f);
	trasladar(nodoCamara, 0.0f, 30.0f, 200.0f);
	return nodoCamara;
}

TNodo * TGraphicEngine::addLuz(TNodo * nodoPadre)
{
	TTransform *transfRL = crearTransform();
	TTransform *transfEL = crearTransform();
	TTransform *transfTL = crearTransform();
	transfTL->trasladar(0, 100, 10);
	TNodo* nodoTransfRL;
	if (nodoPadre == nullptr)
	{
		nodoTransfRL = crearNodo(nodoRaiz(), transfRL);
	}
	else
	{
		nodoTransfRL = crearNodo(nodoPadre, transfRL);
	}
	TNodo* nodoTransfEL = crearNodo(nodoTransfRL, transfEL);
	TNodo* nodoTransfTL = crearNodo(nodoTransfEL, transfTL);
	TNodo* nodoLuz = crearNodo(nodoTransfTL, crearLuz(0.0f, 10.0f, 10.0f, true));
	addRegistroLuz(nodoLuz);
	return nodoLuz;
}

void TGraphicEngine::trasladar(TNodo * nodo, float x, float y, float z)
{
	TTransform * t = static_cast<TTransform*>(nodo->getPadre()->getEntidad());
	t->trasladar(x, y, z);
}

void TGraphicEngine::rotar(TNodo * nodo, float a, float x, float y, float z)
{
	TTransform * r = static_cast<TTransform*>(nodo->getPadre()->getPadre()->getPadre()->getEntidad());
	r->rotar(a, x, y, z);
}

void TGraphicEngine::rotarYPR(TNodo * nodo, float y, float p, float r)
{
	TTransform * ro = static_cast<TTransform*>(nodo->getPadre()->getPadre()->getPadre()->getEntidad());
	ro->rotarYPR(y, p, r);
}

void TGraphicEngine::escalar(TNodo * nodo, float x, float y, float z)
{
	TTransform * e = static_cast<TTransform*>(nodo->getPadre()->getPadre()->getEntidad());
	e->escalar(x, y, z);
}

void TGraphicEngine::resetTransform(TNodo * nodo, char tipo)
{
	switch (tipo)
	{
		case 0:
			(static_cast<TTransform*>(nodo->getPadre()->getEntidad()))->resetMatriz();
			break;
		case 1:
			(static_cast<TTransform*>(nodo->getPadre()->getPadre()->getEntidad()))->resetMatriz();
			break;
		case 2:
			(static_cast<TTransform*>(nodo->getPadre()->getPadre()->getPadre()->getEntidad()))->resetMatriz();
			break;
		default:
			std::cout << "No se puede reniciar la TTransformacion" << std::endl;
	}
}

TNodo * TGraphicEngine::getPadreX(TNodo * hijo, char padre)
{
	if (padre == 0 || hijo->getPadre() == nullptr) {
		return hijo->getPadre();
	}
	else
	{
		return getPadreX(hijo->getPadre(), padre-1);
	}
}

void TGraphicEngine::draw(double time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.use();
	camaraActivada();
	luzActivada();
	this->escena->draw(shader, camaraActiva->getView(), camaraActiva->getProjectionMatrix());
	shader.unUse();
}

void TGraphicEngine::onstart()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);

	shader.compile("Shader/directional.vertex_shader", "Shader/directional.fragment_shader");

	// ocultar el cursor y ubicarlo en el centro de la ventana
	//glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(this->window, 1280 / 2, 720 / 2);
}

void TGraphicEngine::onstop()
{
}

void TGraphicEngine::onresize(int width, int height)
{
	aspect_ratio = std::max(0.0f, width / (float)height);
	glViewport(0, 0, width, height);
}

void TGraphicEngine::camaraActivada()
{
	for (size_t i = 0; i < registroCamaras.size(); i++)
	{
		if (static_cast<TCamara*>(registroCamaras.at(i)->getEntidad())->getActiva())
		{
			glm::mat4 t = static_cast<TTransform*>(registroCamaras.at(i)->getPadre()->getEntidad())->getMatriz();
			glm::mat4 e = static_cast<TTransform*>(registroCamaras.at(i)->getPadre()->getPadre()->getEntidad())->getMatriz();
			glm::mat4 r = static_cast<TTransform*>(registroCamaras.at(i)->getPadre()->getPadre()->getPadre()->getEntidad())->getMatriz();
			if (static_cast<TCamara*>(registroCamaras.at(i)->getEntidad())->getTipo()==2) 
			{
				glm::mat4 tt = static_cast<TTransform*>(registroCamaras.at(i)->getPadre()->getPadre()->getPadre()->getPadre()->getEntidad())->getMatriz();
				static_cast<TCamara*>(registroCamaras.at(i)->getEntidad())->setView(tt*r*e*t);
			}
			else
			{
				static_cast<TCamara*>(registroCamaras.at(i)->getEntidad())->setView((r*e)*t);
			}
			camaraActiva = static_cast<TCamara*>(registroCamaras.at(i)->getEntidad());
			break;
		}
	}
}

void TGraphicEngine::luzActivada()
{
	for (size_t i = 0; i < registroLuces.size(); i++) {
		if (static_cast<TLuz*>(registroLuces.at(i)->getEntidad())->getActiva())
		{
			glm::mat4 t = static_cast<TTransform*>(registroLuces.at(i)->getPadre()->getEntidad())->getMatriz();
			glm::mat4 e = static_cast<TTransform*>(registroLuces.at(i)->getPadre()->getPadre()->getEntidad())->getMatriz();
			glm::mat4 r = static_cast<TTransform*>(registroLuces.at(i)->getPadre()->getPadre()->getPadre()->getEntidad())->getMatriz();
			static_cast<TLuz*>(registroLuces.at(i)->getEntidad())->renderLuz((r*e)*t, shader, camaraActiva->getView(), camaraActiva->getProjectionMatrix());
		}
	}
}

void TGraphicEngine::error_callback(int error, const char * description)
{
	std::cerr << "Error: " << error << ", " << description << std::endl << std::endl;
}

void TGraphicEngine::close_callback(GLFWwindow *window)
{
	TGraphicEngine* win_app = getTGraphicEngineApp(window);
	win_app->onstop();
}

void TGraphicEngine::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	TGraphicEngine* win_app = getTGraphicEngineApp(window);
	double currentTime = glfwGetTime();
	win_app->getMovimentHandler()->onKey(window, key, scancode, action, mods, currentTime-win_app->getLastTime(), win_app);
	win_app->setLastTime(currentTime);
}

void TGraphicEngine::resize_callback(GLFWwindow * window, int width, int height)
{
	TGraphicEngine* win_app = getTGraphicEngineApp(window);
	win_app->onresize(width, height);
}

void TGraphicEngine::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	TGraphicEngine* win_app = getTGraphicEngineApp(window);
	win_app->getMovimentHandler()->onMouse(window, xpos, ypos, win_app);
}

inline TGraphicEngine * TGraphicEngine::getTGraphicEngineApp(GLFWwindow * window)
{
	return static_cast<TGraphicEngine*>(glfwGetWindowUserPointer(window));
}
