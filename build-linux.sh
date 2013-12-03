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
        -I ../TestsDatabase/Independent/bzip \
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
g++ *.o \
	../TestsDatabase/CEF/Linux/lib/libcef_dll_wrapper.a \
	-o TestsDatabase \
	`pkg-config --cflags --libs gtk+-2.0` \
	-L ../TestsDatabase/CEF/Linux/lib \
	-ldl \
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
strip bin/TestsDatabase
strip bin/libcef.so
cp ../TestsDatabase/Linux/install/* .
cd ..
mv build TestsDatabase-1.0
tar -cf TestsDatabase-1.0.tar TestsDatabase-1.0
xz -e TestsDatabase-1.0.tar
rm -r TestsDatabase-1.0

# Creates a .sh installer
cat > install_TestsDatabase_linux.sh << 'EOF'
#!/usr/bin/env bash

created_folder="TestsDatabase-1.0"
file_to_execute="install.sh"

echo "Unpacking installer..."

# some tail don't understand the "-n", so we test for it
use_tail="-n"
tail -n +1 "$0" > /dev/null 2> /dev/null || use_tail=""
tail $use_tail +19 "$0" | tar -xJ
cd "$created_folder"
bash "$file_to_execute"
cd ..
rm -R "$created_folder"

exit 0

EOF

cat TestsDatabase-1.0.tar.xz >> install_TestsDatabase_linux.sh
chmod +x install_TestsDatabase_linux.sh

