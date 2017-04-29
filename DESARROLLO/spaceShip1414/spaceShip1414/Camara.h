#pragma once
class TTransform;
class TGraphicEngine;

class Camara
{
public:
	
	Camara(TGraphicEngine * motorApp);
	~Camara();
	float getVelocity();
	float getYaw();
	float getPitch();
	TTransform * getTransformacion();
	void setVelocity(float);
	void setYaw(float);
	void setPitch(float);
private:
	float velocity;
	float yaw;
	float pitch;

	TTransform *Transform;
};

