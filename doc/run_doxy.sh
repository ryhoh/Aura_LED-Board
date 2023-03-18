#!/bin/sh

cp Doxyfile Doxyfile.bak
cd ../
echo `pwd` >> doc/Doxyfile
cd doc
doxygen
rm Doxyfile
mv Doxyfile.bak Doxyfile
