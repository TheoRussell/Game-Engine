#include "UIElements.h"



UIElements::UIElements()
{

}


UIElements::~UIElements()
{
}


void UIElements::saveAll(std::ofstream &output) {
	try {
		
		if (output.good()) {
			BinaryFiles::writeString(output, UIName);

			for (InterfaceItem* item : Interface_Items) {
				item->Save(output);
			}
		}
		
	}
	catch (std::exception ex) {
		std::cout << ex.what() << std::endl;
	}
}

bool UIElements::loadUI(std::ifstream &input) {
	try {
		Interface_Clip.clear();
		Interface_Items.clear();
		labels.clear();
		polygons.clear();
		lines.clear();
		bars.clear();
		rectangles.clear();
		buttons.clear();

		if (input.good()) {

			UIName = BinaryFiles::getString(input);
			BINARYWrite writeType = BINARY_EOO;

			while (input.good() && writeType != BINARY_END && writeType != BINARY_Script) {
				writeType = BinaryFiles::getBINARYType(input);

				if (writeType == BINARY_Label) {
					//Loading label element.
					UILabel lbl;
					lbl.Load(input);
					labels.push_back(lbl);
				}
				else if (writeType == BINARY_Box) {
					//Loading rectangle item.
					UIRectangle rect;
					rect.Load(input);
					rectangles.push_back(rect);
				}
				else if (writeType == BINARY_Poly) {
					//Loading polygon.
					UIPolygon poly;
					poly.Load(input);
					polygons.push_back(poly);
				}
				else if (writeType == BINARY_Percent) {
					//Percent bars.
					UIPBar pb;
					pb.Load(input);
					bars.push_back(pb);
				}
				else if (writeType == BINARY_Line) {
					//Lines
					UILine line;
					line.Load(input);
					line.SetID(genID());
					lines.push_back(line);
				}
				else if (writeType == BINARY_Button) {
					//Buttons.
					UIButton button;
					button.Load(input);
					buttons.push_back(button);
				}
			}
		}
		GeneratePointers();


	} 
	catch (std::ifstream::failure ex) {
		std::cout << ex.what() << std::endl;
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

	bool same = true;
	while (same) {
		for (int i = 0; i < IDLENGTH; i++) {
			//Random char from 0-9,A-Z.
			id.append(intToChar((int)std::fmodf((double)rand(), (double)36)));
		}
		//Check against other elements.
		same = false;
		for (InterfaceItem* item : Interface_Items) {
			if (item->GetID() == id) {
				same = true;
			}
		}
		
	}

	return id;
}

void UIElements::deselectAll() {
	for (InterfaceItem* item : Interface_Items) {
		item->SetSelected(false);
	}
}


void UIElements::GeneratePointers() {
	Interface_Items.clear();
	
	//Creating a pointer to each of the elements and storing it in a vector.

	for (int element = 0; element < bars.size(); element++) {
		Interface_Items.push_back(&bars[element]);
	}
	for (int element = 0; element < lines.size(); element++) {
		Interface_Items.push_back(&lines[element]);
	}
	for (int element = 0; element < rectangles.size(); element++) {
		Interface_Items.push_back(&rectangles[element]);
	}
	for (int element = 0; element < buttons.size(); element++) {
		Interface_Items.push_back(&buttons[element]);
	}
	for (int element = 0; element < labels.size(); element++) {
		Interface_Items.push_back(&labels[element]);
	}
	for (int element = 0; element < polygons.size(); element++) {
		Interface_Items.push_back(&polygons[element]);
	}
}


//Adding elements.
void UIElements::addPercentBar() {
	UIPBar pb = UIPBar();
	pb.SetID(genID());
	bars.push_back(pb);
	GeneratePointers();
}

void UIElements::addLine(glm::vec2 start, glm::vec2 end, glm::vec4 col) {
	UILine l(start, end);
	l.SetID(genID());
	lines.push_back(l);
	GeneratePointers();
}

void UIElements::addRect() {
	UIRectangle rect = UIRectangle();
	rect.SetID(genID());
	rectangles.push_back(rect);
	GeneratePointers();
}

void UIElements::addPolygon(std::vector<glm::vec2> points, glm::vec4 fillCol, glm::vec4 lineCol) {
	UIPolygon p(points, fillCol, lineCol);
	p.SetID(genID());
	polygons.push_back(p);
	GeneratePointers();
}

void UIElements::addLabel() {
	UILabel l = UILabel();
	l.SetID(genID());
	labels.push_back(l);
	GeneratePointers();
}

void UIElements::addButton() {
	UIButton button = UIButton();
	button.SetID(genID());
	buttons.push_back(button);
	GeneratePointers();
}