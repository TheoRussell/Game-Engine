#include "ScriptManager.h"



ScriptManager::ScriptManager()
{
}


ScriptManager::~ScriptManager()
{
}

void ScriptManager::NewScript() {
	NewScript("NewScript");
}

void ScriptManager::NewScript(std::string name) {
	if (isValid(name)) {
		genCPP(name);
		genH(name);
		modifyVCXProj(name);
		modifyVCXProjFilters(name);
		modifyApplicationCPP(name);
	}
}



void ScriptManager::DeleteScript(std::string name) {
	DeleteScript(name, false); //DEFAULTS TO NOT DELETING THE FILE.
}

void ScriptManager::DeleteScript(std::string name, bool complete) {
	if (!isValid(name)) {
		deleteVCXProj(name);
		deleteVCXProjFilters(name);
		deleteApplicationCPP(name);
		if (complete) {
			std::string cppName = "src\\Components\\Scripts\\" + name + ".cpp";
			std::remove(cppName.c_str());
			std::string hName = "src\\Components\\Scripts\\" + name + ".h";
			std::remove(hName.c_str());
		}
	}
}

bool ScriptManager::isValid(std::string name) {
	std::string fileName;
	for (auto & p : std::experimental::filesystem::directory_iterator("src\\Components\\Scripts\\")) {
		fileName = p.path().string();
		fileName.erase(0, 23);

		if (fileName.find(".cpp") != std::string::npos) {
			fileName.erase(fileName.length() - 4, 4);
		}
		else if (fileName.find(".h") != std::string::npos) {
			fileName.erase(fileName.length() - 2, 2);
		}
		if (fileName == name) {
			return false;
		}
	}
	return true;
}

void ScriptManager::modifyApplicationCPP(std::string name) {
	std::string path = "src\\Application.cpp";
	std::fstream infile(path, std::ios::in);
	std::vector<std::string> lines;
	std::string readLine;
	if (infile.good()) {
		while (std::getline(infile, readLine)) {
			lines.push_back(readLine);
		}
	}
	infile.close();
	std::fstream outfile("src\\Application.cpp", std::ios::out);
	if (outfile.good()) {
		outfile.clear();

		int currentMode = 0; // 1 = CPP, 2 = H
		bool writeScript = false;
		bool writeScriptImport = false;
		for each (std::string line in lines) {
			if (line.find("//<\\@GENERATED CODE@SCRIPT IMPORT@>//") != std::string::npos) {
				outfile << "#include \"Components\\Scripts\\" << name << ".h\"\n";
				writeScriptImport = true;
			}

			if (line.find("//<\\@GENERATED CODE@SCRIPTS@>//") != std::string::npos) {
				writeScript = true;
				outfile << "	" << name << " script_" << name << ";\n";
				outfile << "	" << "physics.addScript(&script_" << name << ");\n";
			}
			outfile << line << "\n";
		}
		if (!writeScript && !writeScriptImport) {
			std::cout << "AN ERROR HAS OCCURED GENERATING THE NEW SCRIPT!" << std::endl;
		}
	}
	outfile.close();
}

void ScriptManager::deleteApplicationCPP(std::string name) {
	std::string path = "src\\Application.cpp";
	std::fstream infile(path, std::ios::in);
	std::vector<std::string> lines;
	std::string readLine;
	if (infile.good()) {
		while (std::getline(infile, readLine)) {
			lines.push_back(readLine);
		}
	}
	infile.close();
	std::fstream outfile("src\\Application.cpp", std::ios::out);
	if (outfile.good()) {
		outfile.clear();
		for each (std::string line in lines) {
			if (line.find("#include \"Components\\Scripts\\" + name + ".h\"") != std::string::npos) {

			}
			else if (line.find("	" + name + " script_" + name + ";") != std::string::npos) {

			}
			else if (line.find("	physics.addScript(&script_" + name + ")") != std::string::npos) {

			}
			else {
				outfile << line << "\n";
			}

		}
	}
	outfile.close();
}

void ScriptManager::genCPP(std::string name) {
	try {
		std::string path = "src\\Components\\Scripts\\" + name + ".cpp";
		std::fstream file(path, std::ios::out);
		if (file.good()) {
			file << "#include \"" << name.c_str() << ".h\"\n\n"; //						#include "SCRIPT.h"
			file << name.c_str() << "::" << name.c_str() << "() {\n"; //				SCRIPT::SCRIPT() { setName("SCRIPT"); }
			file << "	SetName(\"" << name.c_str() << "\"); //DO NOT EDIT \n}\n\n";
			file << name.c_str() << "::~" << name.c_str() << "() {\n\n}\n\n"; //		SCRIPT::~SCRIPT() { }
			file << "void " << name.c_str() << "::OnStart() {\n\n}\n\n"; //				SCRIPT::OnStart() { }
			file << "void " << name.c_str() << "::OnFixedUpdate(float deltaTime) {\n\n}\n\n"; //			SCRIPT::OnUpdate() { }
		}

		file.close();
	}
	catch (std::exception ex) {
		std::cout << ex.what() << std::endl;
	}
}

void ScriptManager::genH(std::string name) {
	try {
		std::string path = "src\\Components\\Scripts\\" + name + ".h";
		std::fstream file(path, std::ios::out);
		if (file.good()) {
			file << "#pragma once\n"; //													#pragma once
			file << "#include \"ComponentScript.h\"\n\n"; //							#include "ComponentScript.h"
			file << "class " << name.c_str() << " : public ComponentScript {\n"; //		class SCRIPT : public ComponentScript {
			file << "public:\n	"; //													public:
			file << name.c_str() << "();\n	"; //											SCRIPT();
			file << "~" << name.c_str() << "();\n	"; //									~SCRIPT();
			file << "void OnStart() override;\n"; //										OnStart() override;
			file << "	void OnFixedUpdate(float deltaTime) override;\n"; //									OnUpdate() override;					  };
			file << "private:\n\n};";
		}

		file.close();
	}
	catch (std::exception ex) {
		std::cout << ex.what() << std::endl;
	}
}

void ScriptManager::modifyVCXProj(std::string name) {
	std::string path = "Engine.vcxproj"; //FILE NAME PROBABLY SHOULD BE CHANGED...
	std::fstream infile(path, std::ios::in);
	std::vector<std::string> lines;
	std::string readLine;
	if (infile.good()) {
		while (std::getline(infile, readLine)) {
			lines.push_back(readLine);
		}
	}
	infile.close();
	std::fstream outfile("Engine.vcxproj", std::ios::out);
	if (outfile.good()) {
		outfile.clear();

		int currentMode = 0; // 1 = CPP, 2 = H
		for each (std::string line in lines) {
			outfile << line << "\n";
			if (line.find(".cpp\" />") != std::string::npos && currentMode == 0) {
				currentMode = 1;
			}
			if (line.find(".h\" />") != std::string::npos && currentMode == 2) {
				currentMode = 3;
			}
			if (currentMode == 1) {
				outfile << "    <ClCompile Include=\"src\\Components\\Scripts\\" << name.c_str() << ".cpp\" />" << "\n";
				currentMode = 2;
			}
			if (currentMode == 3) {
				outfile << "    <ClInclude Include=\"src\\Components\\Scripts\\" << name.c_str() << ".h\" />" << "\n";
				currentMode = 4;
			}

		}
	}
	outfile.close();
}

void ScriptManager::deleteVCXProj(std::string name) {
	std::string path = "Engine.vcxproj"; //FILE NAME PROBABLY SHOULD BE CHANGED...
	std::fstream infile(path, std::ios::in);
	std::vector<std::string> lines;
	std::string readLine;
	if (infile.good()) {
		while (std::getline(infile, readLine)) {
			lines.push_back(readLine);
		}
	}
	infile.close();
	std::fstream outfile("Engine.vcxproj", std::ios::out);
	if (outfile.good()) {
		outfile.clear();

		int currentMode = 0; // 1 = CPP, 2 = H
		for each (std::string line in lines) {
			if (line.find(name + ".cpp\" />") != std::string::npos) {
				
			} else if (line.find(name + ".h\" />") != std::string::npos) {
				
			}
			else {
				outfile << line << "\n";
			}
		}
	}
	outfile.close();
}

void ScriptManager::modifyVCXProjFilters(std::string name) {
	std::string path = "Engine.vcxproj.filters"; //FILE NAME PROBABLY SHOULD BE CHANGED...
	std::fstream infile(path, std::ios::in);
	std::vector<std::string> lines;
	std::string readLine;
	if (infile.good()) {
		while (std::getline(infile, readLine)) {
			lines.push_back(readLine);
		}
	}
	infile.close();
	std::fstream outfile("Engine.vcxproj.filters", std::ios::out);
	if (outfile.good()) {
		outfile.clear();

		int currentMode = 0; // 1 = CPP, 2 = H
		int countDown = 0;
		for each (std::string line in lines) {
			outfile << line << "\n";
			if (line.find(".cpp\">") != std::string::npos && currentMode == 0) {
				currentMode = 1;
				countDown = 2;
			}
			if (line.find(".h\">") != std::string::npos && currentMode == 2) {
				currentMode = 3;
				countDown = 2;
			}
			if (currentMode == 1 && countDown == 0) {
				outfile << "    <ClCompile Include=\"src\\Components\\Scripts\\" << name.c_str() << ".cpp\">" << "\n";
				outfile << "      <Filter>Source Files</Filter>\n";
				outfile << "	</ClCompile>\n";
				currentMode = 2;
			}
			if (currentMode == 3 && countDown == 0) {
				outfile << "    <ClInclude Include=\"src\\Components\\Scripts\\" << name.c_str() << ".h\">" << "\n";
				outfile << "      <Filter>Header Files</Filter>\n";
				outfile << "    </ClInclude>\n";
				currentMode = 4;
			}
			if (countDown > 0) {
				countDown--;
			}
		}
	}
	outfile.close();
}

void ScriptManager::deleteVCXProjFilters(std::string name) {
	std::string path = "Engine.vcxproj.filters"; //FILE NAME PROBABLY SHOULD BE CHANGED...
	std::fstream infile(path, std::ios::in);
	std::vector<std::string> lines;
	std::string readLine;
	if (infile.good()) {
		while (std::getline(infile, readLine)) {
			lines.push_back(readLine);
		}
	}
	infile.close();
	std::fstream outfile("Engine.vcxproj.filters", std::ios::out);
	if (outfile.good()) {
		outfile.clear();

		int lineCountDown = 0;
		for each (std::string line in lines) {
			if (line.find(name + ".cpp\">") != std::string::npos) {
				lineCountDown = 3;
			}
			else if (line.find(name + ".h\">") != std::string::npos) {
				lineCountDown = 3;
			}
			else {
				if (lineCountDown == 0) {
					outfile << line << "\n";
				}
			}
			if (lineCountDown > 0) {
				lineCountDown--;
			}
		}
	}
	outfile.close();
}