#include "UIElements.h"



UIElements::UIElements()
{

}


UIElements::~UIElements()
{
}


void UIElements::save(std::string WorkingDir, std::string UIName) {
	
}


void UIElements::toItem(std::string command) {

}

void UIElements::saveAll(std::string path) {
	try {
		std::ofstream output(path + UIName + ".gui", std::ofstream::out | std::ios::binary);
		output.clear();
		if (output.good()) {
			output.write(UIName.c_str(), sizeof(std::string));

			BINARYWrite bw = BINARY_Label;
			for each (UILabel lbl in labels) {
				BinaryFiles::writeBINARYType(output, bw);
				BinaryFiles::writeString(output, lbl.id);
				BinaryFiles::writeVec2(output, lbl.point);
				BinaryFiles::writeString(output, lbl.text);
				BinaryFiles::writeFloat(output, lbl.fontSize);
				BinaryFiles::writeVec4(output, lbl.backColour);
				BinaryFiles::writeVec4(output, lbl.foreColour);
				BinaryFiles::writeVec4(output, lbl.borderColour);
				BinaryFiles::writeBool(output, lbl.outLine);
				BinaryFiles::writeBool(output, lbl.backFill);
			}
			bw = BINARY_Box;
			for each (UIRectangle rect in rectangles) {
				BinaryFiles::writeBINARYType(output, bw);
				BinaryFiles::writeString(output, rect.id);
				BinaryFiles::writeVec4(output, rect.cornerA);
				BinaryFiles::writeVec4(output, rect.cornerB);
				BinaryFiles::writeVec4(output, rect.cornerC);
				BinaryFiles::writeVec4(output, rect.cornerD);
				BinaryFiles::writeVec2(output, rect.topLeft);
				BinaryFiles::writeVec2(output, rect.lowRight);
				BinaryFiles::writeBool(output, rect.filled);
				BinaryFiles::writeBool(output, rect.outlined);
			}
			bw = BINARY_Poly;
			for each (UIPolygon poly in polygons) {
				BinaryFiles::writeBINARYType(output, bw);
				BinaryFiles::writeString(output, poly.id);
				BinaryFiles::writeVec4(output, poly.fillCol);
				BinaryFiles::writeVec4(output, poly.lineCol);
				BinaryFiles::writeFloat(output, poly.thickness);
				BinaryFiles::writeBool(output, poly.filled);
				int vertexCount = poly.points.size();
				BinaryFiles::writeInt(output, vertexCount);
				for each (glm::vec2 vertex in poly.points) {
					BinaryFiles::writeVec2(output, vertex);
				}
			}
			bw = BINARY_Percent;
			for each (UIPercentBar pb in bars) {
				BinaryFiles::writeBINARYType(output, bw);
				BinaryFiles::writeString(output, pb.id);
				int pbFillDir = pb.barFillDirection;
				BinaryFiles::writeInt(output, pbFillDir);
				BinaryFiles::writeVec4(output, pb.empty_fillCol);
				BinaryFiles::writeVec4(output, pb.full_fillCol);
				BinaryFiles::writeVec4(output, pb.lineCol);
				BinaryFiles::writeBool(output, pb.outlined);
				BinaryFiles::writeInt(output, pb.paddingWidth);
				BinaryFiles::writeFloat(output, pb.percent);
				BinaryFiles::writeVec2(output, pb.topLeft);
				BinaryFiles::writeVec2(output, pb.lowRight);
			}

		}
		output.close();



		loadUI(path + UIName + ".gui");
	}
	catch (std::exception ex) {
		std::cout << ex.what() << std::endl;
	}
}

bool UIElements::loadUI(std::string path) {
	try {
		std::ifstream input(path, std::ifstream::in | std::ios::binary);
		input.exceptions(std::ifstream::badbit | std::ifstream::failbit); //Error handling.

		labels.clear();
		polygons.clear();
		lines.clear();
		bars.clear();
		rectangles.clear();

		if (input.good()) {

			UIName = BinaryFiles::getString(input);
			BINARYWrite writeType;

			while (input.good()) {
				writeType = BinaryFiles::getBINARYType(input);

				if (writeType == BINARY_Label) {
					//Loading label element.
					UILabel lbl;
					lbl.id = BinaryFiles::getString(input);
					lbl.point = BinaryFiles::getVec2(input);
					lbl.text = BinaryFiles::getString(input);
					lbl.fontSize = BinaryFiles::getFloat(input);
					lbl.backColour = BinaryFiles::getVec4(input);
					lbl.foreColour = BinaryFiles::getVec4(input);
					lbl.borderColour = BinaryFiles::getVec4(input);
					lbl.outLine = BinaryFiles::getBool(input);
					lbl.backFill = BinaryFiles::getBool(input);

					labels.push_back(lbl);
				}
				else if (writeType == BINARY_Box) {
					//Loading rectangle item.
					UIRectangle rect;
					rect.id = BinaryFiles::getString(input);
					rect.cornerA = BinaryFiles::getVec4(input);
					rect.cornerB = BinaryFiles::getVec4(input);
					rect.cornerC = BinaryFiles::getVec4(input);
					rect.cornerD = BinaryFiles::getVec4(input);
					rect.topLeft = BinaryFiles::getVec2(input);
					rect.lowRight = BinaryFiles::getVec2(input);
					rect.filled = BinaryFiles::getBool(input);
					rect.outlined = BinaryFiles::getBool(input);
					rectangles.push_back(rect);
				}
				else if (writeType == BINARY_Poly) {
					UIPolygon poly;
					poly.id = BinaryFiles::getString(input);
					poly.fillCol = BinaryFiles::getVec4(input);
					poly.lineCol = BinaryFiles::getVec4(input);
					poly.thickness = BinaryFiles::getFloat(input);
					poly.filled = BinaryFiles::getBool(input);
					int vCount = BinaryFiles::getInt(input);

					if (vCount > 0) {
						for (int i = 0; i < vCount; i++) {
							poly.points.push_back(BinaryFiles::getVec2(input));
						}
					}
					polygons.push_back(poly);
				}
				else if (writeType == BINARY_Percent) {
					UIPercentBar pb;
					pb.id = BinaryFiles::getString(input);
					pb.barFillDirection = (BAR_FILL_DIR)BinaryFiles::getInt(input);
					pb.empty_fillCol = BinaryFiles::getVec4(input);
					pb.full_fillCol = BinaryFiles::getVec4(input);
					pb.lineCol = BinaryFiles::getVec4(input);
					pb.outlined = BinaryFiles::getBool(input);
					pb.paddingWidth = BinaryFiles::getInt(input);
					pb.percent = BinaryFiles::getFloat(input);
					pb.topLeft = BinaryFiles::getVec2(input);
					pb.lowRight = BinaryFiles::getVec2(input);

					bars.push_back(pb);
				}


			}
		}


	} 
	catch (std::ifstream::failure ex) {
		return false;
	}
	return true;
}


std::string UIElements::intToChar(int v) {
	if (v >= 10) {
		char asciiVal = 55 + v;
		std::string val;
		val = asciiVal;
		return val;
	}
	else {
		return std::to_string(v);
	}
}

std::string UIElements::genID() {
	srand(rand());
	std::string id = "";
	for (int i = 0; i < IDLENGTH; i++) {
		//Random char from 0-9,A-Z.
		id.append(intToChar((int)std::fmodf((double)rand(), (double)36)));
	}
	return id;
}



void UIElements::deselectAll() {
	if (lines.size() > 0) {
		for (int i = 0; i < lines.size(); i++) {
			lines[i].selected = false;
		}
	}
	if (labels.size() > 0) {
		for (int i = 0; i < labels.size(); i++) {
			labels[i].selected = false;
		}
	}
	if (polygons.size() > 0) {
		for (int i = 0; i < polygons.size(); i++) {
			polygons[i].selected = false;
		}
	}
	if (rectangles.size() > 0) {
		for (int i = 0; i < rectangles.size(); i++) {
			rectangles[i].selected = false;
		}
	}
	if (bars.size() > 0) {
		for (int b = 0; b < bars.size(); b++) {
			bars[b].selected = false;
		}
	}
}


void UIElements::addPercentBar() {
	UIPercentBar pb;
	pb.id = genID();
	bars.push_back(pb);
}

void UIElements::addLine(glm::vec2 start, glm::vec2 end, glm::vec4 col) {
	Line l;
	l.colour = col;
	l.startPos = start;
	l.endPos = end;
	l.id = genID();
	lines.push_back(l);
}

void UIElements::addLine(Line l) {
	l.id = genID();
	lines.push_back(l);
}

void UIElements::addLine() {
	Line l = Line();
	l.id = genID();
	lines.push_back(l);
}


Line UIElements::getLine(int index, int width, int height) {
	if (index >= 0 && index < lines.size()) {
		Line r = lines[index];
		r.startPos.x *= width;
		r.startPos.y *= height;
		r.endPos.x *= width;
		r.endPos.y *= height;
		return r;
	}
	return Line();
}



UIRectangle UIElements::getRect(int index, int width, int height) {
	if (index >= 0 && index < rectangles.size()) {
		
		UIRectangle r = rectangles[index];
		r.topLeft.x *= width;
		r.topLeft.y *= height;
		r.lowRight.x *= width;
		r.lowRight.y *= height;
		return r;

	}
	return UIRectangle();
}

bool UIElements::changeRectZOrder(int index, int layers) {
	if ((layers > 0 && index < rectangles.size() - layers) | (layers < 0 && index > 0 && index < rectangles.size())) {
		//Moving the layer up or down.
		UIRectangle swapped = rectangles[index+layers];
		rectangles[index + layers] = rectangles[index];
		rectangles[index] = swapped;
		return true;
	}
	return false;
}

void UIElements::addRect(glm::vec4 cornerA, glm::vec4 cornerB, glm::vec4 cornerC, glm::vec4 cornerD, glm::vec4 topLeft, glm::vec4 lowRight) {
	UIRectangle x;
	x.cornerA = cornerA;
	x.cornerB = cornerB;
	x.cornerC = cornerC;
	x.cornerD = cornerD;
	x.lowRight = lowRight;
	x.topLeft = topLeft;

	x.id = genID();

	rectangles.push_back(x);
}

void UIElements::addRect() {
	UIRectangle rect = UIRectangle();
	rect.id = genID();
	rectangles.push_back(rect);
}

void UIElements::addRect(UIRectangle rect) {
	rect.id = genID();
	rectangles.push_back(rect);
}



void UIElements::addPolygon(std::vector<glm::vec2> points, glm::vec4 fillCol, glm::vec4 lineCol) {
	UIPolygon p;
	p.fillCol = fillCol;
	p.lineCol = lineCol;
	p.points = points;
	p.id = genID();
	polygons.push_back(p);
}

UIPolygon UIElements::getPolygon(int index, int width, int height) {
	if (index >= 0 && index < polygons.size()) {
		UIPolygon p = polygons[index];
		int i = 0;
		for each (glm::vec2 point in p.points) {
			point.x *= width;
			point.y *= height;
			p.points[i] = point;
			i++;
		}
		return p;
	}
	return UIPolygon();
}

bool UIElements::changePolygonZOrder(int index, int layers) {
	if ((layers > 0 && index < polygons.size() - layers) | (layers < 0 && index > 0 && index < polygons.size())) {
		UIPolygon swapped = polygons[index + layers];
		polygons[index + layers] = polygons[index];
		polygons[index] = swapped;
		return true;
	}
	return false;
}



void UIElements::addLabel() {
	UILabel l = UILabel();
	l.id = genID();
	labels.push_back(l);
}

void UIElements::addLabel(std::string text = "Label1") {
	UILabel l;
	l.text = text;
	l.id = genID();
	labels.push_back(l);
}

void UIElements::addLabel(UILabel l) { 
	l.id = genID();
	labels.push_back(l);
}