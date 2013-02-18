#!/usr/bin/env bash

mkdir -p build
touch build/touched # for silencing the next line, in case the folder was just created
rm -r build/*
cd build

echo "Building BZIP"
gcc -c ../TestsDatabase/Independent/bzip/*.c

echo "Building compressor"
g++ ../TestsDatabase/Independent/Compressor/main.cpp \
	*.o \
	-o compressor \
	-I ../TestsDatabase/Independent \
	-lpthread -ldl

echo "Building SQLite 3"
gcc -c ../TestsDatabase/Independent/sqlite/sqlite3.c

echo "Building MD5"
g++ -c ../TestsDatabase/Independent/MD5/md5.cpp

echo "Building TestsDatabase::DataBase"
g++ -c ../TestsDatabase/Independent/DataBase/*.cpp \
	-I ../TestsDatabase/Independent \
	-I ../TestsDatabase/Independent/DataBase \
	-I ../TestsDatabase/Independent/sqlite \
	-I ../TestsDatabase/CEF/Linux \
	-I ../TestsDatabase/CEF/Linux/include \
	`pkg-config --cflags --libs gtk+-2.0`

echo "Building TestsDatabase::Independent"
g++ -c ../TestsDatabase/Independent/*.cpp \
	-I ../TestsDatabase/Independent \
	-I ../TestsDatabase/Independent/DataBase \
	-I ../TestsDatabase/Independent/sqlite \
	-I ../TestsDatabase/Independent/MD5 \
	-I ../TestsDatabase/CEF/Linux \
	-I ../TestsDatabase/CEF/Linux/include \
	`pkg-config --cflags --libs gtk+-2.0`

echo "Building TestsDatabase::Linux"
g++ -c ../TestsDatabase/Linux/*.cpp \
	-I ../TestsDatabase/Independent \
	-I ../TestsDatabase/Independent/DataBase \
	-I ../TestsDatabase/Independent/sqlite \
	-I ../TestsDatabase/Independent/MD5 \
	-I ../TestsDatabase/CEF/Linux \
	-I ../TestsDatabase/CEF/Linux/include \
	`pkg-config --cflags --libs gtk+-2.0`

echo "Linking..."
gcc *.o \
	../TestsDatabase/CEF/Linux/lib/libcef_dll_wrapper.a \
	-o TestsDatabase \
	`pkg-config --cflags --libs gtk+-2.0` \
	-L ../TestsDatabase/CEF/Linux/lib \
	-lcef \
	-Wl,-R. -Wl,-R/usr/lib

echo "Copying files around..."
rm *.o
mkdir bin
mv TestsDatabase bin/
cp -r ../TestsDatabase/CEF/Linux/resources/* bin/
cp -r ../TestsDatabase/CEF/Linux/lib/libcef.so bin/libcef.so
cp -r ../TestsDatabase/Independent/html bin/
rm bin/html/compressor
rm bin/html/build.rb
mv compressor bin/html/compressor
cp ../TestsDatabase/Linux/build.rb bin/html/build.rb
cd bin/html

echo "'Building' HTML/JS/CSS"
ruby build.rb
cd ../..

echo "Creating install 'package'"
cp ../TestsDatabase/Linux/install/* .
cd ..
mv build TestsDatabase-1.0
tar -cf TestsDatabase-1.0.tar TestsDatabase-1.0
xz -e TestsDatabase-1.0.tar
rm -r TestsDatabase-1.0

