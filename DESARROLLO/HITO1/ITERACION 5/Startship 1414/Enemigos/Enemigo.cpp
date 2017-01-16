/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Enemigo.cpp
 * Author: Iván
 * 
 * Created on 29 de noviembre de 2016, 17:52
 */

#include "Enemigo.h"

#include <Math.h>

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

Enemigo::Enemigo(ISceneManager* smgr, IVideoDriver* driver, b2World *world, vector3df posicion) {
    
}

Enemigo::Enemigo(const Enemigo& orig) {
}

Enemigo::~Enemigo() {
      
}

void Enemigo::Update(int estado)
{  
}

void Enemigo::Mover(int modo, f32 dt) {

}

vector3df Enemigo::getPos() {
    return pos;
}

void Enemigo::setPos(vector3df pos) {
    maya->setPosition(pos);
}

float Enemigo::getVel() {
    return vel;
}

void Enemigo::setVelocidad() {

    entity->getCuerpo2D()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    entity->getSombraE2D()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
}


void Enemigo::Patrullar(){
    
    
}

bool Enemigo::estaVivo() {
    
     if(entity != NULL)
        return entity->getLive();
    else
        return false;

}
