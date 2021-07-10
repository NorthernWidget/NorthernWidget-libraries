#! /bin/sh

doxygen doxygen_Okapi.cfg
moxygen -l cpp -o README.md xml/
rm -r xml
