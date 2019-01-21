#include "BinaryFiles.h"



BinaryFiles::BinaryFiles()
{
}


BinaryFiles::~BinaryFiles()
{
}

std::string BinaryFiles::getString(std::ifstream &s) {
	unsigned int size = 0;
	s.read(reinterpret_cast<char *>(&size), sizeof(size));
	char *buffer = new char[size];
	s.read(buffer, size);
	std::string result = "";
	result.append(buffer, size);
	delete[] buffer;
	return result;
}

BINARYWrite BinaryFiles::getBINARYType(std::ifstream &s) {
	BINARYWrite writeType = BINARY_Label;
	s.read((char*)(&writeType), sizeof(BINARYWrite));
	return writeType;
}

glm::vec2 BinaryFiles::getVec2(std::ifstream &s) {
	glm::vec2 writeType(0.0f);
	s.read((char*)(&writeType), sizeof(glm::vec2));
	return writeType;
}

glm::vec3 BinaryFiles::getVec3(std::ifstream &s) {
	glm::vec3 writeType(0.0f);
	s.read((char*)(&writeType), sizeof(glm::vec3));
	return writeType;
}

glm::vec4 BinaryFiles::getVec4(std::ifstream &s) {
	glm::vec4 val(0.0f);
	s.read((char*)(&val), sizeof(glm::vec4));
	return val;
}

float BinaryFiles::getFloat(std::ifstream &s) {
	float val = 0.0f;
	s.read((char*)(&val), sizeof(float));
	return val;
}

bool BinaryFiles::getBool(std::ifstream &s) {
	bool val = false;
	s.read((char*)(&val), sizeof(bool));
	return val;
}

int BinaryFiles::getInt(std::ifstream &s) {
	int val = 0;
	s.read((char*)(&val), sizeof(int));
	return val;
}

void BinaryFiles::writeString(std::ofstream &s, std::string &value) {
	unsigned int size = value.size();
	s.write(reinterpret_cast<char *>(&size), sizeof(size));
	s.write(value.c_str(), size);
}

void BinaryFiles::writeFloat(std::ofstream &s, float &value) {
	s.write((char*)&value, sizeof(float));
}

void BinaryFiles::writeBINARYType(std::ofstream &s, BINARYWrite &type) {
	s.write((char *)(&type), sizeof(BINARYWrite));
}

void BinaryFiles::writeVec4(std::ofstream &s, glm::vec4 &value) {
	s.write((char*)&value[0], sizeof(glm::vec4));
}

void BinaryFiles::writeVec3(std::ofstream &s, glm::vec3 &value) {
	s.write((char*)&value[0], sizeof(glm::vec3));
}

void BinaryFiles::writeVec2(std::ofstream &s, glm::vec2 &value) {
	s.write((char*)&value[0], sizeof(glm::vec2));
}

void BinaryFiles::writeBool(std::ofstream &s, bool &value) {
	s.write((char*)&value, sizeof(bool));
}

void BinaryFiles::writeInt(std::ofstream &s, int &value) {
	s.write((char*)&value, sizeof(int));
}