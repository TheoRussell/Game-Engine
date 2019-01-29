#include "EngineInterface.h"



EngineInterface::EngineInterface()
{
	selectedTypes.insert(std::pair<UIType, bool>(s_Rectangle, false));
	selectedTypes.insert(std::pair<UIType, bool>(s_Line, false));
	selectedTypes.insert(std::pair<UIType, bool>(s_Label, false));
	selectedTypes.insert(std::pair<UIType, bool>(s_Poly, false));
	selectedTypes.insert(std::pair<UIType, bool>(s_PBar, false));

	debug_msg_box.DebugStyle();




}


EngineInterface::~EngineInterface()
{
}


void EngineInterface::modifyScriptRef(ClientHandler &client, PhysicsEngine &physics, ResourceManager &res, ScriptManager &scripts) {
	ImGui::Begin("Script Refs", &UIErefopen);

	int index = 0;
	for each (ComponentScript * scr in physics.scripts) { //Loop through each script.
		if (ImGui::TreeNode((scr->GetName() + "##scriptTree" + std::to_string(index)).c_str())) { //Display the script name as a new node.


																								  ///Labels.
			for (std::pair<std::string, InterfaceItem*> pointerRef : *scr->GetInterfacePointers()) { //Loops through each reference to a UI element.	
																									 //Checks if the user has selected an item, hovering it over an option and then releasing the button (to pair the reference to the element).
				std::string identifier = pointerRef.first + "##" + std::to_string(index) + scr->GetName();
				ImGui::Selectable(identifier.c_str());
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(0)) {

					glm::vec2 revertDist = refStartPos - glm::vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);

					for (InterfaceItem* item : UIE.Interface_Items) {
						if (item->GetSelected()) {
							pointerRef.second = item;
							scriptInterfacePointers.at(scr).at(pointerRef.first) = item;
							//Moves the element back to origin after pairing is complete.
							item->Move(revertDist, uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);

							OnStart();
							ImGui::TreePop();
							ImGui::End();
							return;
						}
					}

				}
			}

			ImGui::TreePop();
		}

		index++;
	}

	ImGui::End();
}


void EngineInterface::OnLoad(ClientHandler &client, PhysicsEngine &physics) {

	for (ComponentScript * s : physics.scripts) {
		//Runs through each script.
		//For each new interface reference created, generate a map of the reference to the pointer of the element.
		for (std::string refName : *s->getNewInterfaceRefs()) {
			s->GetInterfacePointers()->insert(std::pair<std::string, InterfaceItem*>(refName, nullptr));
		}


		std::map<std::string, InterfaceItem*> maps;
		for each (std::pair<std::string, InterfaceItem*> refNPointer in *s->GetInterfacePointers()) { //Loops through each reference to a UI label.
			maps.insert(refNPointer);
		}
		//Adds any of the UI references from the script into a map that keeps track of all references for all scripts.
		scriptInterfacePointers.insert(std::pair<ComponentScript*, std::map<std::string, InterfaceItem*>>( s,maps));

	}
}


void EngineInterface::OnStart() {
	for (std::pair<ComponentScript*, std::map<std::string, InterfaceItem*>> pair : scriptInterfacePointers) {
		//Updating each script to have the appropriate references added.
		*pair.first->GetInterfacePointers() = pair.second;
	}
}

void EngineInterface::OnUpdate(float deltaTime, ClientHandler &client, PhysicsEngine &physics, ResourceManager &res) {
	//Debug messages.
	debug_msg_box.text = "";
	try {
		for (std::pair<std::string, float> msg : debug_messages) {
			msg.second += deltaTime;
			if (msg.second < 2.0f) {
				debug_messages.at(msg.first) = msg.second;
				debug_msg_box.text.append("\n" + msg.first);
			}
			else {
				debug_messages.erase(msg.first);
			}
		}
	}
	catch (std::exception ex) {
		
	}

	for each (ComponentScript *cs in physics.scripts) {
		for each (std::string s in cs->getDebugText()) {
			debug_messages.insert(std::pair<std::string, float>(s, 0.0f));
		}
	}



	//Updating buttons.
	glm::vec2 mousePosition = { (ImGui::GetMousePos().x - xDisplacement) / uiWidth, (ImGui::GetMousePos().y - yDisplacement) / uiHeight };
	int index = 0;
	for (UIButton button : UIE.buttons) {
		if (ImGui::IsMouseClicked(0)) {
			if (button.InBoundingBox(mousePosition)) {
				button.Clicked = true;
			}
		}
		UIE.buttons[index] = button;
		index++;
	}
}


void EngineInterface::changeUI(ClientHandler &client, PhysicsEngine &physics, std::string name) {
	if (loadInterface(client.WorkingDir + "Interfaces\\" + name + ".gui", client, physics)) {
		client.project.startInterface = name;
	}
}

void EngineInterface::deleteUI(ClientHandler &client, std::string name) {
	std::string loc = client.WorkingDir + "Interfaces\\" + name + ".gui";
	if (std::remove(loc.c_str())) {
		//Deleted file.
	}
}


void EngineInterface::NormalDesign() {
	ImGuiStyle & style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.01f, 0.01f, 0.01f, 1.00f); //Background
	style.Colors[ImGuiCol_Border] = ImVec4(0.01f, 0.01f, 0.01f, 1.00f); //Outline
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f); //Text bg
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.35f, 0.1f, 1.00f); //Text bg
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1f, 0.4f, 0.1f, 1.00f); //Text bg
	style.Colors[ImGuiCol_Button] = ImVec4(0.007, 0.203, 0.572, 1.0f);
}

//Top bar.
void EngineInterface::Menu(ClientHandler &client, PhysicsEngine &physics, ResourceManager &res, ScriptManager &scripts) {
	ImGui::Begin("Menu", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowSize(ImVec2(width, 50));
	ImGui::SetWindowPos(ImVec2(0, 0));

	UIEy += 50;


	ImGuiStyle & style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImVec4(0.5, 0.001, 0.001, 1.0f);
	if (ImGui::Button("X")) {
		client.engineView = Editor; //Goes to editor so that exit may occur.
		client.closeProgram = true;
	};
	ImGui::SameLine();

	if (client.engineView != UIEditor) {
		ImGui::SameLine();
		if (ImGui::Button("UI View")) {
			client.engineView = UIEditor;
			UIEx = UIEx_s;
			UIEy = UIEy_s;
			UIEzoom = UIEzoom_s;
		}
	}
	else {
		ImGui::SameLine();
		if (ImGui::Button("Engine View")) {
			OnStart(); //Required to update pointers.
			client.engineView = Editor;
			client.InEditor = false;
			UIEx_s = UIEx;
			UIEy_s = UIEy;
			UIEzoom_s = UIEzoom;
			UIEx = 0;
			UIEy = 0;
			UIEzoom = 1.0f;
		}

		ImGui::SameLine();
		if (clickMode == EDIT) {
			if (ImGui::Button("UI: Cursor##UIMode")) {
				clickMode = CREATE;
			}
		}
		else if (clickMode == CREATE) {
			if (ImGui::Button("UI: Poly##UIMode")) {
				clickMode = EDIT;
			}
		}

		ImGui::SameLine();

		char *txtChar = new char[255];
		strcpy_s(txtChar, 255, UIE.UIName.c_str());
		ImGui::InputText("UI Name", txtChar, 255);
		std::string txtLength = txtChar;
		if (txtLength.length() > 0) {
			UIE.UIName = txtChar;
		}
		delete[] txtChar;

		ImGui::SameLine();
		if (ImGui::Button("Save##saveUI")) {
			if (!UIEitembox) {
				saveInterface(client);
				loadInterface(client.WorkingDir + "Interfaces\\" + UIE.UIName + ".gui", client, physics);


			}
			else {
				debug_write("Unable to save interface while modifying item!");
			}

		}
	}
	


	NormalDesign();

	ImGui::End();
}

bool EngineInterface::loadInterface(std::string path, ClientHandler & client, PhysicsEngine & physics) {
	try {
		std::ifstream input(path, std::ifstream::in | std::ios::binary);
		if (!UIE.loadUI(input)) {
			input.close();
			return false;
		}

		scriptInterfacePointers.clear();
		OnLoad(client, physics);

		BINARYWrite writeType = BINARY_Script;
		while (input.good() && writeType != BINARY_END) {
			if (writeType == BINARY_Script) {
				int refCount = BinaryFiles::getInt(input);
				if (refCount > 0) {
					for (int iter = 0; iter < refCount; iter++) {
						//Loops through each script paired to interface.
						std::string scriptName = BinaryFiles::getString(input);
						//Fetch script pointer.
						for (ComponentScript* script : physics.scripts) {
							if (script->GetName() == scriptName) {
								//Checks if the correct script is found.
								int pairCount = BinaryFiles::getInt(input);
								if (pairCount > 0) {
									//Binds each of the pointers to specific script/interface reference.
									for (int pairInter = 0; pairInter < pairCount; pairInter++) {
										std::string referenceName = BinaryFiles::getString(input);
										std::string referenceID = BinaryFiles::getString(input);

										//Fetch actual pointer.
										for (InterfaceItem* item : UIE.Interface_Items) {
											if (item->GetID() == referenceID) {
												scriptInterfacePointers.at(script).at(referenceName) = item;
												break;
											}
										}
									}
								}
								break;
							}
						}

						
					}
				}
			}
			writeType = BinaryFiles::getBINARYType(input);
		}

		OnStart();

		input.close();
	}
	catch (std::exception ex) {
		std::cout << "cannot load interface" << std::endl;
		return false;
	}
	return true;

}

void EngineInterface::saveInterface(ClientHandler &client) {
	try {
		std::ofstream output(client.WorkingDir + "Interfaces\\" + UIE.UIName + ".gui", std::ofstream::out | std::ios::binary);
		output.clear();
		UIE.saveAll(output);

		BINARYWrite end = BINARY_END;

		//Saving script references to file.
		//Writes count of all scripts, then name of script & number of references for script, then the references.
		end = BINARY_Script;
		BinaryFiles::writeBINARYType(output, end);
		int refCount = scriptInterfacePointers.size();
		BinaryFiles::writeInt(output, refCount);
		for (std::pair<ComponentScript*, std::map<std::string, InterfaceItem*>> ref : scriptInterfacePointers) {
			BinaryFiles::writeString(output, ref.first->GetName());

			int pairCount = ref.second.size();
			BinaryFiles::writeInt(output, pairCount);
			for (std::pair<std::string, InterfaceItem*> pair : ref.second) {
				BinaryFiles::writeString(output, pair.first);

				//If no reference was added, make it null.
				if (pair.second == nullptr) {
					std::string null = "NULL";
					BinaryFiles::writeString(output, null);
				}
				else {
					BinaryFiles::writeString(output, pair.second->GetID());
				}

			}
		}

		end = BINARY_END;
		BinaryFiles::writeBINARYType(output, end);
		output.close();
	}
	catch (std::exception ex) {
		std::cout << "error saving interface" << std::endl;
	}

}


void EngineInterface::debug_write(std::string msg) {
	debug_messages.insert(std::pair<std::string, float>(msg, 0.0f));
}


void EngineInterface::projects(ClientHandler &client) {
	ImGui_ImplGlfwGL3_NewFrame();

	
	ImGuiStyle & style = ImGui::GetStyle();


	style.Colors[ImGuiCol_Border] = ImVec4(0.1f, 0.1f, 0.1f, 0.00f); //Text bg
	ImGui::Begin("Projects", (bool*)true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(width, height));

	style.Colors[ImGuiCol_Button] = ImVec4(0.5, 0.001, 0.001, 1.0f);
	if (ImGui::Button("X")) {
		client.closeProgram = true;
	};
	NormalDesign();

	ImGui::Text("Welcome to the Game Engine");


	std::string fileName;
	std::vector<std::string> projectNames;
	for (auto & p : std::experimental::filesystem::directory_iterator("src\\Projects\\")) {
		fileName = p.path().string();
		fileName.erase(0, 13);
		projectNames.push_back(fileName);
	}

	if (projectNames.size() != 0) {
		for each (std::string Proj in projectNames) {
			ImGui::NewLine();
			if (ImGui::Selectable(&Proj[0], false, ImGuiSelectableFlags_AllowDoubleClick)) {
				if (ImGui::IsMouseDoubleClicked(0)) {
					client.WorkingDir = "src\\Projects\\" + Proj + "\\";
					if (client.loadProject()) {
						client.engineView = Editor;
					}
					
				}
			}
		}
	}
	else {
		//If there are no projects already ge
		client.project.generate("NewProject");
		client.WorkingDir = "src\\Projects\\NewProject\\";
		client.world = Scene();
		client.world.saveBinary(client.WorkingDir);
		client.engineView = Editor;
	}


	if (ImGui::Button("New Project")) {
		std::string newName = "Project";
		bool newNameGen = false;
		unsigned int index = 0;
		while (!newNameGen) {
			newNameGen = true;
			for each (std::string name in projectNames) {
				if (name == newName) {
					index++;
					newName = "Project" + std::to_string(index);
					newNameGen = false;
				}
			}
		}

		client.project.generate(newName);
		client.WorkingDir = "src\\Projects\\" + newName + "\\";
		client.world = Scene();
		client.world.saveBinary(client.WorkingDir);
		client.engineView = Editor;


	}



	ImGui::End();

	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}


//Interface


void EngineInterface::UIEitem(ClientHandler &client, PhysicsEngine &physics, ResourceManager &res, ScriptManager &scripts) {
	ImGui::SetNextWindowPos({ (float)UIEboxx, (float)UIEboxy });
	ImGui::Begin("New Object##UIEitembox", (bool*)true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
	if (ImGui::Button("Copy")) {
		UIEitembox = !copyUI();
	}
	ImGui::SameLine();
	if (ImGui::Button("Paste")) {
		pasteUI();
		if (UIE.Interface_Clip.size() > 0) {
			//Hide the new item box.
			UIEitembox = false;
		}
	}


	unsigned int index = 0;
	for (InterfaceItem* item : UIE.Interface_Items) {
		if (item->DisplayData()) {

			//Deleting the element from the interface.
			if (item->GetType() == s_Label) {
				for (int i = 0; i < UIE.labels.size(); i++) {
					if (&UIE.labels.at(i) == item) {
						UIE.labels.erase(UIE.labels.begin() + i);
						break;
					}
				}
			}
			if (item->GetType() == s_Button) {
				for (int i = 0; i < UIE.buttons.size(); i++) {
					if (&UIE.buttons.at(i) == item) {
						UIE.buttons.erase(UIE.buttons.begin() + i);
						break;
					}
				}
			}
			if (item->GetType() == s_PBar) {
				for (int i = 0; i < UIE.bars.size(); i++) {
					if (&UIE.bars.at(i) == item) {
						UIE.bars.erase(UIE.bars.begin() + i);
						break;
					}
				}
			}
			if (item->GetType() == s_Rectangle) {
				for (int i = 0; i < UIE.rectangles.size(); i++) {
					if (&UIE.rectangles.at(i) == item) {
						UIE.rectangles.erase(UIE.rectangles.begin() + i);
						break;
					}
				}
			}
			if (item->GetType() == s_Poly) {
				for (int i = 0; i < UIE.polygons.size(); i++) {
					if (&UIE.polygons.at(i) == item) {
						UIE.polygons.erase(UIE.polygons.begin() + i);
						break;
					}
				}
			}
			if (item->GetType() == s_Line) {
				for (int i = 0; i < UIE.lines.size(); i++) {
					if (&UIE.lines.at(i) == item) {
						UIE.lines.erase(UIE.lines.begin() + i);
						break;
					}
				}
			}



			UIE.Interface_Items.erase(UIE.Interface_Items.begin() + index);
			UIE.GeneratePointers();

			break;
		}
		index++;
	}


	ImGui::TextDisabled("NEW");
	if (ImGui::Button("% Bar")) {
		UIE.addPercentBar();
		UIEitembox = false;
	}
	if (ImGui::Button("Rectangle")) {
		UIE.addRect();
		UIEitembox = false;
	}
	if (ImGui::Button("Label")) {
		UIE.addLabel();
		UIEitembox = false;
	}
	if (ImGui::Button("Button")) {
		UIE.addButton();
		UIEitembox = false;
	}
	//Used to prevent clicking generating a new polygon.
	UIEboxwidth = ImGui::GetWindowWidth() + 50; //50 is for the scrollbar
	UIEboxheight = ImGui::GetWindowHeight() + 10;
	ImGui::End();
}

bool EngineInterface::copyUI() {
	bool copied = false;

	for (InterfaceItem* item : UIE.Interface_Items) {
		if (item->GetSelected()) {
			UIE.Interface_Clip.push_back(item);
			copied = true;
		}
	}



	return copied;
}

void EngineInterface::pasteUI() {
	glm::vec2 mousePos(UIEboxx, UIEboxy);
	std::cout << "Needs doing" << std::endl;

	//for each (UIRectangle box in UIE.clipboard.rect) {
	//	pointToScreen(box.topLeft, uiWidth, uiHeight);
	//	int dx = mousePos.x - box.topLeft.x;
	//	int dy = mousePos.y - box.topLeft.y;
	//	pointToScreen(box.lowRight, uiWidth, uiHeight);
	//	box.lowRight.x += dx;
	//	box.lowRight.y += dy;
	//	box.topLeft.x = mousePos.x;
	//	box.topLeft.y = mousePos.y;
	//	screenToPoint(box.topLeft);
	//	screenToPoint(box.lowRight);
	//	UIE.addRect(box);
	//}
	//for each (UILabel lbl in UIE.clipboard.label) {
	//	glm::vec2 tinyPos = mousePos;
	//	screenToPoint(tinyPos);
	//	lbl.point = tinyPos;
	//	UIE.addLabel(lbl);
	//}
}

void EngineInterface::pointToScreen(glm::vec2 &pos, int width, int height) {
	pos.x = pos.x*UIEzoom*width + xDisplacement + UIEx;
	pos.y = pos.y*UIEzoom*height + UIEy;
}

void EngineInterface::screenToPoint(glm::vec2 &pos) {
	pos.x = (pos.x - UIEx - xDisplacement) / (UIEzoom * uiWidth);
	pos.y = (pos.y - UIEy) / (UIEzoom * uiHeight);
}

void EngineInterface::movePoint(glm::vec2 &pos, int width, int height, int xChange, int yChange) {
	pointToScreen(pos, width, height);
	pos.x += xChange;
	pos.y += yChange;
	pos.x = (pos.x - UIEx - xDisplacement) / (UIEzoom * width);
	pos.y = (pos.y - UIEy) / (UIEzoom * height);
}

void EngineInterface::scalePoint(glm::vec2 &pos, int width, int height, float xChange, float yChange) {
	pointToScreen(pos, width, height);
	pos.x += pos.x*xChange;
	pos.y += pos.y*yChange;
	pos.x = (pos.x - UIEx - xDisplacement) / (UIEzoom * width);
	pos.y = (pos.y - UIEy) / (UIEzoom * height);
}

bool EngineInterface::pointInRect(glm::vec2 point, glm::vec2 topLeft, glm::vec2 lowRight, int width, int height) {
	pointToScreen(topLeft, width, height);
	pointToScreen(lowRight, width, height);
	if (point.x >= topLeft.x && point.x <= lowRight.x && point.y >= topLeft.y && point.y <= lowRight.y) {
		return true;
	}
	return false;
}

bool EngineInterface::pointInRect(glm::vec2 point, glm::vec2 topLeft, glm::vec2 lowRight) {
	if (point.x >= topLeft.x && point.x <= lowRight.x && point.y >= topLeft.y && point.y <= lowRight.y) {
		return true;
	}
	return false;
}

void EngineInterface::findSelection() {
	//Checks if the left shift is held. If it isn't, all previously selected options will be deselected.
	ImGuiIO & io = ImGui::GetIO();
	ImVec2 mousePos = ImGui::GetMousePos();
	if (io.KeysDown[GLFW_KEY_LEFT_SHIFT]) {

	}
	else {
		UIE.deselectAll();
		for (std::pair<UIType, bool> p : selectedTypes) {
			selectedTypes.at(p.first) = false;
		}
	}


	//Gets mouse position between 0 and 1.
	glm::vec2 mouseRelative = { mousePos.x, mousePos.y };
	screenToPoint(mouseRelative);

	for (InterfaceItem* item : UIE.Interface_Items) {
		//Checks if mouse intersects the items bounding area.
		try {
			if (item->InBoundingBox(mouseRelative)) {
				item->SetSelected(true);
				selectedTypes[item->GetType()] = true;
			}
		}
		catch (std::exception ex) {
			std::cout << "An error occured in selecting an item. Type ID:" << item->GetType() << std::endl;
		}

	}
}

void EngineInterface::userInterface(ClientHandler &client, PhysicsEngine &physics, ResourceManager &res, ScriptManager &scripts) {

	ImGui::SetNextWindowBgAlpha(0.0f);

	uiWidth = width - xDisplacement;
	uiHeight = (uiWidth / 16.0f)*9.0f;

	ImGui::SetNextWindowPos(ImVec2(xDisplacement, UIEy));
	ImGui::SetNextWindowSize(ImVec2(uiWidth,uiHeight));
	ImGui::Begin("##UIOverlay", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);



	//All objects will have a position of 0-1;
	//Rendering the created objects.

	UIE.uiWidth = uiWidth;
	UIE.uiHeight = uiHeight;
	for (InterfaceItem* item : UIE.Interface_Items) {
		item->Render(uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);
	}

	if (client.engineView == Game) {
		debug_msg_box.Render(uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);
	}


	ImGuiIO & io = ImGui::GetIO();
	ImDrawList * drawList = ImGui::GetOverlayDrawList();

	if (client.engineView == UIEditor) {

		ImVec2 mousePos = ImGui::GetMousePos();

		if ((io.KeysDown[GLFW_KEY_LEFT_CONTROL] | io.KeysDown[GLFW_KEY_RIGHT_CONTROL]) && (io.KeysDown[GLFW_KEY_V])) {
			UIEboxx = mousePos.x;
			UIEboxy = mousePos.y;
			pasteUI();
		}
		if ((io.KeysDown[GLFW_KEY_LEFT_CONTROL] | io.KeysDown[GLFW_KEY_RIGHT_CONTROL]) && (io.KeysDown[GLFW_KEY_C])) {
			UIEboxx = mousePos.x;
			UIEboxy = mousePos.y;
			copyUI();
		}


		if (ImGui::IsMouseClicked(1)) {
			UIEboxx = mousePos.x;
			UIEboxy = mousePos.y;
			UIEitembox = true;
			findSelection();
		}

		//Moving the display.
		if (ImGui::IsMouseDoubleClicked(2) && inScene(ImGui::GetMousePos())) {
			UIEx = 0;
			UIEy = 0;
			UIEzoom = 1;
		}
		
		//Zooming
		if (client.ScrollData != 0.0f) {
			if (!io.KeysDown[GLFW_KEY_LEFT_CONTROL]) {
				UIEzoom += (float)client.ScrollData * 0.1;
				if (UIEzoom <= 0.1f) {
					UIEzoom = 0.1;
				}
			}
			else {
				int index = 0;
				float scale;
				if (io.KeysDown[GLFW_KEY_LEFT_ALT]) {
					scale = (float)client.ScrollData * 0.01;
				}
				else {
					scale = (float)client.ScrollData * 0.1;
				}

				for (InterfaceItem* item : UIE.Interface_Items) {
					item->Scale({ scale, scale }, uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);
				}
			}
		}
		
		//Dragging.
		if (ImGui::IsMouseDown(2)) {
			UIEdown++;
			if (UIEdown < 2) {
				UIExo = mousePos.x;
				UIEyo = mousePos.y;
			}
			else {
				if (inScene(mousePos)) {
					UIEx += mousePos.x - UIExo;
					UIEy += mousePos.y - UIEyo;
					UIExo = mousePos.x;
					UIEyo = mousePos.y;
				}
			}
		}
		if (ImGui::IsMouseReleased(2)) {
			UIEdown = 0;
		}

		if (clickMode == EDIT) {
			if (ImGui::IsMouseReleased(0)) {
				UIEdown = 0;
				UIEtimer = 0;
			}
			if (ImGui::IsMouseDown(0) && !(UIEitembox && mousePos.x >= UIEboxx && mousePos.x <= UIEboxx + UIEboxwidth && mousePos.y >= UIEboxy && mousePos.y <= UIEboxy + UIEboxheight)) {

				UIEtimer++;
				if (UIEtimer == 1) {
					//Clear all selections. Find the selected object.
					findSelection();
					refStartPos.x = ImGui::GetMousePos().x;
					refStartPos.y = ImGui::GetMousePos().y;
				}

				//Moving objects.
				UIEdown++;
				if (UIEdown < 2) {
					UIExo = mousePos.x;
					UIEyo = mousePos.y;
				}
				else {
					int xChange = mousePos.x - UIExo;
					int yChange = mousePos.y - UIEyo;

					bool anyItemSelected = false;

					for (InterfaceItem* item : UIE.Interface_Items) {
						if (item->GetSelected()) {
							anyItemSelected = true;
							item->Move({ xChange, yChange }, uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);
						}
					}
					UIExo = mousePos.x;
					UIEyo = mousePos.y;



					if (!anyItemSelected) {
						UIEitembox = false;
					}
				}

			}
		}

		//Adding designs.
		if (clickMode == CREATE) {
			//When the user double clicks or presses enter, the new polygon will be completed.
			if (ImGui::IsMouseDoubleClicked(0) | io.KeysDown[GLFW_KEY_ENTER]) {
				if (UIEpos.size() == 2) {
					UIE.addLine(UIEpos[0], UIEpos[1], glm::vec4(255, 0, 0, 255));
				}
				else {
					UIE.addPolygon(UIEpos, glm::vec4(255, 0, 0, 255), glm::vec4(255, 0, 0, 255));
				}
				UIEpos.clear();
			}
			//If the user continues to click whilst still generating the polygon, the new point will be added to a list.
			else if (ImGui::IsMouseClicked(0)) {
				if (mousePos.x > xDisplacement && mousePos.y > 50) {
					if (UIEitembox && mousePos.x >= UIEboxx && mousePos.x <= UIEboxx + UIEboxwidth && mousePos.y >= UIEboxy && mousePos.y <= UIEboxy + UIEboxheight) {
						//Prevents generating of new polygons when using the menu of items.
					}
					else {
						glm::vec2 pos;
						pos.x = (mousePos.x - UIEx - xDisplacement) / (uiWidth*UIEzoom);
						pos.y = (mousePos.y - UIEy - 50) / (uiHeight*UIEzoom);
						if (UIEpos.size() > 0) {
							if (io.KeysDown[GLFW_KEY_LEFT_SHIFT]) {
								glm::vec2 lastPos = UIEpos[UIEpos.size()];
								float xChange = abs(pos.x - lastPos.x);
								float yChange = abs(pos.y - lastPos.y);
								if (xChange > yChange) { pos.y = lastPos.y; }
								if (xChange < yChange) { pos.x = lastPos.x; }
							}
						}

						UIEpolyX = pos.x;
						UIEpolyY = pos.y;
						UIEpos.push_back(pos);
					}

				}
			}

			if (UIEpos.size() > 0) {
				int ind = 0;
				for each (glm::vec2 vertex in UIEpos) {
					if (ind == UIEpos.size() - 1) {
						if (io.KeysDown[GLFW_KEY_LEFT_SHIFT]) {
							glm::vec2 lastPos = UIEpos[UIEpos.size() - 1];
							lastPos.y = (lastPos.y * uiHeight * UIEzoom) + 50 + UIEy;
							lastPos.x = (lastPos.x * uiWidth * UIEzoom) + xDisplacement + UIEx;
							float xChange = abs(mousePos.x - lastPos.x);
							float yChange = abs(mousePos.y - lastPos.y);
							if (xChange > yChange) { mousePos.y = lastPos.y; }
							if (xChange < yChange) { mousePos.x = lastPos.x; }
						}
						drawList->AddLine({ UIEpolyX*uiWidth*UIEzoom + xDisplacement + UIEx, UIEpolyY*uiHeight*UIEzoom + 50 + UIEy }, { mousePos.x, mousePos.y }, IM_COL32(255, 0, 0, 255), 5.0f);
					}
					else {
						drawList->AddLine({ UIEpos[ind].x*uiWidth*UIEzoom + xDisplacement + UIEx, UIEpos[ind].y*uiHeight*UIEzoom + 50 + UIEy }, { UIEpos[ind + 1].x*uiWidth*UIEzoom + xDisplacement + UIEx, UIEpos[ind + 1].y*uiHeight*UIEzoom + 50 + UIEy }, IM_COL32(255, 0, 0, 255), 5.0f);
					}
					ind++;
				}

			}
		}
	}


	ImGui::End();


	

	if (UIEitembox && client.engineView == UIEditor) {
		UIEitem(client, physics, res, scripts);
	}
}


//Engine

//A menu for exiting the game.
void EngineInterface::playStopGame(ClientHandler &client, PhysicsEngine & physics, ResourceManager & res, ScriptManager & scripts) {
	UIEy += 50;


	ImGui::Begin("Game", (bool*)true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowSize(ImVec2(width - xDisplacement, 50));
	if (client.engineView == Editor) {
		ImGui::SetWindowPos(ImVec2(xDisplacement, 50));
	}
	else {
		ImGui::SetWindowPos(ImVec2(xDisplacement, 0));
	}
	

	ImGui::SameLine();
	//Entering the game from the editor.
	if (ImGui::Button("PLAY/STOP"))  {

		if (client.engineView == Game) {
			client.engineView = Editor;
			client.InGame = false;
		}
		else if (client.engineView == Editor) {
			xDisplacement = 0;
			yDisplacement = 50;

			client.engineView = Game;
			client.InGame = true;

			OnStart();

		}
	}


	ImGui::SameLine();
	ImGui::Checkbox("Show UI##showUIRB", &client.showUI);

	ImGui::End();
}



void EngineInterface::update(bool &inGame, ClientHandler &client, PhysicsEngine &physics, ResourceManager &res, ScriptManager &script) {
	ImGui_ImplGlfwGL3_NewFrame();
	//Colour of the interface.
	NormalDesign();


	UIEy = 0;


	//Entering the engine game.
	if (ImGui::IsMouseClicked(0)) {
		int mouse_x = ImGui::GetMousePos().x;
		int mouse_y = ImGui::GetMousePos().y;
		if (mouse_x > xDisplacement && mouse_x < width && mouse_y > 100 && mouse_y < height - yDisplacement) {
			if (!client.InEditor) {
				client.InEditor = true;
			}
		}
	}


	if (client.engineView != PRODUCT && client.engineView != Game) {
		Menu(client, physics, res, script);
	}
	else {
		UIEy = (height - uiHeight) / 2;
		if (UIEy < 50) {
			UIEy = 50;
		}
	}


	if (client.engineView == UIEditor) {
		xDisplacement = 0;
		yDisplacement = 0;

		modifyScriptRef(client, physics, res, script);
	} else if (client.engineView == PRODUCT) {
		
		userInterface(client, physics, res, script);
	} else if (client.engineView == Game | client.engineView == Editor) { //inGame
		playStopGame(client, physics, res, script);
	}

	if (client.showUI && (client.engineView == Game | client.engineView == UIEditor | client.engineView == Editor)) {
		userInterface(client, physics, res, script);
	}

	if (!client.InGame && !client.InEditor && showImport) {
		importItem(client, physics, res, script);
	}

	if (client.viewingCursor) {


		
		if (client.engineView != UIEditor && client.engineView != PRODUCT && client.engineView != Game) {

				if (!showReloadScreen) {

					ImGui::Begin("Project", (bool*)true, ImGuiWindowFlags_NoMove);
					ImGui::SetWindowPos(ImVec2(0, 50));

					displayScenes(client, physics);
					displayActors(res);
					displayWorld(client, physics);
					displayUserInterfaces(client, physics);


					xDisplacement = ImGui::GetWindowWidth();
					yDisplacement = ImGui::GetWindowHeight();
					if (ImGui::GetWindowWidth() > width * 2 / 3) {
						xDisplacement = 2 * width / 3;
						ImGui::SetWindowSize(ImVec2(xDisplacement, yDisplacement));
					}

					ImGui::End();



					ImGui::Begin("Inspector", (bool*)true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
					ImGui::SetWindowSize(ImVec2(xDisplacement, 1080 - yDisplacement));
					ImGui::SetWindowPos(ImVec2(0, yDisplacement + 50));

					displaySelectedObj(client, physics, res);

					ImGui::End();

					DragNDrop(client, physics, res, script);

				}

				if (!showReloadScreen && showNewComponent && client.world.getSelected() > -1) {
					displayNewComponent(client, physics, res, script);
				}
				if (showReloadScreen) {
					showReload(client);
				}

		}
		else if (client.engineView == UIEditor) {
			ImGui::Begin("Interface", (bool*)true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
			ImGui::SetWindowSize(ImVec2(width, 0));
			ImGui::SetWindowPos(ImVec2(0, 50));

			ImGui::End();
		}


	}

	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


}


bool EngineInterface::inScene(ImVec2 mousePos) {
	if (mousePos.x > xDisplacement && mousePos.x < width && mousePos.y > 50 && mousePos.y < height - 50 - yDisplacement) {
		return true;
	}
	return false;
}

void EngineInterface::DragNDrop(ClientHandler &client, PhysicsEngine &physics, ResourceManager &res, ScriptManager &scripts) {
	if (ImGui::IsMouseDragging(0)) {
		mouseNow = ImGui::GetMousePos();
	}
	else {
		if (ImGui::IsMouseReleased(0)) {
			if (inScene(mouseNow)) {
				if (dragItem.itemType == 1) {
					client.world.addObject(dragItem.obj, true);
				}
				mouseNow = ImVec2(-1, -1);
				dragItem.itemType = 0;
			}
		}
		mouseOrigin = ImGui::GetMousePos();
	}
}



void EngineInterface::showReload(ClientHandler &client) {
	ImGui::Begin("IMPORTANT");
	ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), "It is essential to reload the program to load new assets.");
	ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "The current scene will be saved.");
	if (ImGui::Button("EXIT")) {
		if (saveScene) {
			client.world.saveBinary(client.WorkingDir);
		}
		client.closeProgram = true;
	}
	ImGui::End();
}

void EngineInterface::importItem(ClientHandler &client, PhysicsEngine &physics, ResourceManager &res, ScriptManager &scripts) {
	ImGui::Begin("IMPORT##ImportWindow");

	std::string fileName;
	std::vector<std::string> sceneNames;
	for (auto & p : std::experimental::filesystem::directory_iterator("src\\models\\")) {
		fileName = p.path().string();
		fileName.erase(0, 11);

		if (fileName.find(".obj") != std::string::npos) {
			fileName.erase(fileName.length() - 4, 4);
			sceneNames.push_back(fileName);
		}
	}
	for each (Model model in res.getModels()) {
		int index = 0;
		for each (std::string name in sceneNames) {
			if (name == model.getPathName()) {
				sceneNames.erase(sceneNames.begin() + index);
				break;
			}
			index++;
		}
	}
	for each (std::string name in sceneNames) {
		if (ImGui::Button(&name[0])) {
			res.newModel(name, "src\\models\\" + name + ".obj");
			try {
				std::ofstream file("src\\scenes\\proj1.proj", std::ios_base::app);
				
				if (file.good()) {
					file << name << "\n";
				}
				file.close();
			}
			catch (std::exception ex) {

			}
		}
	}

	ImGui::End();
}


void EngineInterface::displayUserInterfaces(ClientHandler &client, PhysicsEngine& physics) {
	if (ImGui::TreeNode("Interfaces")) {

		std::vector<std::string> uiNames;
		std::string fileName;
		for (auto & p : std::experimental::filesystem::directory_iterator(client.WorkingDir + "Interfaces\\")) {
			fileName = p.path().string();
			fileName.erase(0, client.WorkingDir.length() + 11);

			if (fileName.find(".gui") != std::string::npos) {
				fileName.erase(fileName.length() - 4, 4);
				uiNames.push_back(fileName);
			}
		}

		int uiIndex = 0;
		for each (std::string s in uiNames) {
			bool opened = (s == UIE.UIName);
			if (ImGui::Selectable(s.c_str(), s == UIE.UIName, ImGuiSelectableFlags_AllowDoubleClick)) {
				if (ImGui::IsMouseDoubleClicked(0)) {
					if (!opened) {
						changeUI(client, physics, s);
					}
				}

			}
			if (opened) {
				if (ImGui::Button("Delete##delUI")) {
					if (ImGui::IsMouseDoubleClicked(0)) {
						deleteUI(client, s);
						uiNames.erase(uiNames.begin(), uiNames.begin() + uiIndex);
						break;
					}
				}
			}
			uiIndex++;
		}

		ImGui::TreePop();
	}
}


void EngineInterface::displayNewComponent(ClientHandler &client, PhysicsEngine &physics, ResourceManager &res, ScriptManager &scripts) {
	int selected = client.world.getSelected();

	ImGui::Begin("New component");
	if (ImGui::Button("Close")) {
		showNewComponent = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Change type")) {
		component_type++;
		if (component_type == 4) {
			component_type = 0;
		}
	}
	if (component_type == 0) {
		ImGui::Text("Model");
		ImGui::InputText("Name", component_name, 255);
		if (component_name == "") {
			strcpy_s(component_name, "<Empty>");
		}
		if (ImGui::Button("Add")) {
			ModelComponent model = ModelComponent();
			model.modelData = Model();
			unsigned int path = client.world.objects[selected].components.addModel(model);
			client.world.objects[selected].components.models[path].componentTransform.name = component_name;
		}
	}
	else if (component_type == 1) {
		ImGui::Text("Point Light");
		ImGui::InputText("Name", component_name, 255);
		if (component_name == "") {
			strcpy_s(component_name, "<Empty>");
		}
		if (ImGui::Button("Add")) {
			unsigned int path = client.world.objects[selected].components.addPointLight(PointLight());
			client.world.objects[selected].components.pointLights[path].componentTransform.name = component_name;
		}
	}
	else if (component_type == 2) {
		ImGui::Text("Camera");
		ImGui::InputText("Name", component_name, 255);
		if (component_name == "") {
			strcpy_s(component_name, "<Empty>");
		}
		if (ImGui::Button("Add")) {
			client.world.disableCameras();
			unsigned int path = client.world.objects[selected].components.addCamera(Camera());
			client.world.objects[selected].components.cameras[path].componentTransform.name = component_name;
		}
	}
	else if (component_type == 3) {
		ImGui::TextDisabled("Script");
		if (component_name == "") {
			strcpy_s(component_name, "NewScript");
		}
		ImGui::InputText("Name", component_name, 255);
		if (ImGui::Button("New")) {
			for each (ComponentScript* script in physics.scripts) {
				if (script->GetName() == component_name) {
					std::string name = component_name;
					name.append("(1)");
					strcpy_s(component_name, name.c_str());
				}
			}
			scripts.NewScript(component_name);
			client.world.objects[selected].addScript(physics.scripts.size());
			showReloadScreen = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete")) {
			unsigned int id = 0;
			for each (ComponentScript* script in physics.scripts) {
				if (script->GetName() == component_name) {
					unsigned int index = 0;
					for each (Object obj in client.world.objects) {
						obj.deleteScript(id);
						client.world.setObject(index, obj);
						index++;
					}
					scripts.DeleteScript(component_name);
					showReloadScreen = true;
				}
				id++;
			}
		}
		unsigned int id = 0;
		for each (ComponentScript* script in physics.scripts) {
			bool has = false;
			for each (unsigned int x in client.world.objects[selected].componentScriptIDs) {
				if (x == id) {
					has = true;
				}
			}
			if (ImGui::Selectable(script->GetName().c_str(), has, ImGuiSelectableFlags_AllowDoubleClick)) {
				if (ImGui::IsMouseDoubleClicked(0)) {
					if (has == false) {
						client.world.objects[selected].addScript(id);
					}
				}
			}
			id++;
		}

	}
	ImGui::End();
}

void EngineInterface::displayActors(ResourceManager &res) {
	if (ImGui::TreeNode("Actors")) {
		unsigned int index = 0;
		for each (Object act in res.getActors()) {

			if (ImGui::Selectable(act.name.c_str(), act.isSelected(), ImGuiSelectableFlags_AllowDoubleClick)) {
				if (ImGui::IsMouseDoubleClicked(0)) {
					unsigned int otherIndex = 0;
					for each (Object actor in res.getActors()) {
						actor.deselect();
						res.setActor(otherIndex, actor);
						otherIndex++;
						showInspector = 2;
					}
					act.select();
					res.setActor(index, act); //update resources.
				}

				if (ImGui::IsMouseDown(0)) {
					dragItem.obj = act;
					dragItem.itemType = 1;
				}

			}




			index++;
		}
		ImGui::TreePop();
	}
}

void EngineInterface::displayScenes(ClientHandler &client, PhysicsEngine &physics) {
	std::vector<std::string> sceneNames;
	if (ImGui::TreeNode("Scenes")) {

		if (ImGui::Button("New")) {
			client.world.saveBinary(client.WorkingDir);
			client.world = Scene();
		}

		std::string fileName;
		for (auto & p : std::experimental::filesystem::directory_iterator(client.WorkingDir + "Scenes\\")) {
			fileName = p.path().string();
			fileName.erase(0, client.WorkingDir.length() + 7);

			if (fileName.find(".SCENE") != std::string::npos) {
				fileName.erase(fileName.length() - 6, 6);
				sceneNames.push_back(fileName);
			}
		}

		if (sceneNames.size() == 0) {
			ImGui::TextColored(ImVec4(0.7f, 0.0f, 0.0f, 1.0f), "No scenes were located!");
		}
		else {
			for each (std::string sceneName in sceneNames) {
				bool selectedScene = false;
				if (sceneName == client.world.name) {
					selectedScene = true;
				}
				if (ImGui::Selectable(sceneName.c_str(), selectedScene, ImGuiSelectableFlags_AllowDoubleClick)) {
					if (ImGui::IsMouseDoubleClicked(0)) {
						if (!selectedScene) {
							client.world.saveBinary(client.WorkingDir);
							client.world.loadBinary(client.WorkingDir + "Scenes\\" + sceneName);
						}
					}
				}
			}
		}



		ImGui::TreePop();
	}
}
void EngineInterface::displayWorld(ClientHandler &client, PhysicsEngine &physics) {
	if (ImGui::TreeNode("World")) {
		if (ImGui::Button("Save")) {
			if (client.world.saveBinary(client.WorkingDir)) {
				std::cout << "saved scene" << std::endl;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete")) {
			client.world.deleteScene();
		}
		if (ImGui::Button("Set as start level")) {
			client.project.startScene = client.world.name;
			client.project.save();
		}


		client.world.voidCol = inputColour("VOID colour", glm::vec4({ client.world.voidCol , 0.0f }), true);
		client.world.d_ambient = inputVec3("Ambient intensity", glm::vec4({ client.world.d_ambient , 0.0f }));
		client.world.d_diffuse = inputVec3("Diffuse intensity", glm::vec4({ client.world.d_diffuse , 0.0f }));
		client.world.d_specular = inputVec3("Specular intensity", glm::vec4({ client.world.d_specular , 0.0f }));

		//float voidCol[3];
		//voidCol[0] = client.world.voidCol.x;
		//voidCol[1] = client.world.voidCol.y;
		//voidCol[2] = client.world.voidCol.z;
		//ImGui::ColorEdit3("VOID colour", voidCol);
		//client.world.voidCol.x = voidCol[0];
		//client.world.voidCol.y = voidCol[1];
		//client.world.voidCol.z = voidCol[2];

		char name[1024];
		strcpy_s(name, client.world.name.c_str());
		if (ImGui::InputText("Name", name, 1024)) {
			if (name != client.world.name) {
				std::string newName = name;
				if (newName != "") {
					client.world.name = name;
				}
			}
		}

		if (ImGui::TreeNode("Object List")) {
			if (ImGui::Button("New object")) {
				unsigned int obj = client.world.addObject(Object());
				unsigned int count = 0;
				for each (Object temp in client.world.objects) {
					if (temp.name.find("NewObj") != std::string::npos) {
						count++;
					}
				}
				if (count == 0) {
					client.world.objects[obj].name = "NewObj";
				}
				else {
					client.world.objects[obj].name = ("NewObj(" + std::to_string(count) + ")");
				}

				client.world.select(obj);
			}

			unsigned int index = 0;
			for each (Object obj in client.world.objects) {
				//if (obj.VisibleToUser) { //Hide objects from user.
					if (ImGui::Selectable(obj.name.c_str(), obj.isSelected(), ImGuiSelectableFlags_AllowDoubleClick)) {
						if (ImGui::IsMouseDoubleClicked(0)) {
							client.world.deselectAll();
							client.world.select(index);
							showInspector = 1;
						}
					}
				//}
				index++;
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void EngineInterface::displaySelectedObj(ClientHandler &client, PhysicsEngine &physics, ResourceManager &res) {
	ImGuiStyle & style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImVec4(0.007, 0.203, 0.572, 1.0f);
	style.ItemSpacing.x = 0;
	if (showInspector == 1) {
		style.Colors[ImGuiCol_Button] = ImVec4(0.031, 0.305, 0.831, 1.0f);
	}
	if (ImGui::Button("Object")) {
		showInspector = 1;
	}
	style.Colors[ImGuiCol_Button] = ImVec4(0.007, 0.203, 0.572, 1.0f);
	ImGui::SameLine();
	if (showInspector == 2) {
		style.Colors[ImGuiCol_Button] = ImVec4(0.031, 0.305, 0.831, 1.0f);
	}
	if (ImGui::Button("Actor")) {
		showInspector = 2;
	}
	//Reset styles to defaults.
	style.Colors[ImGuiCol_Button] = ImVec4(0.007, 0.203, 0.572, 1.0f);
	style.ItemSpacing.x = 10;
	NormalDesign();

	int actorID = res.selectedActor();
	if (showInspector == 2 && actorID > -1) {
		Object actor = res.getActors()[actorID];
		if (ImGui::Button("Delete")) {
			res.destroyActor(actor.name);
			return;
		}
		if (ImGui::TreeNode("Actor Details")) {

			//Changing the name;
			char name[1024];
			strcpy_s(name, actor.name.c_str());
			if (ImGui::InputText("Name", name, 1024)) {
				if (name != actor.name) {
					std::string newName = name;
					if (newName != "") {
						bool sameName = false;
						for each (Object act in res.getActors()) {
							if (act.name == newName) { //Can't allow duplicates.
								sameName = true;
								break;
							}
						}
						if (!sameName) {
							actor.name = newName;
						}
					}
				}
			}
			


			ImGui::TreePop();

			res.setActor(actorID, actor);
		}
	}
	else if (showInspector == 2 && actorID == -1) {
		ImGui::Text("Nothing to see here..");
	}

	if (client.world.getSelected() > -1 && showInspector == 1) {
		Object obj = client.world.getObject(client.world.getSelected());
		if (ImGui::TreeNode("Object Details"))
		{
			char name[1024];
			strcpy_s(name, obj.name.c_str());
			if (ImGui::InputText("Name", name, 1024)) {
				if (name != obj.name) {
					std::string newName = name;
					if (newName != "") {
						bool newNameUnique = true;
						for each (Object o in client.world.objects) {
							if (o.name == newName) {
								newNameUnique = false;
							}
						}
						if (newNameUnique) {
							obj.name = name;
						}
					}
				}
			}

			if (ImGui::Button("Delete object")) {
				unsigned int selected = client.world.getSelected();
				client.world.deleteObject(selected);
				client.world.select(selected - 1);
				if (selected == 0) {
					client.world.select(selected);
				}

				ImGui::TreePop();
				return;

			} //CRASHED IF SPAMMED.
			ImGui::SameLine();
			if (ImGui::Button("Duplicate object")) {
				unsigned int selected = client.world.getSelected();
				Object obj = client.world.getObject(selected);
				client.world.addObject(obj);
				client.world.select(client.world.objects.size() - 1);
			}
			ImGui::SameLine();
			if (ImGui::Button("Make actor")) {
				res.newActor(obj);
			}


			if (ImGui::TreeNode("Transform"))
			{
				//XYZ coords
				ImGui::Text("Location>");
				ImGui::SameLine();
				ImGui::DragFloat(":x", &obj.pos.x, 0.1f);
				ImGui::Text("	     ");
				ImGui::SameLine();
				ImGui::DragFloat(":y", &obj.pos.y, 0.1f);
				ImGui::Text("	     ");
				ImGui::SameLine();
				ImGui::DragFloat(":z", &obj.pos.z, 0.1f);

				ImGui::Text("Rotation>");
				ImGui::SameLine();
				ImGui::DragFloat(":roll", &obj.roll, 0.1f);
				ImGui::Text("	     ");
				ImGui::SameLine();
				ImGui::DragFloat(":pitch", &obj.pitch, 0.1f);
				ImGui::Text("	     ");
				ImGui::SameLine();
				ImGui::DragFloat(":yaw", &obj.yaw, 0.1f);

				ImGui::Text("Scale>   ");
				ImGui::SameLine();
				ImGui::DragFloat(":x ", &obj.sca.x, 0.1f);
				ImGui::Text("	     ");
				ImGui::SameLine();
				ImGui::DragFloat(":y ", &obj.sca.y, 0.1f);
				ImGui::Text("	     ");
				ImGui::SameLine();
				ImGui::DragFloat(":z ", &obj.sca.z, 0.1f);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Physics"))
			{
				//XYZ coords
				ImGui::Checkbox("Does Gravity", &obj.physicsBody.doesGravity);
				ImGui::DragFloat("Mass", &obj.physicsBody.mass);

				if (ImGui::TreeNode("Collision")) {
					ImGui::Checkbox("Enabled", &obj.physicsBody.collides);
					if (obj.physicsBody.collides) {
						if (ImGui::Button("+")) {
							obj.physicsBody.coll_Box.push_back(BoxCollider());
						}

						unsigned int bc_index = 0;
						for each (BoxCollider bc in obj.physicsBody.coll_Box) {
							ImGui::Text(std::to_string(bc_index).c_str());

							std::string identifier = "Remove##delCollBox" + std::to_string(bc_index);
							ImGui::SameLine();
							if (ImGui::Button(identifier.c_str())) {
								obj.physicsBody.coll_Box.erase(obj.physicsBody.coll_Box.begin() + bc_index);
								std::cout << "break" << std::endl;
								break;
							}
							ImGui::Text("Position:");

							identifier = "##posx" + std::to_string(bc_index);
							ImGui::DragFloat(identifier.c_str(), &bc.position.x);
							identifier = "##posy" + std::to_string(bc_index);
							ImGui::DragFloat(identifier.c_str(), &bc.position.y);
							identifier = "##posz" + std::to_string(bc_index);
							ImGui::DragFloat(identifier.c_str(), &bc.position.z);
							ImGui::Text("Scale:");
							identifier = "##scalex" + std::to_string(bc_index);
							ImGui::DragFloat(identifier.c_str(), &bc.scale.x);
							identifier = "##scaley" + std::to_string(bc_index);
							ImGui::DragFloat(identifier.c_str(), &bc.scale.y);
							identifier = "##scalez" + std::to_string(bc_index);
							ImGui::DragFloat(identifier.c_str(), &bc.scale.z);
							obj.physicsBody.coll_Box[bc_index] = bc;
							bc_index++;
						}

					}

					ImGui::TreePop();
				}

				
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Components")) {
				if (ImGui::TreeNode("List")) {
					if (ImGui::Button("Add component")) {
						showNewComponent = true;
					}
					unsigned int index = 0;
					for each (CamComponent cam in obj.components.cameras) {
						if (ImGui::Selectable(cam.componentTransform.name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (ImGui::IsMouseDoubleClicked(0)) {
								deselectAllComponents(obj);
								cam.componentTransform.selected = true;
								obj.components.cameras[index] = cam;
							}
						}
						index++;
					}
					index = 0;
					for each (ModelComponent model in obj.components.models) {
						if (ImGui::Selectable(model.componentTransform.name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (ImGui::IsMouseDoubleClicked(0)) {
								deselectAllComponents(obj);
								model.componentTransform.selected = true;
								obj.components.models[index] = model;
							}
						}
						index++;
					}
					index = 0;
					for each (PLightComponent pl in obj.components.pointLights) {
						if (ImGui::Selectable(pl.componentTransform.name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (ImGui::IsMouseDoubleClicked(0)) {
								deselectAllComponents(obj);
								pl.componentTransform.selected = true;
								obj.components.pointLights[index] = pl;
							}
						}
						index++;
					}
					index = 0;
					ImGui::TreePop();
				}

				glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 sca = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);
				unsigned int compType = 0;
				unsigned int compID[3] = { 0,0,0 };
				bool located = false;
				for each (CamComponent cam in obj.components.cameras) {
					if (cam.componentTransform.selected) {
						pos = cam.componentTransform.position;
						sca = cam.componentTransform.scale;
						rot.x = cam.componentTransform.roll;
						rot.y = cam.componentTransform.pitch;
						rot.z = cam.componentTransform.yaw;
						located = true;
						compType = 1;
						break;
					}
					compID[0]++;
				}
				for each (ModelComponent model in obj.components.models) {
					if (model.componentTransform.selected) {
						pos = model.componentTransform.position;
						sca = model.componentTransform.scale;
						rot.x = model.componentTransform.roll;
						rot.y = model.componentTransform.pitch;
						rot.z = model.componentTransform.yaw;
						located = true;
						compType = 2;
						break;
					}
					compID[1]++;
				}
				for each (PLightComponent pl in obj.components.pointLights) {
					if (pl.componentTransform.selected) {
						pos = pl.componentTransform.position;
						sca = pl.componentTransform.scale;
						rot.x = pl.componentTransform.roll;
						rot.y = pl.componentTransform.pitch;
						rot.z = pl.componentTransform.yaw;
						located = true;
						compType = 3;
						break;
					}
					compID[2]++;
				}
				if (located) {
					if (ImGui::TreeNode("Component Data")) {

						if (ImGui::Button("Delete")) {
							if (compType == 1) {
								obj.components.deleteCamera(compID[0]);
							} else if (compType == 2) {
								obj.components.deleteModel(compID[1]);
							} else if (compType == 3) {
								obj.components.deletePointLight(compID[2]);
							}
						}

						if (ImGui::TreeNode("Transform")) {
							ImGui::Text("Location>");
							ImGui::SameLine();
							ImGui::DragFloat(":x", &pos.x, 0.1f);
							ImGui::Text("	     ");
							ImGui::SameLine();
							ImGui::DragFloat(":y", &pos.y, 0.1f);
							ImGui::Text("	     ");
							ImGui::SameLine();
							ImGui::DragFloat(":z", &pos.z, 0.1f);

							ImGui::Text("Rotation>");
							ImGui::SameLine();
							ImGui::DragFloat(":roll", &rot.x, 0.1f);
							ImGui::Text("	     ");
							ImGui::SameLine();
							ImGui::DragFloat(":pitch", &rot.y, 0.1f);
							ImGui::Text("	     ");
							ImGui::SameLine();
							ImGui::DragFloat(":yaw", &rot.z, 0.1f);

							ImGui::Text("Scale>   ");
							ImGui::SameLine();
							ImGui::DragFloat(":x ", &sca.x, 0.1f);
							ImGui::Text("	     ");
							ImGui::SameLine();
							ImGui::DragFloat(":y ", &sca.y, 0.1f);
							ImGui::Text("	     ");
							ImGui::SameLine();
							ImGui::DragFloat(":z ", &sca.z, 0.1f);
							ImGui::TreePop();
						}
						unsigned int index = 0;
						for each (CamComponent cam in obj.components.cameras) {
							if (cam.componentTransform.selected) {
								if (ImGui::Button("Set as active camera")) {
									client.world.disableCameras();
									cam.camera.enabled = true;
								}
								ImGui::Checkbox("Locked", &cam.camera.locked);

								//if (ImGui::Checkbox("Active", &cam.camera.enabled)) {
								//	if (ImGui::IsMouseClicked(0)) {
								//		if (cam.camera.enabled) {
								//			client.world.disableCameras();
								//		}
								//	}
								//}
								cam.componentTransform.position = pos;
								cam.componentTransform.scale = sca;
								cam.componentTransform.roll = rot.x;
								cam.componentTransform.pitch = rot.y;
								cam.componentTransform.yaw = rot.z;
								obj.components.cameras[index] = cam;
								break;
							}
							index++;
						}
						index = 0;

						for each (ModelComponent model in obj.components.models) {
							if (model.componentTransform.selected) {


								if (ImGui::Button("Update Model")) {
									std::vector<Material> tempMatStore = model.modelData.materials;
									model.modelData.regenerateMeshes();
									model.modelData.regenerateMats(tempMatStore);
								}
								if (ImGui::TreeNode("Mesh")) {

									unsigned int index = 0;
									for each (Mesh mesh in model.modelData.meshes) {

										//Modifying mesh path.
										std::string disTxt = "#" + std::to_string(index) + ":";
										ImGui::TextDisabled(disTxt.c_str());
										ImGui::SameLine();

										disTxt = "##meshName" + std::to_string(index);
										char meshName[255];
										strcpy_s(meshName, sizeof(meshName), mesh.name.c_str());
										ImGui::InputText(disTxt.c_str(),meshName, sizeof(meshName));
										std::string result = meshName;
										if (result.length() > 0) {
											mesh.name = result;
										}

										//Deleting mesh.
										disTxt = "Delete##delMesh" + std::to_string(index);
										if (ImGui::Button(disTxt.c_str())) {
											model.modelData.meshes.erase(model.modelData.meshes.begin() + index);
											break;
										}

										model.modelData.meshes[index] = mesh;
										index++;
									}

									ImGui::TreePop();
								}

								if (ImGui::TreeNode("Material")) {
									if (ImGui::Button("New##newMat")) {
										model.modelData.materials.push_back(Material());
									}

									unsigned int index = 1;
									for each (Material m in model.modelData.materials) {
										//Each material of .mtl may be changed.

										std::string lbl = "#" + std::to_string(index) + ":";
										ImGui::TextDisabled(lbl.c_str());

										ImGui::SameLine();
										char matName[255];
										strcpy_s(matName, 255, m.name.c_str());
										std::string itID = "##matName" + std::to_string(index);
										ImGui::InputText(itID.c_str(), matName, sizeof(matName));
										std::string matName_s = matName;
										if (matName_s.length() > 0) {
											m.name = matName_s;
										}

										unsigned int tPIndex = 0;
										for each (std::string dir in m.texture_paths) {
											m.texture_paths[tPIndex] = inputText(dir, "##iTValTP" + std::to_string(index) + std::to_string(tPIndex));
											tPIndex++;
										}


										std::string delMat = "Delete##delMaterial" + std::to_string(index);
										if (ImGui::Button(delMat.c_str())) {
											model.modelData.materials.erase(model.modelData.materials.begin() + index - 1);
											break;
										}

										model.modelData.materials[index - 1] = m;

										index++;
									}

									ImGui::TreePop();
								}


								model.componentTransform.position = pos;
								model.componentTransform.scale = sca;
								model.componentTransform.roll = rot.x;
								model.componentTransform.pitch = rot.y;
								model.componentTransform.yaw = rot.z;
								obj.components.models[index] = model;
								break;
							}
							index++;
						}
						index = 0;
						for each (PLightComponent pl in obj.components.pointLights) {
							if (pl.componentTransform.selected) {
								ImGui::Checkbox("Active##plActive", &pl.light.active);
								pl.light.colour_amb = inputColour("Ambient##plAmbient", { pl.light.colour_amb, 1.0f }, true);
								pl.light.colour_diff = inputColour("Diffuse##plDiffuse", { pl.light.colour_diff, 1.0f }, true);
								pl.light.colour_spec = inputColour("Specular##plSpecular", { pl.light.colour_spec, 1.0f }, true);
								pl.light.linear = inputFloat("Linear Const##plLinear", pl.light.linear, 0.01f, 0.1f);
								pl.light.quadratic = inputFloat("Quadratic Const##plQuad", pl.light.quadratic, 0.01f, 0.1f);

								pl.componentTransform.position = pos;
								pl.componentTransform.scale = sca;
								pl.componentTransform.roll = rot.x;
								pl.componentTransform.pitch = rot.y;
								pl.componentTransform.yaw = rot.z;
								obj.components.pointLights[index] = pl;
								break;
							}
							index++;
						}

						ImGui::TreePop();
					}
				}
				

				ImGui::TreePop();
			}

			for each (unsigned int scriptID in obj.componentScriptIDs) {
				if (ImGui::TreeNode(physics.scripts[scriptID]->GetName().c_str())) {
					if (ImGui::Button("Delete##DelScript")) {
						obj.deleteScript(scriptID);
						client.world.setObject(client.world.getSelected(), obj);
						ImGui::TreePop();
						ImGui::TreePop();
						return;

					}
					ImGui::Checkbox("Enabled", &obj.componentScriptData[scriptID].enabled);

					//Bool values
					for each (std::pair<std::string, bool> data in obj.componentScriptData[scriptID].variable_bool) {
						ImGui::Checkbox(data.first.c_str(), &data.second);
						obj.componentScriptData[scriptID].variable_bool[data.first] = data.second;
					}
					//Int values
					for each (std::pair<std::string, int> data in obj.componentScriptData[scriptID].variable_int) {
						ImGui::DragInt(data.first.c_str(), &data.second, 1);
						obj.componentScriptData[scriptID].variable_int[data.first] = data.second;
					}
					//Unsigned int values
					for each (std::pair<std::string, unsigned int> data in obj.componentScriptData[scriptID].variable_uint) {
						int x = (int)data.second;
						ImGui::DragInt(data.first.c_str(), &x, 1, 0, 4294967295);
						if (x < 0) { x = 0; }
						obj.componentScriptData[scriptID].variable_uint[data.first] = (unsigned int)x;
					}
					//Float values
					for each (std::pair<std::string, float> data in obj.componentScriptData[scriptID].variable_float) {
						ImGui::DragFloat(data.first.c_str(), &data.second, 0.1f);
						obj.componentScriptData[scriptID].variable_float[data.first] = data.second;
					}
					//String values
					for each (std::pair<std::string, std::string> data in obj.componentScriptData[scriptID].variable_string) {
						char result[1024];
						strcpy_s(result, data.second.c_str());

						ImGui::InputText(data.first.c_str(), result, 1024);
						obj.componentScriptData[scriptID].variable_string[data.first] = result;
					}
					ImGui::TreePop();
				}
			}

			ImGui::TreePop(); //Form the details tree.
		}
		client.world.setObject(client.world.getSelected(), obj);
	}
}



std::string EngineInterface::inputText(std::string value, std::string id) {
	char name[255];
	strcpy_s(name, 255, value.c_str());
	ImGui::InputText(id.c_str(), name, sizeof(name));
	std::string name_s = name;
	if (name_s.length() > 0) {
		return name_s;
	}
	else {
		return value;
	}
}


glm::vec4 EngineInterface::inputColour(std::string id, glm::vec4 original, bool opaque) {
	float voidCol[4];
	voidCol[0] = original.x;
	voidCol[1] = original.y;
	voidCol[2] = original.z;
	voidCol[3] = original.w;
	if (opaque) {
		ImGui::ColorEdit3(id.c_str(), voidCol);
	}
	else {
		ImGui::ColorEdit4(id.c_str(), voidCol);
	}
	
	return glm::vec4(voidCol[0], voidCol[1], voidCol[2], voidCol[3]);
}


glm::vec3 EngineInterface::inputVec3(std::string id, glm::vec3 original) {
	ImGui::InputFloat3(id.c_str(), &original[0]);

	return glm::vec3(original.x, original.y, original.z);
}

float EngineInterface::inputFloat(std::string id, float original, float smallStep, float quickStep) {
	ImGui::InputFloat(id.c_str(), &original, smallStep, quickStep);
	return original;
}


void EngineInterface::deselectAllComponents(Object &obj) {
	unsigned int index = 0;
	for each (CamComponent cam in obj.components.cameras) {
		cam.componentTransform.selected = false;
		obj.components.cameras[index] = cam;
		index++;
	}
	index = 0;
	for each (ModelComponent model in obj.components.models) {
		model.componentTransform.selected = false;
		obj.components.models[index] = model;
		index++;
	}
	index = 0;
	for each (PLightComponent pl in obj.components.pointLights) {
		pl.componentTransform.selected = false;
		obj.components.pointLights[index] = pl;
		index++;
	}
}