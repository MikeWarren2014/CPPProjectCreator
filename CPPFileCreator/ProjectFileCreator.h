#ifndef PROJECTFILECREATOR_H
#define PROJECTFILECREATOR_H

#include <iostream>	// you might not need this, but it is here "just in case"
#include <fstream>
#include <sstream>	// again, you might not need this, but it is a good idea to #include it
#include <string>

#include "FileTypes.h"

class ProjectFileCreator
{
	public:
		// constructors
		ProjectFileCreator();
		ProjectFileCreator(const char*);
		ProjectFileCreator(const std::string&);
		ProjectFileCreator(const std::string*);
		// functions for creating the files
		void createMain();
		void createClass(const std::string&, bool);
		void createEnum(const std::string&);
		// useful getter/setter functions
		std::string getActiveDirectory(void) const;
		void setActiveDirectory(const std::string&);
		// for prompting the user if they want to change this->activeDirectory
		void askForNewDirectory();
	private:
		std::ofstream fileWriter;
		// this may be extra, but it might be a good idea to have
		std::string activeDirectory;
		// helper functions
		void createHeaderFile(const std::string&, FileTypes);
		void createCPPFile(const std::string&, FileTypes);
};

#endif
