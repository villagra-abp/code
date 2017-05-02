#pragma once
class TTransform;
class TGraphicEngine;
class TNodo;

class Camara
{
public:
	
	Camara(TGraphicEngine * motorApp, TNodo* jugador = nullptr);
	~Camara();
	float getVelocity();
	float getYaw();
	float getPitch();
	TTransform * Rotation();
	TTransform * Scale();
	TTransform * Translation();
	void setVelocity(float);
	void setYaw(float);
	void setPitch(float);
private:
	float velocity;
	float yaw;
	float pitch;
	TTransform *rotation;
	TTransform *scale;
	TTransform *translation;
};

