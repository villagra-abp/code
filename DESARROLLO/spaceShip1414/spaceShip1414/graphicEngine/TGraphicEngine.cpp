#include "TGraphicEngine.h"
#include <iostream>
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "resourceManager\TGestorRecursos.h"
#include "entityTree\TEntidad.h"
#include "entityTree\TNodo.h"
#include "entityTree\TTransform.h"
#include "entityTree\TCamara.h"
#include "entityTree\TLuz.h"
#include "entityTree\TMalla.h"
#include "framework\movimentHandler.h"
#include "../Fisicas/Mundo.h"


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

TCamara * TGraphicEngine::crearCamara(bool pe, float xu, float yu, float zu, float xf, float yf, float zf, bool a)
{
	TCamara* c = new TCamara(pe, xu, yu, zu, xf, yf, zf);
	if (a) 
	{
		c->activar();
	}
	return c;
}

TCamara * TGraphicEngine::crearCamara()
{
	return new TCamara();
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

	if (!glfwInit()) return false;

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

void TGraphicEngine::run(Mundo * world)
{
	onstart();
	glfwSetTime(0.0);
	lastTime = 0.0;
	double currentFrame = glfwGetTime();
	double last = currentFrame;

	while (!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = (currentFrame - last);
		last = currentFrame;
		world->getWorldBox2D()->Step(deltaTime, 6, 2); //1.0f/60.0f
		world->getWorldBox2D()->ClearForces();
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

void TGraphicEngine::setCameraMove(Camara * j)
{
	move->setCamara(j);
}

double TGraphicEngine::getLastTime()
{
	return lastTime;
}

double TGraphicEngine::getDT()
{
	return deltaTime;
}

void TGraphicEngine::setLastTime(double t)
{
	lastTime = t;
}

TCamara* TGraphicEngine::getCamaraActiva()
{
	return camaraActiva;
}

void TGraphicEngine::cambiarCamaraActiva(bool m)
{
	if (m) {
		static_cast<TCamara*>(registroCamaras.at(0)->getEntidad())->activar();
		static_cast<TCamara*>(registroCamaras.at(1)->getEntidad())->desactivar();
		camaraActiva = static_cast<TCamara*>(registroCamaras.at(0)->getEntidad());
		camaraActiva->setWindow(window);
	}
	else {
		static_cast<TCamara*>(registroCamaras.at(1)->getEntidad())->activar();
		static_cast<TCamara*>(registroCamaras.at(0)->getEntidad())->desactivar();
		camaraActiva = static_cast<TCamara*>(registroCamaras.at(1)->getEntidad());
		camaraActiva->setWindow(window);
	}
}

glm::vec3 TGraphicEngine::moverCamara()
{

	std::cout << "hle" << std::endl;
	camaraActiva->setWindow(window);
	return camaraActiva->mover();
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

	shader.compile("graphicEngine/Shader/directional.vertex_shader", "graphicEngine/Shader/directional.fragment_shader");

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

	for (size_t i = 0; i < registroCamaras.size(); i++) {
		if (static_cast<TCamara*>(registroCamaras.at(i)->getEntidad())->getActiva())
		{
		
			glm::mat4 t = static_cast<TTransform*>(registroCamaras.at(i)->getPadre()->getEntidad())->getMatriz();
			glm::mat4 e = static_cast<TTransform*>(registroCamaras.at(i)->getPadre()->getPadre()->getEntidad())->getMatriz();
			glm::mat4 r = static_cast<TTransform*>(registroCamaras.at(i)->getPadre()->getPadre()->getPadre()->getEntidad())->getMatriz();
			static_cast<TCamara*>(registroCamaras.at(i)->getEntidad())->setView((r*e)*t);
			camaraActiva = static_cast<TCamara*>(registroCamaras.at(i)->getEntidad());
			camaraActiva->setWindow(this->window);
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
	win_app->getMovimentHandler()->onMouse(window, xpos, ypos);
}

inline TGraphicEngine * TGraphicEngine::getTGraphicEngineApp(GLFWwindow * window)
{
	return static_cast<TGraphicEngine*>(glfwGetWindowUserPointer(window));
}
