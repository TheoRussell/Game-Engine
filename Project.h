#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm> // Lowercase for string
#include <filesystem>
#include <ws2tcpip.h> //This library is used for generating file directories in Windows.
#include <stdio.h> //Used for deleting files (ie the session lock).

#ifndef H_BinaryFile
#include "BinaryFiles.h"
#endif

class Project
{
public:
	Project();
	~Project();

	void generate(std::string name);
	void save(std::string projVersion);
	std::string getActive();
	void setActive();
	void load(std::string projVersion);

	void save();

	std::string fileName = "project";
	std::string name = "Project1";
	std::string directory;
	std::string startScene = "Scene";
	std::string startInterface = "UI1";

private:

};

