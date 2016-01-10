#! /usr/bin/env bash

echo '----Source Code Management----'
IOS_SRC=~/Documents/development/openFrameworks/of_preRelease_v007_iphone/apps/iPhoneExamples/furSwarmiOS/src
OSX_SRC=~/Documents/development/openFrameworks/of_preRelease_v007_osx/apps/applications/furSwarm/src

echo '----Update furSwarmPatternConst.h distribution----'
cp ~/Documents/development/openFrameworks/of_preRelease_v007_iphone/apps/iPhoneExamples/furSwarmiOS/src/furSwarmPatternConst.h /Users/m/Documents/development/furSwarmMember

echo '----IOS Listing----'
ls $IOS_SRC
echo '----OSX Listing----'
ls $OSX_SRC

echo '----Refresh OSX from IOS----'
FILES=*.mm
for f in $FILES
do
	echo '----Rename *.mm -> *.cpp----'
	replF=${f/.mm/.cpp}
	cp -v $IOS_SRC/$f $OSX_SRC/$replF
done
FILES=*.cpp
for f in $FILES
do
	cp -v $IOS_SRC/$f $OSX_SRC/$f
done
FILES=*.h
for f in $FILES
do
	cp -v $IOS_SRC/$f $OSX_SRC/$f
done

echo '----SVN Maintenance----'
cd $OSX_SRC
pwd
svn update
svn list -v

echo '----Committing Changes----'
FILES=*.cpp
for f in $FILES
do
	svn commit $f -m 'Auto-sync commit' --password mauricio
done
FILES=*.h
for f in $FILES
do
	svn commit $f -m 'Auto-sync commit' --password mauricio
done
FILES=*.mm
for f in $FILES
do
	svn commit $f -m 'Auto-sync commit' --password mauricio
done

echo '----Archive Offsite----'
echo '----Clear Previous Deployment----'
rm -rf /Users/mauricio/Dropbox/apps/furSwarmController/
echo '----Publish----'
cp -R ../ /Users/mauricio/Dropbox/apps/furSwarmController/

