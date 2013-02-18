#!/usr/bin/env bash

mkdir -p ~/bin/TestsDatabase
cp -r bin/* ~/bin/TestsDatabase/

echo
echo "#"
echo "# The library libcef.so needs to be installed in /usr/lib"
echo "#"
echo
sudo mv ~/bin/TestsDatabase/libcef.so /usr/lib/libcef.so

cp TestsDatabase.desktop ~/.local/share/applications/TestsDatabase.desktop

echo "Exec=`echo ~`/bin/TestsDatabase/TestsDatabase %F" >> ~/.local/share/applications/TestsDatabase.desktop
echo "Icon=`echo ~`/bin/TestsDatabase/html/img/icon.png" >> ~/.local/share/applications/TestsDatabase.desktop

xdg-mime install acristoffers-x-qdb-mime.xml
xdg-mime default TestsDatabase.desktop application/x-qdb
xdg-icon-resource install --context mimetypes --size 254 ~/bin/TestsDatabase/html/img/icon.png application-x-qdb
update-mime-database ~/.local/share/mime
update-desktop-database ~/.local/share/applications

echo
echo "#"
echo "# Don't Panic! It's ok if you see some errors above."
echo "# As long as it's not an error with TestsDatabase.desktop, then everything went well."
echo "# The errors are from other apps."
echo "#"
echo
