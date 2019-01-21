#pragma once
#include "imgui\imgui.h" //Gui
#include "imgui\imgui_impl_glfw_gl3.h" //Example imgui

#include "ClientHandler.h"
#include "Physics\PhysicsEngine.h"
#include "ResourceManager.h"
#include "Components\Scripts\ScriptManager.h"

#include <filesystem>



#include "UIElements.h"


struct DragPacket {
	Object obj;
	int itemType = 0;
};

enum ClickMode {
	EDIT,
	CREATE
};

class EngineInterface
{
public:
	EngineInterface();
	~EngineInterface();
	void OnLoad(ClientHandler & client, PhysicsEngine & physics, ResourceManager & res, ScriptManager & scripts);
	void OnStart(ClientHandler & client, PhysicsEngine & physics, ResourceManager & res, ScriptManager & scripts);
	void OnUpdate(float deltaTime, ClientHandler & client, PhysicsEngine & physics, ResourceManager & res);
	void changeUI(ClientHandler & client, std::string);
	void deleteUI(ClientHandler & client, std::string name);
	void NormalDesign();
	void Menu(ClientHandler & client, PhysicsEngine & physics, ResourceManager & res, ScriptManager & scripts);
	void debug_write(std::string msg);
	void projects(ClientHandler & client);
	void UIEitem(ClientHandler & client, PhysicsEngine & physics, ResourceManager & res, ScriptManager & scripts);
	bool copyUI();
	void pasteUI();
	void pointToScreen(glm::vec2 & pos, int width, int height);
	void screenToPoint(glm::vec2 & pos);
	void movePoint(glm::vec2 & pos, int width, int height, int xChange, int yChange);
	void scalePoint(glm::vec2 & pos, int width, int height, float xChange, float yChange);
	bool pointInRect(glm::vec2 point, glm::vec2 topLeft, glm::vec2 lowRight, int width, int height);
	bool pointInRect(glm::vec2 point, glm::vec2 topLeft, glm::vec2 lowRight);
	void findSelection();
	void userInterface(ClientHandler & client, PhysicsEngine & physics, ResourceManager & res, ScriptManager & scripts);
	void modifyScriptRef(ClientHandler & client, PhysicsEngine & physics, ResourceManager & res, ScriptManager & scripts);
	void playStopGame(ClientHandler & client);
	void update(bool & inGame, ClientHandler & client, PhysicsEngine & physics, ResourceManager & res, ScriptManager & script);
	bool inScene(ImVec2 mousePos);
	void DragNDrop(ClientHandler & client, PhysicsEngine & physics, ResourceManager & res, ScriptManager & scripts);
	void showReload(ClientHandler & client);
	void importItem(ClientHandler & client, PhysicsEngine & physics, ResourceManager & res, ScriptManager & scripts);
	void displayUserInterfaces(ClientHandler & client);
	void displayScenes(ClientHandler & client, PhysicsEngine & physics);
	void displayWorld(ClientHandler & client, PhysicsEngine & physics);
	void displaySelectedObj(ClientHandler & client, PhysicsEngine & physics, ResourceManager & res);
	std::string inputText(std::string value, std::string id);
	glm::vec4 inputColour(std::string id, glm::vec4 original, bool opaque);
	glm::vec3 inputVec3(std::string id, glm::vec3 original);
	float inputFloat(std::string id, float original, float smallStep, float quickStep);
	void deselectAllComponents(Object & obj);
	void displayNewComponent(ClientHandler & client, PhysicsEngine & physics, ResourceManager & res, ScriptManager & scripts);

	void displayActors(ResourceManager & res);
	

	int xDisplacement = 0;
	int yDisplacement = 0;


	std::map<std::string, float> debug_messages;


	std::map < ComponentScript*, std::map<std::string, UILabel*> > scriptPointer_labels;
	std::map < ComponentScript*, std::map<std::string, UIPercentBar*> > scriptPointer_percBar;
	UIElements UIE;
private:
	int uiWidth = 1920;
	int uiHeight = 1080;
	//Storage.
	float UIEzoom_s = 1.0f;
	int UIEx_s = 0;
	int UIEy_s = 0;
	//Usage
	float UIEzoom = 1.0f;
	int UIEx = 0; //DISPLACEMENT.
	int UIEy = 0;
	int UIExo = 0; //DRAGGING.
	int UIEyo = 0;
	int UIEdown = 0; //Timer.
	float UIEpolyX = 0;
	float UIEpolyY = 0;
	ClickMode clickMode = EDIT;
	//New item box.
	int UIEboxx = 0;
	int UIEboxy = 0;
	int UIEboxwidth = 0;
	int UIEboxheight = 0;
	bool UIEitembox = false;
	//Moving.
	int UIEtimer = 0;
	//Typing.
	int UIEwritingTimer = 0;
	std::map<UISelectType, bool> selectedTypes;
	//Script Refs
	bool UIErefopen = true;
	glm::vec2 refStartPos;

	std::vector<glm::vec2> UIEpos;



	UILabel debug_msg_box;


	int width = 1920;
	int height = 1080;

	int showInspector = 1; //1 = Objects, 2 = Actors.
	bool showImport = false;
	bool showNewComponent = false;
	unsigned int component_type = 0;
	char component_name[255] = "";
	unsigned int modelID = 0;
	bool showReloadScreen = false;
	bool saveScene = true;

	
	DragPacket dragItem;
	ImVec2 mouseOrigin;
	ImVec2 mouseNow;
};

