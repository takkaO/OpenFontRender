#!/bin/bash

# -------------------------------------------------------
# CopyLibs.sh
# 
# Copyright (c) 2021 takkaO
#
# If you use, modify, or redistribute this file independently　
# of the original repository, this program is licensed under 
# the MIT License.
# 
# If you use, modify or redistribute this file as part of 
# the original repository, please follow the repository's license.
# 
# -------------------------------------------------------

RED='\033[0;31m'
NC='\033[0m' # No Color

for eachValue in `ls -d */ | grep "^freetype.*" | cut -f1 -d'/'` ; do
    FreeTypeDir=${eachValue}
	break
done

if [ " $FreeTypeDir" = " " ] ; then
	echo "${RED}Not Found FreeType project directory.${NC}"
	exit
fi

echo "Delete a FreeType libraries in OpenFontRender that already exists."
read -p "Are you sure you want to proceed ? (y/N): " yn
if [ "$yn" != "y" ] && [ "$yn" != "Y" ]; then
  echo "The process has been suspended by user."
  exit
fi

rm -r -f "./src/freetype"
rm -r -f "./src/base"
rm -r -f "./src/cache"
rm -r -f "./src/sfnt"
rm -r -f "./src/smooth"
rm -r -f "./src/truetype"
rm -r -f "./src/ft2build.h"

echo "** Start Copying the necessary files **"

childDir="freetype"
targetPath="${FreeTypeDir}/include/${childDir}"
echo "Copying [${targetPath}]"
if [ ! -d $targetPath ]; then
	echo "${RED}Not Found ${targetPath}${NC}"
	exit
fi
cp -r $targetPath "./src"

childDir="base"
targetPath="${FreeTypeDir}/src/${childDir}"
echo "Copying [${targetPath}]"
if [ ! -d $targetPath ]; then
	echo "${RED}Not Found ${targetPath}${NC}"
	exit
fi
cp -r $targetPath "./src"

childDir="cache"
targetPath="${FreeTypeDir}/src/${childDir}"
echo "Copying [${targetPath}]"
if [ ! -d $targetPath ]; then
	echo "${RED}Not Found ${targetPath}${NC}"
	exit
fi
cp -r $targetPath "./src"

childDir="sfnt"
targetPath="${FreeTypeDir}/src/${childDir}"
echo "Copying [${targetPath}]"
if [ ! -d $targetPath ]; then
	echo "${RED}Not Found ${targetPath}${NC}"
	exit
fi
cp -r $targetPath "./src"

childDir="smooth"
targetPath="${FreeTypeDir}/src/${childDir}"
echo "Copying [${targetPath}]"
if [ ! -d $targetPath ]; then
	echo "${RED}Not Found ${targetPath}${NC}"
	exit
fi
cp -r $targetPath "./src"

childDir="truetype"
targetPath="${FreeTypeDir}/src/${childDir}"
echo "Copying [${targetPath}]"
if [ ! -d $targetPath ]; then
	echo "${RED}Not Found ${targetPath}${NC}"
	exit
fi
cp -r $targetPath "./src"

childFile="ft2build.h"
targetPath="${FreeTypeDir}/include/${childFile}"
echo "Copying [${targetPath}]"
if [ ! -e $targetPath ]; then
	echo "${RED}Not Found ${targetPath}${NC}"
	exit
fi
cp -r $targetPath "./src"

echo "** Success to all progress **"