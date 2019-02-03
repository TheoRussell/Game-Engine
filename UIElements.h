#pragma once

#define UIELEMENT_H

#include "imgui\imgui.h" //Gui
#include "imgui\imgui_impl_glfw_gl3.h" //Example imgui


#ifndef H_BinaryFile
#include "BinaryFiles.h"
#endif // !H_BinaryFile


enum BAR_FILL_DIR {
	bfd_UPDOWN,
	bfd_LEFTRIGHT
};

enum UIType {
	s_Label,
	s_Rectangle,
	s_Poly,
	s_Line,
	s_PBar,
	s_Button
};




class InterfaceItem {
public:
	virtual void Render(int uiWidth, int uiHeight, float UIEx, float UIEy, float UIEzoom, float xDisplacement) {
		
	}
	//Returns a boolean. If true, the interface manager will delete the item.
	virtual bool DisplayData() {
		return false;
	}
	//Renders float inputs on current interface tab.
	void DisplayColours() {

		std::string buttonTxt = "Filled##Filled" + id;
		ImGui::Checkbox(buttonTxt.c_str(), &filled);

		if (filled) {
			buttonTxt = "Fill##FillColour" + id;
			ImGui::ColorPicker4(buttonTxt.c_str(), &FillColour[0]);
		}


		buttonTxt = "Outlined##Outlined" + id;
		ImGui::Checkbox(buttonTxt.c_str(), &outlined);
		if (outlined) {
			buttonTxt = "Outline##LineColour" + id;
			ImGui::ColorPicker4(buttonTxt.c_str(), &LineColour[0]);
		}


	}
	//Iterates through each of the vertices that represent the object and translate them.
	virtual void Move(glm::vec2 direction, int uiWidth, int uiHeight, float UIEx, float UIEy, float UIEzoom, float xDisplacement) {
		unsigned int index = 0;
		for (glm::vec2 vertex : vertices) {
			vertices[index] = screenToPoint(pointToScreen(vertex, uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement) + direction, uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);
			index++;
		}
	}
	//Scales each of the vertices.
	virtual void Scale(glm::vec2 scale, int uiWidth, int uiHeight, float UIEx, float UIEy, float UIEzoom, float xDisplacement) {
		unsigned int index = 0;
		for (glm::vec2 vertex : vertices) {
			vertices[index] = vertex * scale*10.0f;
			index++;
		}
	}

	virtual bool InBoundingBox(glm::vec2 mousePos) {
		return false;
	}

	virtual bool Save(std::ofstream& output) {
		return false;
	}
	virtual bool Load(std::ifstream& input) {
		return false;
	}


	bool pointInRect(glm::vec2 point, glm::vec2 topLeft, glm::vec2 lowRight) {
		if (point.x >= topLeft.x && point.x <= lowRight.x && point.y >= topLeft.y && point.y <= lowRight.y) {
			return true;
		}
		return false;
	}

	glm::vec2 pointToScreen(glm::vec2 point, int uiWidth, int uiHeight, float UIEx, float UIEy, float UIEzoom, float xDisplacement) {
		glm::vec2 pos = point;
		pos.x = pos.x*UIEzoom*uiWidth + xDisplacement + UIEx;
		pos.y = pos.y*UIEzoom*uiHeight + UIEy;
		return pos;
	}

	glm::vec2 screenToPoint(glm::vec2 point, int uiWidth, int uiHeight, float UIEx, float UIEy, float UIEzoom, float xDisplacement) {
		glm::vec2 pos = point;
		pos.x = (pos.x - UIEx - xDisplacement) / (UIEzoom * uiWidth);
		pos.y = (pos.y - UIEy) / (UIEzoom * uiHeight);
		return pos;
	}


	UIType GetType() {
		return type;
	}

	std::vector<glm::vec2> GetVertices() {
		return vertices;
	}

	glm::vec4 GetFillColour() {
		return FillColour;
	}

	bool GetFilled() {
		return filled;
	}

	void SetFillColour(glm::vec4 colour) {
		FillColour = colour;
	}

	void SetFilled(bool fill) {
		filled = fill;
	}

	glm::vec4 GetLineColour() {
		return LineColour;
	}

	bool GetOutlined() {
		return outlined;
	}

	void SetLineColour(glm::vec4 colour) {
		LineColour = colour;
	}

	void SetOutlined(bool outline) {
		outlined = outline;
	}


	bool GetSelected() {
		return selected;
	}
	void SetSelected(bool select) {
		selected = select;
	}

	void SetID(std::string _id) {
		id = _id;
	}
	std::string GetID() {
		return id;
	}

	void SetOutlineColour(glm::vec4 colour) {
		LineColour = colour;
	}


private:
	
protected:
	bool filled = true;
	bool outlined = true;
	bool selected = true;
	std::vector<glm::vec2> vertices;
	glm::vec4 FillColour = glm::vec4(255, 255, 255, 255);
	glm::vec4 LineColour = glm::vec4(255, 255, 255, 255);
	std::string id = "000000";
	UIType type;
};


class UIButton : public InterfaceItem {
public:
	bool Clicked = false;


	UIButton() {
		vertices.push_back({ 0.46875f, 0.46875f }); // Top left
		vertices.push_back({ 0.53125f, 0.53125f }); // Low right
		type = s_Button;
	}

	bool Save(std::ofstream& output) override {
		try {
			BINARYWrite bw = BINARY_Button;
			BinaryFiles::writeBINARYType(output, bw);
			BinaryFiles::writeString(output, id);
			BinaryFiles::writeVec4(output, FillColour);
			BinaryFiles::writeVec4(output, LineColour);
			BinaryFiles::writeVec2(output, vertices[0]);
			BinaryFiles::writeVec2(output, vertices[1]);
			BinaryFiles::writeBool(output, filled);
			BinaryFiles::writeBool(output, outlined);
		}
		catch (std::exception ex) {
			return false;
		}
		return true;
	}

	bool Load(std::ifstream& input) override {
		try {
			vertices.clear();
			id = BinaryFiles::getString(input);
			FillColour = BinaryFiles::getVec4(input);
			LineColour = BinaryFiles::getVec4(input);
			vertices.push_back(BinaryFiles::getVec2(input));
			vertices.push_back(BinaryFiles::getVec2(input));
			filled = BinaryFiles::getBool(input);
			outlined = BinaryFiles::getBool(input);
		}
		catch (std::exception ex) {
			return false;
		}
		return true;
	}


	bool InBoundingBox(glm::vec2 mouse) override {
		if (pointInRect(mouse, vertices[0], vertices[1])) {
			return true;
		}
		return false;
	}


	void Render(int uiWidth, int uiHeight, float UIEx, float UIEy, float UIEzoom, float xDisplacement) override {
		glm::vec2 topLeft = pointToScreen(vertices[0], uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);
		glm::vec2 lowRight = pointToScreen(vertices[1], uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);

		ImGuiIO & io = ImGui::GetIO();
		ImDrawList * drawList = ImGui::GetOverlayDrawList();

		if (filled) {
			drawList->AddRectFilled({ topLeft.x, topLeft.y }, { lowRight.x, lowRight.y }, ImGui::ColorConvertFloat4ToU32({ FillColour.x, FillColour.y, FillColour.z, FillColour.w }));
		}
		if (outlined) {
			drawList->AddRect({ topLeft.x, topLeft.y }, { lowRight.x, lowRight.y }, ImGui::ColorConvertFloat4ToU32({ LineColour.x, LineColour.y, LineColour.z, LineColour.w }));
		}
	}

	bool DisplayData() override {
		if (selected) {
			std::string buttonTxt = "Delete##buttonDel" + id;
			ImGui::TextDisabled("Button");
			if (ImGui::Button(buttonTxt.c_str())) {
				return true;
			}
			DisplayColours();
		}
		return false;
	}
};

class UIPBar : public InterfaceItem {

public:
	int paddingWidth = 1;
	glm::vec4 empty_fillCol = glm::vec4(64, 64, 64, 128);
	glm::vec2 barPos = glm::vec2(0.53125f, 0.53125f);
	float percent = 0.75f;

	BAR_FILL_DIR barFillDirection = bfd_LEFTRIGHT;

	UIPBar() {
		vertices.push_back({ 0.46875f, 0.46875f }); // Top left
		vertices.push_back({ 0.53125f, 0.53125f }); // Low right
		type = s_PBar;
	}

	bool Load(std::ifstream& input) override {
		try {
			vertices.clear();
			id = BinaryFiles::getString(input);
			barFillDirection = (BAR_FILL_DIR)BinaryFiles::getInt(input);
			empty_fillCol = BinaryFiles::getVec4(input);
			FillColour = BinaryFiles::getVec4(input);
			LineColour = BinaryFiles::getVec4(input);
			filled = BinaryFiles::getBool(input);
			outlined = BinaryFiles::getBool(input);
			paddingWidth = BinaryFiles::getInt(input);
			percent = BinaryFiles::getFloat(input);
			vertices.push_back(BinaryFiles::getVec2(input));
			vertices.push_back(BinaryFiles::getVec2(input));
		}
		catch (std::exception ex) {
			return false;
		}
		return true;
	}

	bool Save(std::ofstream& output) override {
		try {
			BINARYWrite bw = BINARY_Percent;
			BinaryFiles::writeBINARYType(output, bw);
			BinaryFiles::writeString(output, id);
			int pbFillDir = barFillDirection;
			BinaryFiles::writeInt(output, pbFillDir);
			BinaryFiles::writeVec4(output, empty_fillCol);
			BinaryFiles::writeVec4(output, FillColour);
			BinaryFiles::writeVec4(output, LineColour);
			BinaryFiles::writeBool(output, filled);
			BinaryFiles::writeBool(output, outlined);
			BinaryFiles::writeInt(output, paddingWidth);
			BinaryFiles::writeFloat(output, percent);
			BinaryFiles::writeVec2(output, vertices[0]);
			BinaryFiles::writeVec2(output, vertices[1]);
		}
		catch (std::exception ex) {
			return false;
		}
		return true;
	}

	void Render(int uiWidth, int uiHeight, float UIEx, float UIEy, float UIEzoom, float xDisplacement) override {
		calcFill();
		glm::vec2 topLeft = pointToScreen(vertices[0], uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);
		glm::vec2 lowRight = pointToScreen(vertices[1], uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);
		glm::vec2 barPosition = pointToScreen(barPos, uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);

		ImGuiIO & io = ImGui::GetIO();
		ImDrawList * drawList = ImGui::GetOverlayDrawList();
		//Line
		if (outlined) {
			drawList->AddRectFilled({ topLeft.x, topLeft.y }, { lowRight.x, lowRight.y }, ImGui::ColorConvertFloat4ToU32({ LineColour.x, LineColour.y, LineColour.z, LineColour.w }));
		}
		//Fill
		drawList->AddRectFilled({ topLeft.x + paddingWidth, topLeft.y + paddingWidth }, { lowRight.x - paddingWidth, lowRight.y - paddingWidth }, ImGui::ColorConvertFloat4ToU32({ empty_fillCol.x, empty_fillCol.y, empty_fillCol.z, empty_fillCol.w }));
		
		if (barFillDirection == bfd_LEFTRIGHT) {
			drawList->AddRectFilled({ topLeft.x + paddingWidth, topLeft.y + paddingWidth }, { barPosition.x, barPosition.y - paddingWidth }, ImGui::ColorConvertFloat4ToU32({ FillColour.x, FillColour.y, FillColour.z, FillColour.w }));
		}

	}


	bool InBoundingBox(glm::vec2 mouse) override {
		if (pointInRect(mouse, vertices[0], vertices[1])) {
			return true;
		}
		return false;
	}

	void calcFill() {
		float length = 1.0f;
		//Error handling.
		if (percent > 1.0f) {
			percent = 1.0f;
		}
		else if (percent < 0.0f) {
			percent = 0.0f;
		}

		if (barFillDirection == bfd_LEFTRIGHT) {
			//Vertice[1] represents lowest left point.
			//Vertice[0] is the top left point.
			length = vertices[1].x - vertices[0].x;
			length *= percent;
			barPos.x = vertices[0].x + length;
			barPos.y = vertices[1].y;
		}
		else if (barFillDirection == bfd_UPDOWN) {
			length = vertices[0].y - vertices[1].y;
			length *= percent;
			barPos.y = vertices[1].y + length;
			barPos.x = vertices[0].x;
		}

	}

	bool DisplayData() override {
		if (selected) {
			std::string buttonTxt = "Delete##pbDel" + id;
			ImGui::TextDisabled("% BAR");
			if (ImGui::Button(buttonTxt.c_str())) {
				return true;
			}

			ImGui::DragInt("Padding##pbPad", &paddingWidth, 1.0f, 0, 100);
			float perc100 = percent * 100;
			ImGui::DragFloat("Percent##pbPerc", &perc100, 1.0f, 0, 100);
			percent = perc100 / 100.0f;

			DisplayColours();
			buttonTxt = "Empty##pbEmptyColour" + id;
			ImGui::ColorPicker4(buttonTxt.c_str(), &empty_fillCol[0]);

		}
		return false;
	}
};

class UILine : public InterfaceItem {
public:
	UILine() {
		vertices.push_back({0.0f, 0.0f});
		vertices.push_back({ 1.0f, 1.0f });
		type = s_Line;
	}

	UILine(glm::vec2 start, glm::vec2 end) {
		vertices.push_back(start);
		vertices.push_back(end);
		type = s_Line;
	}


	bool Load(std::ifstream& input) override {
		try {
			vertices.clear();
			id = BinaryFiles::getString(input);
			LineColour = BinaryFiles::getVec4(input);
			vertices.push_back(BinaryFiles::getVec2(input));
			vertices.push_back(BinaryFiles::getVec2(input));
		}
		catch (std::exception ex) {
			return false;
		}
		return true;
	}

	bool Save(std::ofstream& output) override {
		try {
			BINARYWrite bw = BINARY_Line;
			BinaryFiles::writeBINARYType(output, bw);
			BinaryFiles::writeString(output, id);
			BinaryFiles::writeVec4(output, LineColour);
			BinaryFiles::writeVec2(output, vertices[0]);
			BinaryFiles::writeVec2(output, vertices[1]);
		}
		catch (std::exception ex) {
			return false;
		}
		return true;
	}

	void Render(int uiWidth, int uiHeight, float UIEx, float UIEy, float UIEzoom, float xDisplacement) override {
		glm::vec2 startPos = pointToScreen(vertices[0], uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);
		glm::vec2 endPos = pointToScreen(vertices[1], uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);

		ImGuiIO & io = ImGui::GetIO();
		ImDrawList * drawList = ImGui::GetOverlayDrawList();

		drawList->AddLine({ startPos.x, startPos.y }, { endPos.x, endPos.y }, IM_COL32(LineColour.x, LineColour.y, LineColour.z, LineColour.w));
	}

	bool DisplayData() override {
		if (selected) {
			std::string buttonTxt = "Delete##lineDel" + id;
			ImGui::TextDisabled("Line");
			if (ImGui::Button(buttonTxt.c_str())) {
				return true;
			}

			DisplayColours();
		}
		return false;
	}
};

class UIRectangle : public InterfaceItem {
public:
	//Colours;
	glm::vec4 cornerA = glm::vec4(255, 255, 255, 255); //Top left
	glm::vec4 cornerB = glm::vec4(255, 255, 255, 255); //Top right
	glm::vec4 cornerC = glm::vec4(255, 255, 255, 255); //Low left
	glm::vec4 cornerD = glm::vec4(255, 255, 255, 255); //Low right
	bool solidFill = true;

	UIRectangle() {
		type = s_Rectangle;
		vertices.push_back({ 0.46875f, 0.46875f });
		vertices.push_back({ 0.53125f, 0.53125f });
	}
	UIRectangle(std::string _id, glm::vec4 _cornerA, glm::vec4 _cornerB, glm::vec4 _cornerC, glm::vec4 _cornerD, glm::vec4 _topLeft, glm::vec4 _lowRight) {
		id = _id;
		cornerA = _cornerA;
		cornerB = _cornerB;
		cornerC = _cornerC;
		cornerD = _cornerD;
		vertices.push_back(_topLeft);
		vertices.push_back(_lowRight);
	}

	bool Load(std::ifstream& input) override {
		try {
			vertices.clear();
			id = BinaryFiles::getString(input);
			cornerA = BinaryFiles::getVec4(input);
			cornerB = BinaryFiles::getVec4(input);
			cornerC = BinaryFiles::getVec4(input);
			cornerD = BinaryFiles::getVec4(input);
			vertices.push_back(BinaryFiles::getVec2(input));
			vertices.push_back(BinaryFiles::getVec2(input));
			FillColour = BinaryFiles::getVec4(input);
			LineColour = BinaryFiles::getVec4(input);
			solidFill = BinaryFiles::getBool(input);
			filled = BinaryFiles::getBool(input);
			outlined = BinaryFiles::getBool(input);
		}
		catch (std::exception ex) {
			return false;
		}
		return true;
	}

	bool Save(std::ofstream& output) override {
		try {
			BINARYWrite bw = BINARY_Box;
			BinaryFiles::writeBINARYType(output, bw);
			BinaryFiles::writeString(output, id);
			BinaryFiles::writeVec4(output, cornerA);
			BinaryFiles::writeVec4(output, cornerB);
			BinaryFiles::writeVec4(output, cornerC);
			BinaryFiles::writeVec4(output, cornerD);
			BinaryFiles::writeVec2(output, vertices[0]);
			BinaryFiles::writeVec2(output, vertices[1]);
			//New
			BinaryFiles::writeVec4(output, FillColour);
			BinaryFiles::writeVec4(output, LineColour);
			BinaryFiles::writeBool(output, solidFill);
			BinaryFiles::writeBool(output, filled);
			BinaryFiles::writeBool(output, outlined);
		}
		catch (std::exception ex) {
			return false;
		}
		return true;
	}

	void Render(int uiWidth, int uiHeight, float UIEx, float UIEy, float UIEzoom, float xDisplacement) override {
		glm::vec2 topLeft = pointToScreen(vertices[0], uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);
		glm::vec2 lowRight = pointToScreen(vertices[1], uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);

		ImGuiIO & io = ImGui::GetIO();
		ImDrawList * drawList = ImGui::GetOverlayDrawList();

		if (filled && solidFill) {
			drawList->AddRectFilled({ topLeft.x, topLeft.y }, { lowRight.x, lowRight.y }, ImGui::ColorConvertFloat4ToU32({FillColour.x , FillColour.y, FillColour.z, FillColour.w}));
		}
		else if (!filled && !solidFill) {
			drawList->AddRectFilledMultiColor({ topLeft.x, topLeft.y },
				{ lowRight.x, lowRight.y },
				ImGui::ColorConvertFloat4ToU32({ cornerA.x, cornerA.y, cornerA.z, cornerA.w }),
				ImGui::ColorConvertFloat4ToU32({ cornerB.x, cornerB.y, cornerB.z, cornerB.w }),
				ImGui::ColorConvertFloat4ToU32({ cornerC.x, cornerC.y, cornerC.z, cornerC.w }),
				ImGui::ColorConvertFloat4ToU32({ cornerD.x, cornerD.y, cornerD.z, cornerD.w }));
		}
		if (outlined) {
			drawList->AddRect({ topLeft.x, topLeft.y }, { lowRight.x, lowRight.y }, ImGui::ColorConvertFloat4ToU32({ LineColour.x, LineColour.y, LineColour.z, LineColour.w }));
		}
	}
	

	bool InBoundingBox(glm::vec2 mouse) override {
		if (pointInRect(mouse, vertices[0], vertices[1])) {
			return true;
		}
		return false;
	}

	void ToggleSolidFill() {
		solidFill != solidFill;
	}

	bool DisplayData() override {
		if (selected) {
			std::string buttonTxt = "Delete##rectDel" + id;
			ImGui::TextDisabled("Rectangle");
			if (ImGui::Button(buttonTxt.c_str())) {
				return true;
			}

			DisplayColours();
			if (!solidFill) {
				if (ImGui::TreeNode("Individual Corners")) {

					std::string buttonTxt = "TopLeft##rectTLColour" + id;
					ImGui::ColorPicker4(buttonTxt.c_str(), &cornerA[0]);

					buttonTxt = "TopRight##rectTLColour" + id;
					ImGui::ColorPicker4(buttonTxt.c_str(), &cornerB[0]);

					buttonTxt = "LowLeft##rectTLColour" + id;
					ImGui::ColorPicker4(buttonTxt.c_str(), &cornerC[0]);

					buttonTxt = "LowRight##rectTLColour" + id;
					ImGui::ColorPicker4(buttonTxt.c_str(), &cornerD[0]);

					ImGui::TreePop();
				}
			}
		}
		return false;
	}
};

class UILabel : public InterfaceItem {
public:

	std::string text = "Label1";
	float fontSize = 13.0f;
	glm::vec4 foreColour = glm::vec4(0, 0, 0, 255);

	UILabel() {
		vertices.push_back({0.0f, 0.0f}); //The point that the label is at.
		type = s_Label;
	}

	bool Load(std::ifstream& input) override {
		try {
			vertices.clear();
			id = BinaryFiles::getString(input);
			vertices.push_back(BinaryFiles::getVec2(input));
			text = BinaryFiles::getString(input);
			fontSize = BinaryFiles::getFloat(input);
			FillColour = BinaryFiles::getVec4(input);
			foreColour = BinaryFiles::getVec4(input);
			LineColour = BinaryFiles::getVec4(input);
			outlined = BinaryFiles::getBool(input);
			filled = BinaryFiles::getBool(input);
		}
		catch (std::exception ex) {
			return false;
		}
		return true;
	}


	bool Save(std::ofstream& file) override {
		try {
			BINARYWrite bw = BINARY_Label;
			BinaryFiles::writeBINARYType(file, bw);
			BinaryFiles::writeString(file, id);
			BinaryFiles::writeVec2(file, vertices[0]);
			BinaryFiles::writeString(file, text);
			BinaryFiles::writeFloat(file, fontSize);
			BinaryFiles::writeVec4(file, FillColour);
			BinaryFiles::writeVec4(file, foreColour);
			BinaryFiles::writeVec4(file, LineColour);
			BinaryFiles::writeBool(file, outlined);
			BinaryFiles::writeBool(file, filled);
		}
		catch (std::exception ex) {
			return false;
		}
		return true;
	}


	void Scale(glm::vec2 scale) {
		fontSize += scale.x * 10;
	}


	void Render(int uiWidth, int uiHeight, float UIEx, float UIEy, float UIEzoom, float xDisplacement) override {
		ImFont *font = ImGui::GetFont();

		ImGuiIO & io = ImGui::GetIO();
		ImDrawList * drawList = ImGui::GetOverlayDrawList();

		glm::vec2 point = pointToScreen(vertices[0], uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);
		ImVec2 txtSize = ImGui::CalcTextSize(text.c_str());
		if (filled) {
			drawList->AddRectFilled({ point.x - 5, point.y - 5 }, { point.x + (txtSize.x*(fontSize / font->FontSize)) + 5, point.y + txtSize.y + 5 }, ImColor(FillColour.x, FillColour.y, FillColour.z, FillColour.w), 1.0f);
		}
		if (outlined) {
			drawList->AddRect({ point.x - 5, point.y - 5 }, {  point.x + (txtSize.x*(fontSize / font->FontSize)) + 5, point.y + txtSize.y + 5 }, ImColor(LineColour.x, LineColour.y, LineColour.x, LineColour.w), 1.0f);
		}

		drawList->AddText(
			font,
			fontSize,
			{ point.x, point.y },
			ImGui::ColorConvertFloat4ToU32({ foreColour.x, foreColour.y, foreColour.z, foreColour.w }),
			text.c_str(), 0, 0.0f, 0);

	}

	void DebugStyle() {
		text = "DEBUG";
		vertices[0] = glm::vec2(0.0f, 0.0f);
		foreColour = glm::vec4(0, 0, 255, 200);
	}

	bool DisplayData() {
		if (selected) {
			std::string buttonTxt = "Delete##lblDel" + id;
			ImGui::TextDisabled("Label");
			if (ImGui::Button(buttonTxt.c_str())) {
				return true;
			}

			buttonTxt = "##lblText" + id;
			char buffer[2048];
			strcpy_s(buffer, text.c_str());
			ImGui::InputTextMultiline(buttonTxt.c_str(), buffer, sizeof(buffer));
			text = buffer;

			DisplayColours();
		}
		return false;
	}
};

class UIPolygon : public InterfaceItem {
public:
	float thickness = 5.0f;

	UIPolygon() {
		type = s_Poly;
	}
	UIPolygon(std::vector<glm::vec2> _vertices, glm::vec4 fill, glm::vec4 outline) {
		vertices = _vertices;
		FillColour = fill;
		LineColour = outline;
	}

	bool InBoundingBox(glm::vec2 mouse) override {
		//Obtains a square around the polygon.
		glm::vec2 squarePointA(1.0f, 1.0f);
		glm::vec2 squarePointB(0.0f, 0.0f);
		for (glm::vec2 point : vertices) {
			if (point.x < squarePointA.x) {
				squarePointA.x = point.x;
			}
			if (point.y < squarePointA.y) {
				squarePointA.y = point.y;
			}
			if (point.x > squarePointB.x) {
				squarePointB.x = point.x;
			}
			if (point.y > squarePointB.y) {
				squarePointB.y = point.y;
			}
		}
		if (pointInRect(mouse, squarePointA, squarePointB)) {
			return true;
		}
		return false;
	}

	bool Load(std::ifstream& input) override {
		try {
			vertices.clear();

			id = BinaryFiles::getString(input);
			FillColour = BinaryFiles::getVec4(input);
			LineColour = BinaryFiles::getVec4(input);
			thickness = BinaryFiles::getFloat(input);
			filled = BinaryFiles::getBool(input);
			outlined = BinaryFiles::getBool(input);
			int vCount = BinaryFiles::getInt(input);

			if (vCount > 0) {
				for (int i = 0; i < vCount; i++) {
					vertices.push_back(BinaryFiles::getVec2(input));
				}
			}
		}
		catch (std::exception ex) {
			return false;
		}
		return true;
	}

	bool Save(std::ofstream& output) override {
		try {
			BINARYWrite bw = BINARY_Poly;
			BinaryFiles::writeBINARYType(output, bw);
			BinaryFiles::writeString(output, id);
			BinaryFiles::writeVec4(output, FillColour);
			BinaryFiles::writeVec4(output, LineColour);
			BinaryFiles::writeFloat(output, thickness);
			BinaryFiles::writeBool(output, filled);
			BinaryFiles::writeBool(output, outlined);

			int vertexCount = vertices.size();
			BinaryFiles::writeInt(output, vertexCount);
			for (glm::vec2 vertex : vertices) {
				BinaryFiles::writeVec2(output, vertex);
			}
		}
		catch (std::exception ex) {
			return false;
		}
		return true;
	}


	void Render(int uiWidth, int uiHeight, float UIEx, float UIEy, float UIEzoom, float xDisplacement) override {

		ImGuiIO & io = ImGui::GetIO();
		ImDrawList * drawList = ImGui::GetOverlayDrawList();


		if (vertices.size() > 0 && filled) {
			//A filled polygon.
			for (glm::vec2 point : vertices) {
				point = pointToScreen(point, uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);
				drawList->PathLineTo({ point.x, point.y });
			}
			drawList->PathFillConvex(IM_COL32(FillColour.x, FillColour.y, FillColour.z, FillColour.w));
		}
		else if (vertices.size() > 0 && !filled) {
			//Not filled, ie just an outline.
			std::vector<ImVec2> convPoints;
			//Generating a vector list of the points that need to be rendered to fit the function parameters.
			for (glm::vec2 polyPoint : vertices) {
				polyPoint = pointToScreen(polyPoint, uiWidth, uiHeight, UIEx, UIEy, UIEzoom, xDisplacement);

				ImVec2 x(polyPoint.x, polyPoint.y);
				convPoints.push_back(x);
			}
			drawList->AddPolyline(&convPoints[0], convPoints.size(), ImGui::ColorConvertFloat4ToU32({ LineColour.x, LineColour.y, LineColour.z, LineColour.w }), false, thickness);
		}
	}

	bool DisplayData() {
		if (selected) {
			std::string buttonTxt = "Delete##polyDel" + id;
			ImGui::TextDisabled("Polygon");
			if (ImGui::Button(buttonTxt.c_str())) {
				return true;
			}

			DisplayColours();
		}
		return false;
	}
};




class UIElements
{
public:

	const int IDLENGTH = 6;


	UIElements();
	~UIElements();

	void saveAll(std::ofstream & output);

	bool loadUI(std::ifstream & input);

	std::string intToChar(int v);

	std::string genID();

	void deselectAll();

	void GeneratePointers();

	void addPercentBar();
	void addLine(glm::vec2 start, glm::vec2 end, glm::vec4 col = glm::vec4(255, 255, 255, 255));
	void addRect();
	void addPolygon(std::vector<glm::vec2> points, glm::vec4 fillCol, glm::vec4 lineCol);
	void addLabel();
	void addButton();


	//Positioning of window.
	int uiWidth;
	int uiHeight;

	//Where the actual items will be stored.
	std::vector<UILine> lines;
	std::vector<UIRectangle> rectangles;
	std::vector<UIPolygon> polygons;
	std::vector<UILabel> labels;
	std::vector<UIPBar> bars;
	std::vector<UIButton> buttons;


	//Lists to pointers of items.
	std::vector<InterfaceItem*> Interface_Items;
	std::vector<InterfaceItem*> Interface_Clip;

	std::string UIName = "UI1";
};

