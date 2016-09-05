#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept> // this is for throwing std::invalid_argument
#include <typeinfo>

#include "ProjectFileCreator.h"

using namespace std;

int main(int argumentCount, char** arguments)
{
	cout << "\f";
	// for test purposes, just output the list of arguments that this program received
	for (unsigned j = 0; j < argumentCount; j++)
		cout << "arguments[" << j << "] == " << arguments[j] << endl;
	// make sure that argumentCount == 2
	if (argumentCount == 2)
	{
		ifstream fileChecker;
		// initialize the ProjectFileCreator with the directory specified by arguments[1]
		cout << string(arguments[1]) << " is " << ((typeid(string(arguments[1])).name() == typeid(string).name()) ? "" : "not ") << "a string."<< endl;
		ProjectFileCreator fileMaker(arguments[1]);
		// try to open the main.cpp at the directory specified at arguments[1]
		string pathnameOfMain = fileMaker.getActiveDirectory();
		pathnameOfMain += "/main.cpp";
		fileChecker.open(pathnameOfMain.c_str());
		// if main.cpp is not found there
		if (!fileChecker.is_open())	
			// create a main.cpp file there
			fileMaker.createMain();
		// otherwise, close the file
		else 
			fileChecker.close();
		// set up a prompt for the user to create classes, enums, a main.cpp file, and/or another standalone .cpp file
		/* 1.) creates a class
		 * 2.) creates an enum
		 * 3.) creates a main.cpp file
		 * 4.) creates a standalone .cpp file (this option won't exist by default; it is a work in progress)
		 * 5.) change the activeDirectory
		 * 6.) exit*/
		bool userIsDone = false;
		string rawInput;
		while (!userIsDone)
		{
			int choice = 0;
			while (choice == 0)
			{
				rawInput = "";
				cout << "\fPlease enter one of the following options:\n";
				cout << "1 - create a class" << endl;
				cout << "2 - create an enum" << endl;
				cout << "3 - create main.cpp file" << endl;
				cout << "4 - create a general .cpp file (this option doesn't work right now)" << endl;
				cout << "5 - change the activeDirectory" << endl;
				cout << "6 - exit\n" << endl;
				cout << "> ";
				getline(cin, rawInput);
				stringstream(rawInput) >> choice;
			}	
			cout << "You entered :" << choice << endl;
			rawInput = "";
			// here, you should clear out any garbage characters
			switch (choice)
			{
				// if the user wants to create a class 
				case 1:	
					// prompt the user for the className (tell them to just name the class, and to put it in camelCase)
					while ((rawInput.length() == 0) || (rawInput.find_first_of("/\\{}[]*&;,\'\"+-^%") != -1))
					{
						cout << "Please enter the name of the class you want to create (preferrably camel-cased): ";
						getline(cin, rawInput);
						// ask them if they want the class to be templated
					}
					// create the .cpp file and the .h file for that class 
					cout << "Creating the .cpp and .h files for that class...\n";
					try
					{
						fileMaker.createClass(rawInput, false);
						cout << "Files successfully created!\n";
					}
					catch (exception& mistake)
					{
						cout << "The following error occurred when trying to create the class files: " << mistake.what() << endl;
					}
					break;
				// if the user wants to create an enum
				case 2:
					{
						// prompt the user for the enumName (same prompt as with the class)
						while ((rawInput.length() == 0) || (rawInput.find_first_of("/\\{}[]*&;,\'\"+-^%") != -1))
						{
							cout << "Please enter the name of the enum you want to create (preferrably camel-cased): ";
							getline(cin, rawInput);
						}	
						// check for a header file that already exists there
						string pathnameOfEnumFile = fileMaker.getActiveDirectory() + "/" + rawInput;
						fileChecker.open(pathnameOfEnumFile.c_str());
						// if one already exists
						if (fileChecker.is_open())
						{
							// simply tell the user that a .h file already exists (and that the .h file was not created by this session)
							cout << "There is already a .h file in the activeDirectory with the name of the enum, so I cannot create one.\n";
							cout << "Sorry!!\n";
							fileChecker.close();
						}
						// otherwise
						else
						{
							// create a .h file for the enum specified by the user
							cout << "Creating enum...\n";
							try
							{
								fileMaker.createEnum(rawInput);
								cout << "Enum successfully created!\n";
							}
							catch (exception& mistake)
							{
								cout << "The following error occurred when trying to create the enum file: " << mistake.what() << endl;
							}
						}
						break;
					}
				// if the user wants to create a main.cpp file
				case 3:
					// check for main.cpp file in the activeDirectory
					rawInput = fileMaker.getActiveDirectory() + "/main.cpp";
					fileChecker.open(rawInput.c_str());
					// if one already exists
					if (fileChecker.is_open())
					{
						// do not create one, and tell the user that it already exists there
						cout << "You know that there is already a main.cpp in this directory!? I cannot make another one...\n";
						fileChecker.close();
					}
					// otherwise
					else
					{
						// create a main.cpp file for the user				
						cout << "Creating main.cpp in activeDirectory...\n";
						try
						{
							fileMaker.createMain();
							cout << "main.cpp successfully created in activeDirectory!\n";
						}
						catch (exception& mistake)
						{
							cout << "The following error occurred when trying to make the main.cpp file : " << mistake.what() << endl;
						}	
					}
					break;
				// if the user wants to create a standalone .cpp file (this is a future option)
				case 4:
					// ask them for the fileName (and tell them to hold the ".cpp")
					// check for the file's existance
					// if the file already exists
						// do not re-create it, and tell the user that it already exists
					// otherwise
						// create the file
					break;
				// if the user wishes to change the activeDirectory
				case 5:
					// ask them for the name of the new activeDirectory (make sure to warn them of exceptions if the directory name is invalid)
					while ((rawInput.length() == 0) || (rawInput.find_first_of(".,") != -1))
					{	
						cout << "Enter the name of the new activeDirectory (directory must already exist)\n";
						cout << "> ";
						getline(cin, rawInput);
					}
					// change the activeDirectory of fileMaker to what the user specified
					fileMaker.setActiveDirectory(rawInput);
					break;
				// if the user is done
				case 6:
					// userIsDone
					userIsDone = true;
					break;
				default: 
					break;
			}
		}
	}
	return 0;
}
