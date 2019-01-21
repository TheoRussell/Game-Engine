#pragma once
#include <GL/glew.h>

#include <string>
#include <fstream>
#include <iostream>

#include "GLM/glm.hpp"
#include "GLM/gtc/type_ptr.hpp"

class Shader
{
public:

	unsigned int programID;


	Shader(const GLchar* vertexShaderPath, const GLchar* fragShaderPath);
	Shader(const GLchar * vertexShaderPath, const GLchar * geomShaderPath, const GLchar * fragShaderPath);
	void use();
	void setM4FV(const std::string &uniformName, glm::mat4 matrix);
	void setV3F(const std::string &uniformName, float x, float y, float z) const;
	void setV3I(const std::string &uniformName, int x, int y, int z) const;
	void setF(const std::string &uniformName, float value) const;
	void setB(const std::string &uniformName, bool value) const;
	void setI(const std::string &uniformName, int value) const;
	void deleteProgram();
	~Shader();

private:
	void genVertex(unsigned int & id, const char * code);
	void genFragment(unsigned int & id, const char * code);
	void genGeometry(unsigned int & id, const char * code);
	std::string getShader(std::string path);
};