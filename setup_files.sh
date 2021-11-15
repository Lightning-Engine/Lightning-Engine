PREMAKE="./Vendor/premake/bin/release/premake5"
if [[ -f "$PREMAKE" ]]
then
	echo "Premake already exists"
else
	if [ "$1" == "osx" ]
	then
		make -f Bootstrap.mak osx
	fi
	if [ "$1" == "linux" ]
	then
		make -f Bootstrap.mak linux
	fi
	if [ "$1" == "win" ]
	then
		nmake -f Bootstrap.mak windows
	fi
fi	

if [ "$1" == "osx" ]
	then
	  "$PREMAKE" gmake2
	fi
	if [ "$1" == "linux" ]
	then
		"$PREMAKE" gmake2
	fi
	if [ "$1" == "win" ]
	then
		"$PREMAKE" vs2022
	fi
