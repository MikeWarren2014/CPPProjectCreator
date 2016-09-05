#include "ProjectFileCreator.h"
#include "FileTypes.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

// constructors defined here
/* NOTE: this->fileWriter is already constructed; don't try to construct it again */
ProjectFileCreator::ProjectFileCreator()
{

}

ProjectFileCreator::ProjectFileCreator(const char* nameOfActiveDirectory)
{
	// set this->activeDirectory
	this->activeDirectory = std::string(nameOfActiveDirectory);
}

ProjectFileCreator::ProjectFileCreator(const std::string& nameOfActiveDirectory)
{
	// same as the previous constructor
	this->activeDirectory = nameOfActiveDirectory;
}

ProjectFileCreator::ProjectFileCreator(const std::string* nameOfActiveDirectory)
{
	// this is here to make the compiler shut up
	this->activeDirectory = *nameOfActiveDirectory;
}

// define helper functions here
void ProjectFileCreator::createHeaderFile(const std::string& nameOfFile, FileTypes fileType)
{
	// make sure that the only way a '.' is present in nameOfFile is if there is a 'h' after it
	unsigned periodLocation = nameOfFile.find_first_of('.');
	if ((periodLocation != -1) && (periodLocation != nameOfFile.length()-2) || ((periodLocation == nameOfFile.length()-2) && (nameOfFile[periodLocation+1] != 'h')))
		throw std::invalid_argument("Invalid filename format. Please try again...");
	// making sure that nameOfFile is not a full pathname
	if (nameOfFile.find_first_of("/\\") != -1)
		throw std::invalid_argument("Please remove the pathname and try again.");	
	// make sure fileType is NOT MAIN_FILE
	if (fileType == MAIN_FILE)
		throw std::invalid_argument("Requested header file for main file");
	// get the nameOfStructure from nameOfFile (discard the ".h" if it is present)
	std::string nameOfStructure(nameOfFile);
	if (periodLocation != -1)
	{
		nameOfStructure.erase(periodLocation);
	}		
	// initialize theMacro as nameOFFile
	std::string theMacro(nameOfFile);
	// if theMacro is not greater than than 2 characters long, or if there is not a ".h" on the end of theMacro, append one
	if ((nameOfFile.length() <= 2) || (nameOfFile.substr(nameOfFile.length()-2, 2) != ".h"))
		theMacro += ".h";
	// open file specified by nameOfFile
	std::string fullFileName = this->activeDirectory + "/" + theMacro;
	this->fileWriter.open(fullFileName.c_str());
	// finalize theMacro by capitalizing every letter and changing the '.' to '_'
	for (unsigned i = 0; i < theMacro.length(); i++)
	{
		if ((theMacro[i] >= 'a') && (theMacro[i] <= 'z'))
			theMacro[i] -= 32;
		else if (theMacro[i] == '.')
			theMacro[i] = '_';
	}
	// write the first two lines of the include guard
	this->fileWriter << "#ifndef " << theMacro << "\n#define " << theMacro << "\n\n";
	// write some "body code" to the file
	this->fileWriter << ((fileType == CLASS) ? "class " : "enum ") << nameOfStructure << "\n{\n\t";
	if (fileType == CLASS)
	{
		this->fileWriter << "public:\n\t\t" << nameOfStructure << "();\n\n\tprivate:\n\t\t\n";
	}
	else
	{
		this->fileWriter << std::endl;
	}
	this->fileWriter << "};\n\n";
	// finalize the include guard and close the file
	this->fileWriter << "#endif";
	this->fileWriter.close();
}

void ProjectFileCreator::createCPPFile(const std::string& nameOfFile, FileTypes fileType)
{
	// make sure that fileType is not ENUM_FILE
	if (fileType == ENUM_FILE)
		throw std::invalid_argument("Requested a .cpp file for an enum.");
	// make sure that the only text following any '.' is "cpp" (if there is a '.' at all in nameOfFile)
	unsigned pos = nameOfFile.find_first_of('.');
	if ((pos != -1) && (pos != nameOfFile.length()-4) || ((pos == nameOfFile.length()-4) && (nameOfFile.substr(pos+1,3) != "cpp")))
		throw std::invalid_argument("Invalid file extension. Please try again.");
	// making sure that nameOfFile is not a pathname
	if (nameOfFile.find_first_of("/\\") != -1)
		throw std::invalid_argument("Please remove the pathname and try again.");
	// create and open a file in this->activeDirectory, named by nameOfFile (append ".cpp" if it is not there already)
	std::string fullFileName = this->activeDirectory + "/" + nameOfFile;
	if (pos == -1)
		fullFileName += ".cpp";
	this->fileWriter.open(fullFileName.c_str());
	// write "#include " to the file
	this->fileWriter << "#include ";
	// if nameOfFile is either "main.cpp" or "main"
	if ((nameOfFile == "main.cpp") || (nameOfFile == "main"))
		// write "<iostream>\n\nusing namespace std;\n\nint main()\n{\n\t\n\treturn 0;\n}" to the file
		this->fileWriter << "<iostream>\n\nusing namespace std;\n\nint main()\n{\n\t\n\treturn 0;\n}";
	// otherwise, we are trying to initialize the content of the definition file for a class
	else
	{
		// get the className
		std::string className = nameOfFile.substr(0, pos);
		// write "\"" +  className + ".h\"\n\n" + className + "::" + className + "()\n{\n\t\n}\n" to the file
		this->fileWriter << "\"" << className << ".h\"\n\n" << className << "::" << className << "()\n{\n\t\n}\n";
	}
	// close the file
	this->fileWriter.close();
}

// the public functions that will be using the helper functions
void ProjectFileCreator::createMain()
{
	// this is a convenience function for createCPPFile("main", MAIN_FILE)
	this->createCPPFile("main", MAIN_FILE);
}

void ProjectFileCreator::createClass(const std::string& className, bool isTemplated)
{
	// create both the .h and the .cpp file
	this->createHeaderFile(className, CLASS);
	this->createCPPFile(className, CLASS);
}

void ProjectFileCreator::createEnum(const std::string& enumName)
{
	// create a header file only (you might want to check and see if the header file already exists first)
	if (std::ifstream((this->activeDirectory + "/" + enumName + ".h").c_str()).is_open())
	{
		std::cout << "This file already exists." << std::endl;
		return;
	}
	this->createHeaderFile(enumName, ENUM_FILE);
}

// getters and setters
std::string ProjectFileCreator::getActiveDirectory(void) const { return this->activeDirectory; }
void ProjectFileCreator::setActiveDirectory(const std::string& newDirectory) { this->activeDirectory = newDirectory; }

// prompting the user for change of activeDirectory
void ProjectFileCreator::askForNewDirectory()
{
	bool keepAsking = true;
	std::string userInput;	
	while (keepAsking)
	{
		userInput = "";
		// prompt the user for the new activeDirectory
		std::cout << "Enter the new activeDirectory (don't forget to prefix it with \"/home/\" + yourUserName ): ";
		std::getline(std::cin, userInput);
		// make sure that there are no unwanted characters in userInput
		keepAsking = (userInput.find_first_of(".") != -1);
		if (keepAsking)
			std::cout << "Invalid directory specified. Please try again...\n";
	}
	// set this->activeDirectory to userInput
	this->activeDirectory = userInput;
}
