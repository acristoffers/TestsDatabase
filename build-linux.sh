#!/usr/bin/env bash

mkdir -p build
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
	-Wl,-R.

echo "Copying files around..."
rm *.o
cp -r ../TestsDatabase/CEF/Linux/resources/* .
cp -r ../TestsDatabase/CEF/Linux/lib/libcef.so libcef.so
cp -r ../TestsDatabase/Independent/html .
rm html/compressor
rm html/build.rb
mv compressor html/
cp ../TestsDatabase/Linux/build.rb html/build.rb
cd html

echo "'Building' HTML/JS/CSS"
ruby build.rb
cd ..

echo "Reducing sizes... (may take quite a while)"
#upx --ultra-brute libcef.so TestsDatabase

