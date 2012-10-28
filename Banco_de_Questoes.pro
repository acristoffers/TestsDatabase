macx {
    TARGET = "Test Database"
} else {
    TARGET = "Banco de Questoes"
}

QT += webkit sql

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += cpp/main.cpp \
    cpp/database.cpp \
    cpp/uibridge.cpp \
    cpp/application.cpp

HEADERS += \
    cpp/database.h \
    cpp/uibridge.h \
    cpp/application.h

TRANSLATIONS = \
    translations/pt.ts \
    translations/de.ts

RESOURCES += resources/Resources.qrc

RC_FILE = resources/winrc.rc


ICON = resources/icon.icns
QMAKE_INFO_PLIST = resources/Info.plist


html_folder.source = html
tr_folder.source = translations

macx {
    lproj_from = $$PWD/resources/MacTranslations/
    lproj_to = $$OUT_PWD/$${TARGET}.app/Contents/Resources/
    QMAKE_POST_LINK += $(COPY_DIR) \"$$lproj_from\" \"$$lproj_to\"
}

win32{
    CONFIG(release, debug|release){
        html_folder.target = release
    }
    CONFIG(debug, debug|release){
        html_folder.target = debug
    }
} else {
    html_folder.target = .
}

tr_folder.target = $$html_folder.target

DEPLOYMENTFOLDERS += html_folder tr_folder

win32 {
    copyCommand =
    for(deploymentfolder, DEPLOYMENTFOLDERS) {
        source = $$PWD/$$eval($${deploymentfolder}.source)
        source = $$replace(source, /, \\)
        sourcePathSegments = $$split(source, \\)
        target = $$OUT_PWD/$$eval($${deploymentfolder}.target)/$$last(sourcePathSegments)
        target = $$OUT_PWD/$$eval($${deploymentfolder}.target)/$$last(sourcePathSegments)
        target = $$replace(target, /, \\)
        target ~= s,\\\\\\.?\\\\,\\,
        !isEqual(source,$$target) {
            !isEmpty(copyCommand):copyCommand += &&
            isEqual(QMAKE_DIR_SEP, \\) {
                copyCommand += $(COPY_DIR) \"$$source\" \"$$target\"
            } else {
                source = $$replace(source, \\\\, /)
                target = $$OUT_PWD/$$eval($${deploymentfolder}.target)
                target = $$replace(target, \\\\, /)
                copyCommand += test -d \"$$target\" || mkdir -p \"$$target\" && cp -r \"$$source\" \"$$target\"
            }
        }
    }
    !isEmpty(copyCommand) {
        copyCommand = @echo Copying application data... && $$copyCommand
        copydeploymentfolders.commands = $$copyCommand
        first.depends = $(first) copydeploymentfolders
        export(first.depends)
        export(copydeploymentfolders.commands)
        QMAKE_EXTRA_TARGETS += first copydeploymentfolders
    }
} else:unix {
    # Assumed to be a Desktop Unix
    copyCommand =
        for(deploymentfolder, DEPLOYMENTFOLDERS) {
            source = $$PWD/$$eval($${deploymentfolder}.source)
            source = $$replace(source, \\\\, /)
            macx {
                target = $$OUT_PWD/$${TARGET}.app/Contents/Resources/$$eval($${deploymentfolder}.target)
            } else {
                target = $$OUT_PWD/$$eval($${deploymentfolder}.target)
            }
            target = $$replace(target, \\\\, /)
            sourcePathSegments = $$split(source, /)
            targetFullPath = $$target/$$last(sourcePathSegments)
            targetFullPath ~= s,/\\.?/,/,
            !isEqual(source,$$targetFullPath) {
                !isEmpty(copyCommand):copyCommand += &&
                copyCommand += $(MKDIR) \"$$target\"
                copyCommand += && $(COPY_DIR) \"$$source\" \"$$target\"
            }
        }
        !isEmpty(copyCommand) {
            copyCommand = @echo Copying application data... && $$copyCommand
            copydeploymentfolders.commands = $$copyCommand
            first.depends = $(first) copydeploymentfolders
            export(first.depends)
            export(copydeploymentfolders.commands)
            QMAKE_EXTRA_TARGETS += first copydeploymentfolders
        }

    installPrefix = /opt/$${TARGET}
    for(deploymentfolder, DEPLOYMENTFOLDERS) {
        item = item$${deploymentfolder}
        itemfiles = $${item}.files
        $$itemfiles = $$eval($${deploymentfolder}.source)
        itempath = $${item}.path
        $$itempath = $${installPrefix}/$$eval($${deploymentfolder}.target)
        export($$itemfiles)
        export($$itempath)
        INSTALLS += $$item
    }
}

macx {
    trs_folder = $$OUT_PWD/$${TARGET}.app/Contents/Resources/translations/*.ts
} else {
    trs_folder = $$OUT_PWD/translations/*.ts
}
trs_folder = $$replace(trs_folder, ' ', '\\ ')
trs.commands = @echo "Deleting .ts" && $(DEL_FILE) $$trs_folder
first.depends = $(first) copydeploymentfolders trs
QMAKE_EXTRA_TARGETS += first trs
