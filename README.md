# CPPProjectCreator
For when you have to do your C++ programming from the BASH environment in Linux
### Installation instructions
The code should be downloaded to the `$HOME` directory of your Linux machine.
### Creating a project
The main script is called `createProject.sh`, and can be called from anywhere with `sh $HOME/createProject.sh`. When running the script, you will be asked for the directory that will house the C++ project. It is best to give an absolute path. The path can have `$HOME` in it. However, if you give relative path, it will be relative to the directory from which you called the script. For example, if you had directory structure like :
* Home
  * otherFolder
    * Project1
  * CPPFileCreator
and you, from `Home/otherFolder` called the script, any directory you specify will be subdirectory of `otherFolder` if you don't use absolute pathname.

The script then creates the directory, and any parent directories, if they don't exist, and calls a program that creates files for the project. *`main.cpp` is created by default.* You may specify any number of classes, enumerations, or main.cpp files you like. 

Once complete, the script copies a script to call the file creator to the project directory, as well as a script for compiling and running your project. Those scripts are called `fileCreator.sh`,`run.sh`, respectively.
### Creating files
To create files for your project, go to the project directory and type `sh fileCreator.sh`. You should see the following menu: 

![File creator menu](/image.png).

Simply enter the numbers corresponding to the things you wish to create and the appropriate files will be generated.

1. This option generates a .cpp file and a .h file for the class name you specify, complete with the include guards.
2. This option generates a .h file for the enum, which also has include guards
3. Self-explanatory. If a main.cpp file already exists there, this option does nothing.
4. This feature doesn't exist right now.
5. Sets the directory to which the files are written
6. Exits
