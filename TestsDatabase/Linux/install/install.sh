#!/usr/bin/env bash

mkdir -p ~/.TestsDatabase
cp -r bin/* ~/.TestsDatabase/

echo
echo "#"
echo "# The library libcef.so needs to be installed in /usr/lib"
echo "#"
echo
sudo mv ~/.TestsDatabase/libcef.so /usr/lib/libcef.so

mkdir -p ~/.local/share/applications
cp TestsDatabase.desktop ~/.local/share/applications/TestsDatabase.desktop

echo "Exec=`echo ~`/.TestsDatabase/TestsDatabase %f" >> ~/.local/share/applications/TestsDatabase.desktop
echo "Icon=`echo ~`/.TestsDatabase/html/img/icon.png" >> ~/.local/share/applications/TestsDatabase.desktop

chmod u+x ~/.local/share/applications/TestsDatabase.desktop
cp ~/.local/share/applications/TestsDatabase.desktop `xdg-user-dir DESKTOP`

xdg-mime install acristoffers-x-qdb-mime.xml
xdg-mime default TestsDatabase.desktop application/x-qdb
xdg-icon-resource install --context mimetypes --size 254 ~/.TestsDatabase/html/img/icon.png application-x-qdb
update-mime-database ~/.local/share/mime
update-desktop-database ~/.local/share/applications

echo
echo "#"
echo "# Don't Panic! It's ok if you see some errors above."
echo "# As long as it's not an error with TestsDatabase.desktop, then everything went well."
echo "# The errors are from other apps."
echo "#"
echo
