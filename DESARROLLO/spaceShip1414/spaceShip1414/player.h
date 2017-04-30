#pragma once
#include <glm\vec3.hpp>
class TTransform;
class TGraphicEngine;
class TCamara;
class player
{
public:
	player(TGraphicEngine*);
	~player();
	float getVelocity();
	float getYaw();
	float getPitch();
	TTransform* Rotation();
	TTransform* Scale();
	TTransform* Translation();
	void setVelocity(float);
	void setYaw(float);
	void setPitch(float);
	glm::vec3 getPos();
	glm::vec3 getRot();
	glm::vec3 getScale();
	void setPos(float x, float y, float z);
	void setRot(float x, float y, float z);
	void setScale(float x, float y, float z);
private:
	float velocity;
	float yaw;
	float pitch;
	TTransform *rotation;
	TTransform *scale;
	TTransform *translation;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 escale;
};

