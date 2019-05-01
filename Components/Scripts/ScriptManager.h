#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem> //Searching directories.
#include <stdio.h> //Deleting files.

class ScriptManager
{
public:
	ScriptManager();
	~ScriptManager();
	void NewScript();
	void NewScript(std::string name);
	void DeleteScript(std::string name);
	void DeleteScript(std::string name, bool complete);

private:
	void genCPP(std::string name);
	void genH(std::string name);
	void modifyVCXProj(std::string name);
	void deleteVCXProj(std::string name);
	void modifyVCXProjFilters(std::string name);
	void deleteVCXProjFilters(std::string name);
	bool isValid(std::string name);
	void modifyApplicationCPP(std::string name);
	void deleteApplicationCPP(std::string name);
};

