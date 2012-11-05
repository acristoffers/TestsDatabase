#include <iostream>

#include <QtCore>

#include "FolderCompressor.h"

int main (int argc, char const *argv[])
{
    if ( argc < 3 ) {
        std::cout << "You need to specify a folder and a output file, or a file and a output folder" << std::endl;
        return 0;
    }

    QString file, folder;
    bool extract = false;

    QFileInfo finfo(argv[1]);

    if ( !finfo.exists() ) {
        std::cout << "File or folder '" << argv[1] << " does not exist" << std::endl;
        return 0;
    }

    if ( finfo.isDir() ) {
        folder = finfo.canonicalFilePath();
        file   = argv[2];
    } else if ( finfo.isFile() ) {
        file   = finfo.canonicalFilePath();
        folder = argv[2];
        extract = true;
    }

    FolderCompressor fc;

    if ( extract ) {
        std::cout << "Extracting '" << file.toLocal8Bit().data() << "' to '" << folder.toLocal8Bit().data() << "'" << std::endl;
        fc.decompressFolder(file, folder);
    } else {
        std::cout << "Compressing '" << folder.toLocal8Bit().data() << "' to '" << file.toLocal8Bit().data() << "'" << std::endl;
        fc.compressFolder(folder, file);
    }

    return 0;
}
