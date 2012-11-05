#!/usr/bin/env bash

if [[ ! -e bin/qtziper ]]; then
    mkdir bin
    cd dependencies/QtFolderCompressor
    qmake;make
    mv qtziper ../../bin/
    make distclean
    cd ../..
fi

rm resources/html.qtx
bin/qtziper html resources/html.qtx

qmake;make
#make clean
#rm Makefile

#rm -r bin
#rm resources/html.qtx
