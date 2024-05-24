#! /bin/sh

doxygen doxygen_NW.cfg
moxygen -l cpp -o README.md xml/
rm -rf xml
