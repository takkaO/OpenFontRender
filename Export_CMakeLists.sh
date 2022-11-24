#!/bin/bash

# -------------------------------------------------------
# Export_CMakeLists.sh
# 
# Copyright (c) 2022 takkaO
#
# If you use, modify, or redistribute this file independently　
# of the original repository, this program is licensed under 
# the MIT License.
# 
# If you use, modify or redistribute this file as part of 
# the original repository, please follow the repository's license.
# 
# -------------------------------------------------------

echo "** Start create CMakeLists.txt **"

## Clean CMakeLists.txt
echo "Processing clear old CMakeLists.txt"
echo -e "# This file was automatically created by \"Export_CMakeLists\" script.\n" > CMakeLists.txt

## Search source files
echo "Processing search C/C++ source files"
lst=$(find "./src" -type f -name "*.c" -or -name "*.cpp")

echo "set(srcs " >> CMakeLists.txt
for f in $lst; do
    # Space for indent
    echo "    "\"$f\" >> CMakeLists.txt
done
echo ")" >> CMakeLists.txt

## Search include directory
echo "Processing search include directories"
lst=$(find "./src" -type d)

echo "idf_component_register(SRCS \${srcs}" >> CMakeLists.txt
echo "                       INCLUDE_DIRS " >> CMakeLists.txt
for f in $lst; do
    # Space for indent
    echo "                       "\"$f\" >> CMakeLists.txt
done
echo ")" >> CMakeLists.txt

echo "** Finish create CMakeLists.txt **"