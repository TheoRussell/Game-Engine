#pragma once

#include "Scene.h"
#include "ClientHandler.h"

class ResourceManager
{
public:
	ResourceManager();
	void clearDisplay();
	~ResourceManager();
	Model getModel(unsigned int ID);
	std::vector<Model> getModels();
	unsigned int add(Model * m);
	void reset();
	void setMatrices(glm::mat4 _view, glm::mat4 _proj);
	void end();
	void render(unsigned int sceneID);
	void render();
	void render(Scene & scene);
	void render(Scene & s, glm::vec3 player_pos, bool InGame);
	void render(Scene & s, glm::vec3 player_pos);
	bool renderModel(unsigned int shaderID, unsigned int render_mode, unsigned int modelID, glm::vec3 pos);
	bool renderModel(unsigned int shaderID, unsigned int render_mode, unsigned int modelID, glm::vec3 pos, glm::vec3 scale);
	bool renderModel(unsigned int shaderID, unsigned int render_mode, unsigned int modelID, glm::vec3 pos, glm::vec3 scale, float pitch, float yaw, float roll);
	bool renderModel(unsigned int shaderID, unsigned int render_mode, Model & model, glm::vec3 pos, glm::vec3 scale, float pitch, float yaw, float roll);
	bool renderTransformers(unsigned int shaderID, Object & selectedObj, EditMode editMode, unsigned int arrowID);
	bool renderObject(unsigned int shaderID, unsigned int render_mode, Object obj);
	void drawLine(glm::vec3 origin, glm::vec3 end);
	bool renderObject(unsigned int shaderID, Object obj);
	unsigned int addScene(Scene currentScene);
	void setScene(unsigned int ID, Scene currentScene);
	void setViewScene(unsigned int ID);
	void nextViewScene();

	unsigned int newModel(std::string _name, std::string obj, std::string diffuse, std::string specular);
	unsigned int newModel(std::string _name, std::string obj);
	void drawBlock(Shader s, unsigned int ID, glm::vec3 pos);



	Object getActor(std::string name, std::map<std::string, float>* debug);

	std::vector<Object> getActors();

	void destroyActor(std::string name);
	int selectedActor();
	void newActor(Object actor, std::string working_dir);
	void newActor(std::string name, Object actor, std::string working_dir);
	void loadSavedActors(std::string working_dir);
	void setActor(unsigned int id, Object actor);

private:

	std::vector<Object> actors;

	std::vector<Shader> shaders;
	std::vector<Model> models;

	std::vector<Scene> scenes;
	unsigned int currentSceneID = 0;

	glm::mat4 w_view;
	glm::mat4 w_projection;

	
};

