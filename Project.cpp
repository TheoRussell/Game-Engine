#include "Project.h"



Project::Project()
{
}


Project::~Project()
{
}


void Project::generate(std::string name) {
	//Generating a start project.
	std::string directory = "src\\Projects\\" + name + "\\";
	CreateDirectory(&directory[0], NULL);
	directory = "src\\Projects\\" + name + "\\Scenes\\";
	CreateDirectory(&directory[0], NULL);
	directory = "src\\Projects\\" + name + "\\Actors\\";
	CreateDirectory(&directory[0], NULL);
	directory = "src\\Projects\\" + name + "\\Interfaces\\";
	CreateDirectory(&directory[0], NULL);
	save("src\\Projects\\" + name + "\\Project");

	directory = "src\\Projects\\" + name + "\\";

	setActive();
}


void Project::load(std::string projVersion) {
	//ProjVersion already includes the file path of the project.
	std::ifstream projFile(directory + projVersion, std::ifstream::binary);

	name = BinaryFiles::getString(projFile);
	startScene = BinaryFiles::getString(projFile);

	projFile.close();

	setActive();
}

void Project::save() {
	save(directory + fileName);
}

void Project::save(std::string projVersion) {
	//Error check so that file path is correct.
	if (projVersion.find(".proj") == std::string::npos) {
		projVersion.append(".proj");
	}

	std::ofstream projFile(projVersion, std::ofstream::binary);

	BinaryFiles::writeString(projFile, name);
	BinaryFiles::writeString(projFile, startScene);

	projFile.close();
}


std::string Project::getActive() {
	std::string SourceDir = "src\\Projects\\";
	std::string fileName;
	std::vector<std::string> files;
	//Finding all the project folders
	for (auto & p : std::experimental::filesystem::directory_iterator(SourceDir)) {
		fileName = p.path().string();
		files.push_back(fileName);
	}
	//Finding all the session locks.
	for (std::string folder : files) {
		for (auto & p : std::experimental::filesystem::directory_iterator(folder)) {
			fileName = p.path().string();
			if (fileName.find("session.lock") != std::string::npos) {
				fileName.erase(fileName.end() - 12, fileName.end());
				return fileName;
				break;
			}
		}
	}
	return "";
}

void Project::setActive() {
	//Deleting the session lock from the other projects.
	///using src\\Projects\\
	//
	std::string SourceDir = "src\\Projects\\";
	std::string fileName;
	std::vector<std::string> files;
	//Finding all the project folders
	for (auto & p : std::experimental::filesystem::directory_iterator(SourceDir)) {
		fileName = p.path().string();
		files.push_back(fileName);
	}
	//Finding all the session locks.
	for (std::string folder : files) {
		for (auto & p : std::experimental::filesystem::directory_iterator(folder)) {
			fileName = p.path().string();
			if (fileName.find("session.lock") != std::string::npos) {
				if (remove(fileName.c_str()) != 0) {
					std::cout << "Unable to delete session lock file! Please check permissions." << std::endl;
					std::cout << fileName << std::endl;
				}
				break;
			}
		}
	}


	//This is so that the engine knows which project to run when it's in PRODUCT mode.
	//The file is essential as without it, nothing will be loaded and a black game will be ran.
	std::ofstream projFile(directory + "session.lock", std::ofstream::binary);
	bool t = true;
	BinaryFiles::writeBool(projFile, t);
	projFile.close();
}