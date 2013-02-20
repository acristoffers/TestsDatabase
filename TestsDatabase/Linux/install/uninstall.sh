#!/usr/bin/env bash

rm -r ~/.TestsDatabase
rm ~/.local/share/applications/TestsDatabase.desktop
rm "`xdg-user-dir DESKTOP`/TestsDatabase.desktop"
sudo rm /usr/lib/libcef.so

