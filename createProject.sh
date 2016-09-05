clear
version="0.0.2"
echo "Project creator"
echo "---------------"
# ask user to specify a directory (could start with spelled-out home directory)
directoryName=""
# Heck, you should probably prevent the user from creating the project in the same directory as rootDir...
# TODO: implement the following requirements:
# 1.) No spaces at all in the directory path
# 2.) some characters must be entered
while [[ ${#directoryName} -eq 0 || $directoryName == *" "* ]] 
	do
		echo -e "\nEnter the full directory of the project (no spaces):"
		read directoryName
	done

# evaluate directoryName so that the rest of the code can understand it
directoryName="${directoryName//\$HOME/$HOME}"
# output directoryName
echo "You entered: $directoryName"
directoryExists=false
# attempt to find directory
find "$directoryName"
if [ $? -eq 0]
then
	directoryExists=true
else
	# create directory if it does not exist
	mkdir -p "$directoryName"
	if [ $? -eq 0 ]; then directoryExists=true
fi
# if directory exists 
if [ $directoryExists ]
	# call executable in /home/miwarren/CPPFileCreator to create a main.cpp file to put in the folder
	$HOME/CPPFileCreator/a.out "$directoryName"
	# copy a run.sh file into the folder, too...
	cp $HOME/CPPFileCreator/run.sh "$directoryName"/run.sh
	# you might want to put a fileCreator.sh in the folder as well
	cp $HOME/fileCreator.sh "$directoryName"/fileCreator.sh
fi
