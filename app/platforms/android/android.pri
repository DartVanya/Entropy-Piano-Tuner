DISTFILES += \
    $$PWD/AndroidManifest.xml \
    $$PWD/src/org/uniwue/tp3/TunerApplication.java \
    $$PWD/build.gradle \
    $$PWD/gradle.properties \
    $$PWD/libs/.gitignore

HEADERS += \
    $$PWD/androidplatformtools.h \
    $$PWD/androidnativewrapper.h

SOURCES += \
    $$PWD/androidplatformtools.cpp \
    $$PWD/androidnativewrapper.cpp

# location of the android package files
ANDROID_PACKAGE_SOURCE_DIR = $$PWD
