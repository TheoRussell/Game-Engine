#pragma once


#include "Camera.h"
#include "PointLight.h"
#include "Model.h"

struct Component {
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	bool visible = true;
	std::string name = "<Empty>";
	bool selected = false;
	unsigned int objID = 0;

	glm::mat4 getTransform() {
		glm::mat4 result = glm::mat4(1.0f);
		result = glm::translate(result, position);
		result = glm::rotate(result, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f)); //X
		result = glm::rotate(result, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f)); //Y
		result = glm::rotate(result, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f)); //Z
		result = glm::translate(result, glm::vec3(0.0f,0.0f,0.0f));
		result = glm::scale(result, scale);
		return result;
	}
	void move(glm::vec3 pos) {
		position += pos;
	}
	void moveTo(glm::vec3 pos) {
		position = pos;
	}

	Component operator-(const Component & other) {
		Component c;
		c.position = position - other.position;
		c.scale = scale - other.scale;
		c.roll = roll - other.roll;
		c.pitch = pitch - other.pitch;
		c.yaw = yaw - other.yaw;
		c.objID = objID - other.objID;
		return c;
	}

	Component operator*(const float & deltaTime) {
		Component c;
		c.position = position * deltaTime;
		c.scale = scale * deltaTime;
		c.roll = roll * deltaTime;
		c.pitch = pitch * deltaTime;
		c.yaw = yaw * deltaTime;
		c.objID = objID * deltaTime;
		return c;
	}

};

struct CamComponent {
	Component componentTransform;
	Camera camera;

	CamComponent operator-(const CamComponent & other) {
		CamComponent c;
		c.componentTransform = componentTransform - other.componentTransform;
		c.camera = camera;
		return c;
	}

	CamComponent operator*(const float & deltaTime) {
		CamComponent c;
		c.componentTransform = componentTransform * deltaTime;
		c.camera = camera;
		return c;
	}
};

struct PLightComponent {
	Component componentTransform;
	PointLight light;
};

struct ModelComponent {
	Component componentTransform;
	unsigned int model;
	Model modelData;
};


class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();
	CamComponent getCamera();
	std::vector<PLightComponent> getPointLights();
	std::vector<ModelComponent> getModels();
	void deleteCamera(unsigned int ID);
	void deletePointLight(unsigned int ID);
	void deleteModel(unsigned int ID);
	unsigned int addModel(ModelComponent mc);
	unsigned int addCamera(CamComponent cam);
	unsigned int addPointLight(PLightComponent pl);
	unsigned int addCamera(Camera camera);
	unsigned int addPointLight(PointLight light);
	unsigned int addModel(unsigned int ID);

	std::vector<CamComponent> cameras;
	std::vector<PLightComponent> pointLights;
	std::vector<ModelComponent> models;


private:

};

