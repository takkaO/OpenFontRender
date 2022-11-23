#!/bin/bash

echo "Execute CopyLibs.sh"
echo ""
./CopyLibs.sh

echo ""
echo "Execute ModifyLibs.sh"
echo ""
./ModifyLibs.sh

echo ""
echo "Execute Export_CMakeLists.sh"
echo ""
./Export_CMakeLists.sh