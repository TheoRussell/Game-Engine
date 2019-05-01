#include "ResourceManager.h"



ResourceManager::ResourceManager()
{
	//The default shaders used by my engine.
	Shader LightShader("src\\shaders\\vertex.vert", "src\\shaders\\fragment.frag");
	shaders.push_back(LightShader);

	Shader BaseShader("src\\shaders\\vertex.vert", "src\\shaders\\fragment.frag");
	shaders.push_back(BaseShader);

	//Adds a defualt model.
	newModel("DEFAULT","src\\models\\block.obj", "default_diffuse.png", "default_specular.png");
	//Clears the viewport.
	glClearColor(0.1f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void ResourceManager::clearDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

ResourceManager::~ResourceManager()
{
	end();
}

void ResourceManager::end() {
	//Deletes the OpenGL shader programs.
	for (Shader s : shaders) {
		s.deleteProgram();
	}
}

void ResourceManager::reset() {
	models.clear();
}

Model ResourceManager::getModel(unsigned int ID)
{
	return models[ID];
}

std::vector<Model> ResourceManager::getModels() {
	return models;
}

unsigned int ResourceManager::addModel(Model* m) {
	models.push_back(*m);
	return models.size() - 1;
}



void ResourceManager::setMatrices(glm::mat4 _view, glm::mat4 _proj) {
	//Updates projection matrices for the viewport.
	w_view = _view;
	w_projection = _proj;
}



void ResourceManager::render(Scene &s, glm::vec3 player_pos, bool InGame) {
	/* Render here */
	glClearColor(s.voidCol.x,s.voidCol.y,s.voidCol.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //reset all of the depth planes.


	//Sets shader program data.
	for (unsigned int i = 0; i < 2; i++) { // 0 & 1 since those are the 2 default shaders.
		shaders[i].use();

		shaders[i].setI("render_mode", 1);
		shaders[i].setM4FV("view", w_view);
		shaders[i].setV3F("viewPos", player_pos.x, player_pos.y, player_pos.z);
		shaders[i].setM4FV("projection", w_projection);

		//Adds directional light.
		shaders[i].setV3F("l_directional.direction", 0.0f, -1.0f, 0.0f);
		shaders[i].setV3F("l_directional.ambient", s.d_ambient.x, s.d_ambient.y, s.d_ambient.z);
		shaders[i].setV3F("l_directional.diffuse", s.d_diffuse.x, s.d_diffuse.y, s.d_diffuse.z);
		shaders[i].setV3F("l_directional.specular", s.d_specular.x, s.d_specular.y, s.d_specular.z);
		//Binding point light data.
		unsigned int index = 0;
		bool light = true;
		for (Object obj : s.getObjects()) {
			for (PLightComponent pl : obj.components.getPointLights()) {
				//PL must be active and visible.
				if (pl.light.isActive() && pl.componentTransform.visible) {
					shaders[i].setV3F("l_points[" + std::to_string(index) + "].position", pl.componentTransform.position.x + obj.pos.x, pl.componentTransform.position.y + obj.pos.y, pl.componentTransform.position.z + obj.pos.z);
					shaders[i].setV3F("l_points[" + std::to_string(index) + "].ambient", pl.light.getAmbient().x, pl.light.getAmbient().y, pl.light.getAmbient().z);
					shaders[i].setV3F("l_points[" + std::to_string(index) + "].diffuse", pl.light.getDiffuse().x, pl.light.getDiffuse().y, pl.light.getDiffuse().z);
					shaders[i].setV3F("l_points[" + std::to_string(index) + "].specular", pl.light.getSpecular().x, pl.light.getSpecular().y, pl.light.getSpecular().z);
					shaders[i].setF("l_points[" + std::to_string(index) + "].constant", 1.0f);
					shaders[i].setF("l_points[" + std::to_string(index) + "].linear", pl.light.linear);
					shaders[i].setF("l_points[" + std::to_string(index) + "].quadratic", pl.light.quadratic);
					index++;
					light = true;
				}
			}
		}
		if (index < 100) {
			//Clears point lights if they aren't in the scene anymore.
			if (light) {
				for (int lights = index; lights < 2; lights++) {
					shaders[i].setV3F("l_points[" + std::to_string(lights) + "].position", 0.0f, 0.0f, 0.0f);
					shaders[i].setV3F("l_points[" + std::to_string(lights) + "].ambient", 0.0f, 0.0f, 0.0f);
					shaders[i].setV3F("l_points[" + std::to_string(lights) + "].diffuse", 0.0f, 0.0f, 0.0f);
					shaders[i].setV3F("l_points[" + std::to_string(lights) + "].specular", 0.0f, 0.0f, 0.0f);
					shaders[i].setF("l_points[" + std::to_string(lights) + "].constant", 1.0f);
					shaders[i].setF("l_points[" + std::to_string(lights) + "].linear", 0.09f);
					shaders[i].setF("l_points[" + std::to_string(lights) + "].quadratic", 0.032f);
				}

				light = false;
			}
		}

	}

	//This opengl function means that the inside of models will not be rendered (only the faces that have normals out of the surface).
	//This improves the run-time.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	shaders[1].use();
	glm::mat4 model(1.0f);

	if (s.getObjectsCount() > 0) {

		unsigned int oIndex = 0;
		for (Object o : s.getObjects()) {

			unsigned int mIndex = 0;
			for (ModelComponent comp : o.components.getModels()) {
				model = glm::mat4(1.0f);

				//Parent object transform (global).
				model = glm::translate(model, o.pos);
				model = glm::rotate(model, glm::radians(o.roll), { 1.0f, 0.0f, 0.0f });
				model = glm::rotate(model, glm::radians(-o.yaw), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(o.pitch), { 0.0f, 0.0f, 1.0f });
				model = glm::scale(model, o.sca);
				//Component transformation (local).
				model = glm::translate(model, comp.componentTransform.position);
				model = glm::rotate(model, glm::radians(comp.componentTransform.roll), { 1.0f, 0.0f, 0.0f }); //Z roll -- o.GetForward()
				model = glm::rotate(model, glm::radians(-comp.componentTransform.yaw), glm::vec3(0.0f, 1.0f, 0.0f)); //Y yaw
				model = glm::rotate(model, glm::radians(comp.componentTransform.pitch), { 0.0f, 0.0f, 1.0f }); //X pitch -- o.getRight()
				model = glm::scale(model, comp.componentTransform.scale);


				//Rendering model.
				shaders[1].setM4FV("model", model);
				comp.modelData.draw(shaders[1]);

				o.components.models[mIndex] = comp;
				mIndex++;
			}
			//Displaying a sphere if there is no model and editing project.
			if (o.components.getModels().size() == 0) {
				if (!InGame) {
					renderModel(1, 1, 10, o.pos, {0.1f, 0.1f, 0.1f});
				}

			}

			s.objects[oIndex] = o;
			oIndex++;
		}
	}

	glDisable(GL_CULL_FACE);




}

bool ResourceManager::renderModel(unsigned int shaderID, unsigned int render_mode, unsigned int modelID, glm::vec3 pos) {
	return renderModel(shaderID, render_mode, modelID, pos, glm::vec3(1.0f, 1.0f, 1.0f));
}

bool ResourceManager::renderModel(unsigned int shaderID, unsigned int render_mode, unsigned int modelID, glm::vec3 pos, glm::vec3 scale) {
	return renderModel(shaderID, render_mode, modelID, pos, scale, 0.0f,0.0f,0.0f);
}

bool ResourceManager::renderModel(unsigned int shaderID, unsigned int render_mode, unsigned int modelID, glm::vec3 pos, glm::vec3 scale, float pitch, float yaw, float roll) {
	return renderModel(shaderID, render_mode, models[modelID], pos, scale, pitch, yaw, roll);
}


bool ResourceManager::renderModel(unsigned int shaderID, unsigned int render_mode, Model &inputModel, glm::vec3 pos, glm::vec3 scale, float pitch, float yaw, float roll) {
	//Very similar to the render procedure however, it does not have a scene object to be position relative to. It is also only a single model rather than multiple components.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	try {
		shaders[shaderID].use();
		shaders[shaderID].setI("render_mode", render_mode);

		glm::mat4 model;
		model = glm::mat4(1.0f);
		model = glm::translate(model, pos);
		model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f)); //X
		model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f)); //Y
		model = glm::rotate(model, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f)); //Z
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, scale);

		shaders[shaderID].setM4FV("model", model);

		try {
			inputModel.draw(shaders[shaderID], render_mode);
		}
		catch (std::exception ex) {
			std::cout << "could not render model!" << std::endl;
		}


		return true;
	}
	catch (std::exception ex) {
		std::cout << "Model render error: " << ex.what() << std::endl;
		return false;
	}
}


bool ResourceManager::renderTransformers(unsigned int shaderID, Object &selectedObj, EditMode editMode, unsigned int arrowID) {
	//Rendering the selection arrows that appear at objects.
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::vec3 selectedPos = selectedObj.pos;
	unsigned int BASEID = arrowID;
	if (editMode == MOVE) {
		//Arrow heads.
		renderModel(0, 1, BASEID, selectedPos, glm::vec3(1.0f,1.0f,1.0f), 0.0f, 0.0f, 0.0f);
		renderModel(0, 1, BASEID+1, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 90.0f, 0.0f, 0.0f);
		renderModel(0, 1, BASEID+2, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, -90.0f);
		return true;
	}
	else if (editMode == SCALE) {
		//Block heads.
		renderModel(1, 1, BASEID+3, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 0.0f);
		renderModel(1, 1, BASEID+4, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 90.0f, 0.0f, 0.0f);
		renderModel(1, 1, BASEID+5, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, -90.0f);
		return true;
	}
	else if (editMode == ROTATE) {
		//Circle transformers.
		renderModel(1, 1, BASEID+6, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 90.0f, 0.0f, 00.0f);
		renderModel(1, 1, BASEID+7, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, -90.0f);
		renderModel(1, 1, BASEID+8, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 90.0f, 0.0f);
		return true;
	}
	return false;
}



//Models.
unsigned int ResourceManager::newModel(std::string _name, std::string obj, std::string diffuse, std::string specular) {
	//Pushes textures to vector.
	std::vector < std::string > textures;
	if (diffuse.length() > 0) {
		textures.push_back(diffuse);
	}
	if (specular.length() > 0) {
		textures.push_back(specular);
	}
	//Generates a new material object based on the textures.
	Material newMat;
	newMat.texture_paths = textures;

	//Instantiating a new model.
	Model model(_name, &obj[0], newMat);
	//Pushes model to models vector.
	models.push_back(model);
	//Returns model index.
	return models.size() - 1;
}

unsigned int ResourceManager::newModel(std::string _name, std::string obj) {
	Model model(_name, &obj[0]);

	models.push_back(model);
	return models.size() - 1;
}

//Actors.
Object ResourceManager::getActor(std::string name, std::map<std::string, float>* debug) {
	//Returns the requested actor.
	for (Object a : actors) {
		if (a.name == name) {
			return a;
		}
	}
	debug->insert(std::pair<std::string, float>("Could not find actor with name " + name + "!", 0.0f));

	return Object();
}

std::vector<Object> ResourceManager::getActors() {
	return actors;
}

void ResourceManager::destroyActor(std::string name) {
	//Iterates through the actors vector until an actor is found with the parameter name.
	unsigned int index = 0;
	for (Object act : actors) {
		if (act.name == name) {
			//Erases the actor from the list.
			actors.erase(actors.begin() + index);
			if (index > 0) {
				//Selects the next actor in the list.
				index--;
				actors[index].select();
			}
			break;
		}
		index++;
	}
}

int ResourceManager::selectedActor() {
	//iterates through each actor till the selected actor is found. Returns the actors index.
	unsigned int index = 0;
	for (Object act : actors) {
		if (act.isSelected()) {
			return index;
		}
		index++;
	}
	return -1;
}


void ResourceManager::newActor(Object actor, std::string working_dir)
{
	newActor(actor.name, actor, working_dir);
}

void ResourceManager::newActor(std::string name, Object actor, std::string working_dir)
{
	actor.name = name;
	unsigned int count = 0;
	//Generating a name for the actor.
	for (Object act : actors) {
		if (act.name == name) {
			count++;
		}
	}
	if (count > 0) {
		actor.name = name + "(" + std::to_string(count) + ")";
	}
	actor.pos = glm::vec3(0.0f,0.0f,0.0f);
	actor.deselect();
	//Adding actor to list of current actors loaded.
	actors.push_back(actor);
	//Saving actor to file.
	std::ofstream file = std::ofstream(working_dir + "Actors\\" + actor.name + ".actor", std::ios::binary | std::ios::out);
	if (file.good()) {
		//Using the scene class functions as these have the same purpose.
		Scene s;
		s.saveObject(actor, file);
	}
	file.close();
}

void ResourceManager::loadSavedActors(std::string working_dir) {
	actors.clear();
	std::string fileName;
	Scene s;

	for (auto & p : std::experimental::filesystem::directory_iterator(working_dir + "Actors\\")) {
		//Iterates through each document and searches for .actor files.
		fileName = p.path().string();
		if (fileName.find(".actor") != std::string::npos) {
			//Loads the actor from the file.
			std::ifstream file = std::ifstream(fileName, std::ios::binary | std::ios::in);
			if (file.good()) {
				actors.push_back(s.loadObject(file, true));
			}
		}
	}
}

void ResourceManager::setActor(unsigned int id, Object actor)
{
	if (id < actors.size()) {
		actors[id] = actor;
	}

}
