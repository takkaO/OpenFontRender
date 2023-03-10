#!/bin/sh
cd /doxygen
#rm -r -f /doxygen/documents/html
#rm -r -f /doxygen/documents/latex
(
cat /Doxyfile
for x in "$@"
do
 echo $x
done
) | doxygen -
