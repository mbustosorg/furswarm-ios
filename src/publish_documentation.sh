#! /usr/bin/env bash

echo '----Update Documentation----'
/Applications/Development/Doxygen.app/Contents/Resources/doxygen /Users/m/Documents/development/furSwarmDocumentation/Doxyfile
echo '----Clear Previous Deployment----'
rm -rf /Volumes/WebServer/Documents/projectDocumentation/furSwarmDocumentation/
echo '----Publish----'
cp -R /Users/m/Documents/development/furSwarmDocumentation /Volumes/WebServer/Documents/projectDocumentation/
