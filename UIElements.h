#pragma once

#ifndef H_BinaryFile
#include "BinaryFiles.h"
#endif // !H_BinaryFile




struct Line {
	glm::vec2 startPos = glm::vec2(0.0f,0.0f);
	glm::vec2 endPos = glm::vec2(1.0f,1.0f);

	glm::vec4 colour = glm::vec4(255,255,255,255);

	bool selected = false;


	std::string id = "000000";
};




struct UIRectangle {
	glm::vec4 cornerA = glm::vec4(255, 255, 255, 255);
	glm::vec4 cornerB = glm::vec4(255, 255, 255, 255);
	glm::vec4 cornerC = glm::vec4(255, 255, 255, 255);
	glm::vec4 cornerD = glm::vec4(255, 255, 255, 255);
	glm::vec2 topLeft = glm::vec2(0.46875f, 0.46875f);
	glm::vec2 lowRight = glm::vec2(0.53125f, 0.53125f);

	glm::vec4 lineCol = glm::vec4(255, 255, 255, 255);
	bool filled = true;
	bool outlined = false;
	bool selected = true;
	std::string id = "000000";


	void setSolidFill(glm::vec4 col) {
		cornerA = col;
		cornerB = col;
		cornerC = col;
		cornerD = col;
	}
};


struct UIPolygon {
	std::vector<glm::vec2> points;
	glm::vec4 lineCol = glm::vec4(255, 255, 255, 255);
	glm::vec4 fillCol = glm::vec4(255, 255, 255, 255);
	bool filled = true;
	float thickness = 5.0f;
	bool selected = false;
	std::string id = "000000";
};


struct UILabel {
	std::string text = "Label1";
	float fontSize = 13.0f;
	glm::vec2 point = glm::vec2(0.5f, 0.5f);
	glm::vec4 foreColour = glm::vec4(255, 255, 255, 255);
	glm::vec4 backColour = glm::vec4(0, 0, 0, 255);
	glm::vec4 borderColour = glm::vec4(255, 255, 255, 255);

	void debugStyle() {
		text = "DEBUG";
		point = glm::vec2(0.0f, 0.0f);
		foreColour = glm::vec4(0,0,255,200);
	}

	std::string id = "000000";
	bool backFill = false;
	bool outLine = false;
	bool selected = false;
};


enum BAR_FILL_DIR {
	bfd_UPDOWN,
	bfd_LEFTRIGHT
};

struct UIPercentBar {
	glm::vec4 lineCol = glm::vec4(0, 0, 0, 255);

	glm::vec4 empty_fillCol = glm::vec4(64, 64, 64, 128);
	glm::vec4 full_fillCol = glm::vec4(150, 0, 0, 255);

	glm::vec2 topLeft = glm::vec2(0.46875f, 0.46875f);
	glm::vec2 lowRight = glm::vec2(0.53125f, 0.53125f);
	
	glm::vec2 barPos = glm::vec2(0.53125f, 0.53125f);

	bool selected = false;
	bool outlined = true;
	float percent = 0.75f;

	BAR_FILL_DIR barFillDirection = bfd_LEFTRIGHT;

	int paddingWidth = 1;
	std::string id = "000000";

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
			length = lowRight.x - topLeft.x;
			length *= percent;
			barPos.x = topLeft.x + length;
			barPos.y = lowRight.y;
		}
		else if (barFillDirection == bfd_UPDOWN) {
			length = topLeft.y - lowRight.y;
			length *= percent;
			barPos.y = lowRight.y + length;
			barPos.x = topLeft.x;
		}
		
	}
};



struct UIClipboard {
	std::vector<UIPolygon> polygon;
	std::vector<Line> line;
	std::vector<UILabel> label;
	std::vector<UIRectangle> rect;
	std::vector<UIPercentBar> bars;
};

enum UISelectType {
	s_Label,
	s_Rectangle,
	s_Poly,
	s_Line,
	s_PBar
};

class UIElements
{
public:

	const int IDLENGTH = 6;


	UIElements();
	~UIElements();

	void save(std::string WorkingDir, std::string UIName);

	void toItem(std::string command);

	void saveAll(std::string path);

	bool loadUI(std::string path);

	std::string intToChar(int v);

	std::string genID();

	void deselectAll();

	void addPercentBar();

	void addLine(glm::vec2 start, glm::vec2 end, glm::vec4 col = glm::vec4(255, 255, 255, 255));
	void addLine(Line l);
	void addLine();
	Line getLine(int index, int width, int height);

	void addRect(glm::vec4 cornerA, glm::vec4 cornerB, glm::vec4 cornerC, glm::vec4 cornerD, glm::vec4 topLeft, glm::vec4 lowRight);
	void addRect();
	void addRect(UIRectangle rect);
	UIRectangle getRect(int index, int width, int height);
	bool changeRectZOrder(int index, int layers);


	void addPolygon(std::vector<glm::vec2> points, glm::vec4 fillCol, glm::vec4 lineCol);

	UIPolygon getPolygon(int index, int width, int height);

	bool changePolygonZOrder(int index, int layers);

	void addLabel();

	void addLabel(std::string text);

	void addLabel(UILabel l);

	std::vector<Line> lines;
	std::vector<UIRectangle> rectangles;
	std::vector<UIPolygon> polygons;
	std::vector<UILabel> labels;
	std::vector<UIPercentBar> bars;
	UIClipboard clipboard;

	std::string UIName = "UI1";
};

