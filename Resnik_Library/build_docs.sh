#! /bin/sh

doxygen doxygen_Resnik.cfg
moxygen -l cpp -o README.md xml/
rm -rf xml
