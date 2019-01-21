#include "ResourceManager.h"



ResourceManager::ResourceManager()
{
	Shader LightShader("src\\shaders\\vertex.vert", "src\\shaders\\fragment.frag");
	shaders.push_back(LightShader);

	Shader BaseShader("src\\shaders\\vertex.vert", "src\\shaders\\fragment.frag");
	shaders.push_back(BaseShader);

	newModel("DEFAULT","src\\models\\block.obj", "b_grass_diffuse.png", "grass_specular.png");

	glClearColor(0.1f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void ResourceManager::clearDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

ResourceManager::~ResourceManager()
{
}

Model ResourceManager::getModel(unsigned int ID)
{
	return models[ID];
}

std::vector<Model> ResourceManager::getModels() {
	return models;
}

unsigned int ResourceManager::add(Model* m) {
	models.push_back(*m);
	return models.size() - 1;
}

void ResourceManager::reset() {
	models.clear();
}

void ResourceManager::setMatrices(glm::mat4 _view, glm::mat4 _proj) {
	w_view = _view;
	w_projection = _proj;
}

void ResourceManager::end() {
	for each (Shader s in shaders) {
		s.deleteProgram();
	}
}

void ResourceManager::render(unsigned int sceneID) {
	currentSceneID = sceneID;
	render();
}

void ResourceManager::render() {
	Scene s = scenes[currentSceneID];
	render(s);
}

void ResourceManager::render(Scene &s) {
	render(s, glm::vec3(0.0f,0.0f,0.0f));
}

void ResourceManager::render(Scene &s, glm::vec3 player_pos) {
	render(s, player_pos, true);
}

void ResourceManager::render(Scene &s, glm::vec3 player_pos, bool InGame) {
	/* Render here */
	glClearColor(s.voidCol.x,s.voidCol.y,s.voidCol.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //reset all of the depth planes.


	//Objects
	for (unsigned int i = 0; i < 2; i++) {
		shaders[i].use();


		shaders[i].setI("render_mode", 1);
		shaders[i].setM4FV("view", w_view);
		shaders[i].setM4FV("projection", w_projection);
		shaders[i].setV3F("l_directional.direction", 0.0f, -1.0f, 0.0f);
		shaders[i].setV3F("l_directional.ambient", s.d_ambient.x, s.d_ambient.y, s.d_ambient.z);
		shaders[i].setV3F("l_directional.diffuse", s.d_diffuse.x, s.d_diffuse.y, s.d_diffuse.z);
		shaders[i].setV3F("l_directional.specular", s.d_specular.x, s.d_specular.y, s.d_specular.z);
		unsigned int index = 0;
		bool light = true;
		for each (Object obj in s.getObjects()) {
			for each (PLightComponent pl in obj.components.getPointLights()) {
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
	shaders[1].setV3F("viewPos", player_pos.x, player_pos.y, player_pos.z);
	glm::mat4 model(1.0f);

	if (s.getObjectsCount() > 0) {

		unsigned int oIndex = 0;
		for each (Object o in s.getObjects()) {

			unsigned int mIndex = 0;
			for each (ModelComponent comp in o.components.getModels()) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, comp.componentTransform.position + o.pos);
				model = glm::rotate(model, glm::radians(comp.componentTransform.roll + o.roll), glm::vec3(1.0f, 0.0f, 0.0f)); //Z roll

				model = glm::rotate(model, glm::radians(-comp.componentTransform.yaw - o.yaw), glm::vec3(0.0f, 1.0f, 0.0f)); //Y yaw

				model = glm::rotate(model, glm::radians(comp.componentTransform.pitch + o.pitch), glm::vec3(0.0f, 0.0f, 1.0f)); //X pitch

				glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
				model = glm::scale(model, comp.componentTransform.scale * o.sca);

				shaders[1].setM4FV("model", model);
				comp.modelData.draw(shaders[1]);

				o.components.models[mIndex] = comp;
				mIndex++;
			}
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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	try {
		shaders[shaderID].use();
		glm::mat4 model;
		model = glm::mat4(1.0f);
		model = glm::translate(model, pos);
		model = glm::rotate(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f)); //X
		model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f)); //Y
		model = glm::rotate(model, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f)); //Z
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, scale);
		
		shaders[1].setV3F("selected", 0.0f, 0.0f, 0.0f);
		shaders[shaderID].setM4FV("model", model);

		try {
			models[modelID].draw(shaders[shaderID], render_mode);
		}
		catch (std::exception ex) {
			std::cout << "model no longer present! RM" << std::endl;
		}


		return true;
	}
	catch (std::exception ex) {
		std::cout << "Model render error: " << ex.what() << std::endl;
		return false;
	}
}


bool ResourceManager::renderModel(unsigned int shaderID, unsigned int render_mode, Model &inputModel, glm::vec3 pos, glm::vec3 scale, float pitch, float yaw, float roll) {
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
	//WANT TO RENDER ABOVE ALL OTHER OBJECTS.
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::vec3 selectedPos = selectedObj.pos;
	unsigned int BASEID = arrowID;
	if (editMode == MOVE) {
		renderModel(0, 1, BASEID, selectedPos, glm::vec3(1.0f,1.0f,1.0f), 0.0f, 0.0f, 0.0f);
		renderModel(0, 1, BASEID+1, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 90.0f, 0.0f, 0.0f);
		renderModel(0, 1, BASEID+2, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, -90.0f);
		return true;
	}
	else if (editMode == SCALE) {
		renderModel(1, 1, BASEID+3, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 0.0f);
		renderModel(1, 1, BASEID+4, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 90.0f, 0.0f, 0.0f);
		renderModel(1, 1, BASEID+5, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, -90.0f);
		return true;
	}
	else if (editMode == ROTATE) {
		renderModel(1, 1, BASEID+6, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 90.0f, 0.0f, 00.0f);
		renderModel(1, 1, BASEID+7, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, -90.0f);
		renderModel(1, 1, BASEID+8, selectedPos, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 90.0f, 0.0f);
		return true;
	}
	return false;
}

bool ResourceManager::renderObject(unsigned int shaderID, Object obj) {
	return renderObject(shaderID,1, obj);
}

bool ResourceManager::renderObject(unsigned int shaderID, unsigned int render_mode, Object obj) {

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	try {
		shaders[shaderID].use();
		glm::mat4 model;
		model = glm::mat4(1.0f);
		model = obj.getTransform();
		if (obj.isSelected()) {
			shaders[1].setV3F("selected", 0.0f, 0.0f, 0.0f); //Highlighting an object.
		} 
		else {
			shaders[1].setV3F("selected", 0.0f, 0.0f, 0.0f);
		}
		shaders[shaderID].setM4FV("model", model);

		for each (ModelComponent mc in obj.components.models) {
			try {
				models[mc.model].draw(shaders[shaderID], render_mode);
			}
			catch (std::exception ex) {
				std::cout << "model no longer present! RO" << std::endl;
			}

		}


		return true;
	}
	catch (std::exception ex) {
		std::cout << "Object render error: " << ex.what() << std::endl;
		return false;
	}
}

void ResourceManager::drawLine(glm::vec3 origin, glm::vec3 end) {
	shaders[1].use();
	glBegin(GL_LINES);
	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(end.x, end.y, end.z);
	glEnd();
}

unsigned int ResourceManager::addScene(Scene currentScene) {
	scenes.push_back(currentScene);
	currentSceneID = scenes.size() - 1;
	return scenes.size() - 1;
}

void ResourceManager::setScene(unsigned int ID, Scene currentScene) {
	if (scenes.size() >= ID && scenes.size() > 0) {
		scenes[ID] = currentScene;
	}
}

void ResourceManager::setViewScene(unsigned int ID) {
	if (scenes.size() >= ID && scenes.size() > 0) {
		currentSceneID = ID;
	}
}

void ResourceManager::nextViewScene() {
	if (scenes.size() > 1) {
		if (currentSceneID > scenes.size() - 2) {
			currentSceneID = 0;
		}
		else {
			currentSceneID++;
		}
	}
	else {
		currentSceneID = 0;
	}
}

unsigned int ResourceManager::newModel(std::string _name, std::string obj, std::string diffuse, std::string specular) {
	std::vector < std::string > textures;
	if (diffuse.length() > 0) {
		textures.push_back(diffuse);
	}
	if (specular.length() > 0) {
		textures.push_back(specular);
	}
	Material newMat;
	newMat.texture_paths.push_back(diffuse);
	newMat.texture_paths.push_back(specular);

	Model model(_name, &obj[0], newMat);

	//while (true) {
	//	bool duplicate = false;
	//	for each (Model m in models) {
	//		if (m.id == model.id) {
	//			model.genID();
	//			duplicate = true;
	//		}
	//	}
	//	if (!duplicate) {
	//		break;
	//	}
	//}

	models.push_back(model);
	return models.size() - 1;
}

unsigned int ResourceManager::newModel(std::string _name, std::string obj) {
	Model model(_name, &obj[0]);

	models.push_back(model);
	return models.size() - 1;
}


void ResourceManager::drawBlock(Shader s,unsigned int ID, glm::vec3 pos) {
	if (ID  != 0) {
		Model m = getModel(ID - 1);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pos);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		s.setM4FV("model", model);
		m.draw(s);
	}
}


Object ResourceManager::getActor(std::string name, std::map<std::string, float>* debug) {
	for each (Object a in actors) {
		if (a.name == name) {
			return a;
		}
	}
	debug->insert(std::pair<std::string, float>("Could not find actor with name " + name + "!", 0.0f));

	return Object();
}

Object ResourceManager::getActor(std::string name) {
	for each (Object a in actors) {
		if (a.name == name) {
			return a;
		}
	}
	std::cout << "could not find actor with name " << name << std::endl;
	
	return Object();
}

std::vector<Object> ResourceManager::getActors() {
	return actors;
}

void ResourceManager::destroyActor(std::string name) {
	unsigned int index = 0;
	for each (Object act in actors) {
		if (act.name == name) {
			actors.erase(actors.begin() + index);
			if (index > 0) {
				index--;
				actors[index].select();
			}
			break;
		}
		index++;
	}
}

int ResourceManager::selectedActor() {
	unsigned int index = 0;
	for each (Object act in actors) {
		if (act.isSelected()) {
			return index;
		}
		index++;
	}
	return -1;
}

void ResourceManager::newActor()
{
	newActor(Object());
}

void ResourceManager::newActor(std::string name)
{
	newActor(name, Object());
}

void ResourceManager::newActor(Object actor)
{
	newActor(actor.name, actor);
}

void ResourceManager::newActor(std::string name, Object actor)
{
	actor.name = name;
	unsigned int count = 0;
	for each (Object act in actors) {
		if (act.name == name) {
			count++;
		}
		else if (act.name.find(name) != std::string::npos) {
			std::string temp = act.name;
			temp.erase(temp.length() - (temp.length() - name.length()), (temp.length() - name.length()));
			if (temp == name) {
				count++;
			}
		}
	}
	if (count > 0) {
		actor.name = name + "(" + std::to_string(count) + ")";
	}
	actor.pos = glm::vec3(0.0f,0.0f,0.0f);
	actor.deselect();
	actors.push_back(actor);
}

void ResourceManager::setActor(unsigned int id, Object actor)
{
	if (id < actors.size()) {
		actors[id] = actor;
	}

}
