TARGET = "Tests Database"

QT = core gui webkit

RESOURCES += resources/Resources.qrc

RC_FILE = resources/winrc.rc

ICON = resources/icon.icns
QMAKE_INFO_PLIST = resources/Info.plist

INCLUDEPATH += source \
    source/sqlite \
    source/DataBase \
    source/FolderCompressor

HEADERS += \
    source/sqlite/sqlite3.h \
    source/DataBase/DataBasePrivate.h \
    source/DataBase/DataBase.h \
    source/application.h \
    source/userinterface.h \
    source/FolderCompressor/FolderCompressor.h

SOURCES += \
    source/sqlite/sqlite3.c \
    source/DataBase/DataBasePrivate.cpp \
    source/DataBase/DataBase.cpp \
    source/DataBase/DataBase_test.cpp \
    source/DataBase/DataBase_question.cpp \
    source/DataBase/DataBase_category.cpp \
    source/DataBase/DataBase_answer.cpp \
    source/main.cpp \
    source/application.cpp \
    source/userinterface.cpp \
    source/FolderCompressor/FolderCompressor.cpp

TRANSLATIONS += resources/translations/pt.ts \
                resources/translations/de.ts

#
# Copy files to destination
#

win32{
    CONFIG(release, debug|release){
        commands.target = release
    }
    CONFIG(debug, debug|release){
        commands.target = debug
    }
} else {
    commands.target = .
}

macx {
    RESOURCES_PATH = $$OUT_PWD/$${TARGET}.app/Contents/Resources/$$commands.target
} else {
    RESOURCES_PATH = $$OUT_PWD/$$commands.target
}



mkdirs.commands = $(MKDIR)  \"$$RESOURCES_PATH/translations\"

translations.depends = mkdirs
translations.commands = $(COPY_DIR) \"$$PWD/resources/translations/\" \"$$RESOURCES_PATH/translations/\"

mactranslations.commands =

macx {
    mactranslations.commands = $(COPY_DIR) \"$$PWD/resources/MacTranslations/\" \"$$RESOURCES_PATH/\"
}

first.depends = $(first) mkdirs translations mactranslations

QMAKE_EXTRA_TARGETS += first mkdirs translations mactranslations
