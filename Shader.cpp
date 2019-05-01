
#include "Shader.h"

#include <string>
#include <fstream>
#include <iostream>


Shader::Shader(const GLchar* vertexShaderPath, const GLchar* fragShaderPath)
{
	//Program with a vertex and fragment shader.
	unsigned int vertex, fragment;
	generateShader(vertex, getShader(vertexShaderPath).c_str(), GL_VERTEX_SHADER);
	generateShader(fragment, getShader(fragShaderPath).c_str(), GL_FRAGMENT_SHADER);

	
	CompileProgram({ vertex, fragment });
}

Shader::Shader(const GLchar* vertexShaderPath, const GLchar* geomShaderPath, const GLchar* fragShaderPath)
{
	//Program with vertex, geometry and fragment shader.
	unsigned int vertex, geometry, fragment;
	generateShader(vertex, getShader(vertexShaderPath) .c_str(), GL_VERTEX_SHADER);
	generateShader(geometry, getShader(geomShaderPath) .c_str(), GL_GEOMETRY_SHADER);
	generateShader(fragment, getShader(fragShaderPath) .c_str(), GL_FRAGMENT_SHADER);

	CompileProgram({vertex, geometry, fragment});
}


void Shader::CompileProgram(std::vector<unsigned int> shaders) {
	//Generate the shader program (compiled from the other shaders into 1).
	int errorLength;
	char infoLog[512];
	programID = glCreateProgram();
	for (int shaderID : shaders) {
		glAttachShader(programID, shaderID);
	}
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &errorLength);
	if (!errorLength) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILURE\n" << infoLog << std::endl;
	}
	//Delete unneccessary shaders.
	for (int shaderID : shaders) {
		glDeleteShader(shaderID);
	}
}


void Shader::generateShader(unsigned int &id, const char* code, int shader_type) {
	//Generating the shader program from the file.
	int errorLength;
	char infoLog[512];
	id = glCreateShader(shader_type); //Generates a shader ID.
	glShaderSource(id, 1, &code, nullptr);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &errorLength);
	if (!errorLength) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << shader_type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}



std::string Shader::getShader(std::string path) {
	std::ifstream file;
	std::string vshaderSource = "";
	//Reading the shader text files.
	try {
		file.open(path);

		if (!file) {
			std::cout << "No shader present at " << path << std::endl;
		}
		//Reads the whole file into the vshaderSource string.
		std::string curLine;
		while (getline(file, curLine)) {
			vshaderSource += curLine + "\n";
		}
		return vshaderSource;
	}
	catch (std::ifstream::failure e) {
		std::cout << "Unable to open shader..." << std::endl;
	}
	return vshaderSource;
}

void Shader::use() {
	glUseProgram(programID);
}

void Shader::setV3I(const std::string &uniformName, int x, int y, int z) const {
	glUniform3f(glGetUniformLocation(programID, uniformName.c_str()), x,y,z);
}

void Shader::setV3F(const std::string &uniformName, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(programID, uniformName.c_str()), x, y, z);
}

void Shader::setF(const std::string &uniformName, float value) const {
	glUniform1f(glGetUniformLocation(programID, uniformName.c_str()), value);
}

void Shader::setB(const std::string &uniformName, bool value) const {
	glUniform1i(glGetUniformLocation(programID, uniformName.c_str()), (int)value);
}

void Shader::setI(const std::string &uniformName, int value) const {
	glUniform1i(glGetUniformLocation(programID, uniformName.c_str()), value);
}

void Shader::setM4FV(const std::string &uniformName, glm::mat4 matrix) {
	unsigned int loc = glGetUniformLocation(programID, uniformName.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::deleteProgram() {
	glDeleteProgram(programID);
}


Shader::~Shader()
{
}
