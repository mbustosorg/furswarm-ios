#!/bin/sh
cd ~/Documents/development/openFrameworks/of_v0.9.0_ios_release/apps/myApps/furSwarmIOS\ copy
xcodebuild clean -project furSwarmIOS.xcodeproj -configuration Release -alltargets
xcodebuild archive -project furSwarmIOS.xcodeproj -scheme furSwarmIOS -archivePath ~/Downloads/furSwarmIOS.xcarchive
rm /Users/mauricio/Music/iTunes/iTunes\ Media/Mobile\ Applications/furSwarmIOS.ipa
xcodebuild -exportArchive -archivePath ~/Downloads/furSwarmIOS.xcarchive -exportPath /Users/mauricio/Music/iTunes/iTunes\ Media/Mobile\ Applications/furSwarmIOS.ipa -exportFormat ipa -exportProvisioningProfile furSwarmAdHoc

