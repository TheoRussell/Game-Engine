
#include "Shader.h"

#include <string>
#include <fstream>
#include <iostream>


Shader::Shader(const GLchar* vertexShaderPath, const GLchar* fragShaderPath)
{
	std::string v_shader = getShader(vertexShaderPath);
	std::string f_shader = getShader(fragShaderPath);
	const char* v_ShaderCode = v_shader.c_str();
	const char* f_ShaderCode = f_shader.c_str();
	//Vertex shader.
	unsigned int vertex, fragment;
	int errorLength;
	char infoLog[512];

	genVertex(vertex, v_shader.c_str());
	genFragment(fragment, f_shader.c_str());

	//Generate the shader program.
	programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, fragment);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &errorLength);
	if (!errorLength) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILURE\n" << infoLog << std::endl;
	}
	//Delete unneccessary shaders.
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::Shader(const GLchar* vertexShaderPath, const GLchar* geomShaderPath, const GLchar* fragShaderPath)
{
	std::string v_shader = getShader(vertexShaderPath);
	std::string f_shader = getShader(fragShaderPath);
	std::string g_shader = getShader(geomShaderPath);

	//Vertex shader.
	unsigned int vertex, geometry, fragment;
	genVertex(vertex, v_shader.c_str());
	genGeometry(geometry, g_shader.c_str());
	genFragment(fragment, f_shader.c_str());
	//Generate the shader program.

	int errorLength;
	char infoLog[512];
	programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, geometry);
	glAttachShader(programID, fragment);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &errorLength);
	if (!errorLength) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILURE\n" << infoLog << std::endl;
	}
	//Delete unneccessary shaders.
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}


void Shader::genVertex(unsigned int &id, const char* code) {
	int errorLength;
	char infoLog[512];
	id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(id, 1, &code, nullptr);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &errorLength);
	if (!errorLength) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::genFragment(unsigned int &id, const char* code) {
	int errorLength;
	char infoLog[512];
	id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(id, 1, &code, nullptr);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &errorLength);
	if (!errorLength) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::genGeometry(unsigned int &id, const char* code) {
	int errorLength;
	char infoLog[512];
	id = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(id, 1, &code, nullptr);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &errorLength);
	if (!errorLength) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}



std::string Shader::getShader(std::string path) {
	std::ifstream file;
	std::string vshaderSource = "";
	
	try {
		file.open(path);

		if (!file) {
			std::cout << "No shader present at " << path << std::endl;
		}
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
