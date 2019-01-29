#pragma once

#define H_BinaryFile

#include <GLM\glm.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

enum BINARYWrite {
	//Interfaces
	BINARY_Label,
	BINARY_Box,
	BINARY_Line,
	BINARY_Poly,
	BINARY_Percent,
	BINARY_Button,
	//Scenes
	BINARY_Object,
	BINARY_Physics,
	BINARY_Camera,
	BINARY_Model,
	BINARY_Script,
	BINARY_PLight,
	BINARY_END,
	BINARY_EOO
};



class BinaryFiles
{
public:


	BinaryFiles();
	~BinaryFiles();

	static std::string getString(std::ifstream & s);

	static BINARYWrite getBINARYType(std::ifstream & s);

	static glm::vec2 getVec2(std::ifstream & s);

	static glm::vec3 getVec3(std::ifstream & s);

	static glm::vec4 getVec4(std::ifstream & s);

	static float getFloat(std::ifstream & s);

	static bool getBool(std::ifstream & s);

	static int getInt(std::ifstream & s);

	static void writeString(std::ofstream & s, std::string & value);

	static void writeFloat(std::ofstream & s, float & value);

	static void writeBINARYType(std::ofstream & s, BINARYWrite & type);

	static void writeVec4(std::ofstream & s, glm::vec4 & value);

	static void writeVec3(std::ofstream & s, glm::vec3 & value);

	static void writeVec2(std::ofstream & s, glm::vec2 & value);

	static void writeBool(std::ofstream & s, bool & value);

	static void writeInt(std::ofstream & s, int & value);
};

