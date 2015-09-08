LIBS += \
    -lGLEW \
    -lglfw \
    -lGL

QMAKE_CXXFLAGS += \
    -std=c++11 \
    -std=gnu++11

HEADERS += \
    window.hpp \
    setupgame.hpp \
    lodepng.h \
    loadtexture.hpp \
    setupprogram.hpp \
    resourcemanager.hpp \
    readtostream.hpp \
    map.hpp \
    visibleobject.hpp \
    character.hpp \
    setupobjects.hpp \
    playgame.hpp \
    ai.hpp \
    directionenum.hpp \
    collisiondetection.hpp \
    textrenderer.hpp \
    mainmenu.hpp \
    pausemenu.hpp \
    menuupdate.hpp \
    highscoremenu.hpp \
    mapeditor.hpp \
    createbuffer.hpp \
    writedata.hpp

SOURCES += \
    window.cpp \
    main.cpp \
    setupgame.cpp \
    lodepng.cpp \
    loadtexture.cpp \
    setupprogram.cpp \
    resourcemanager.cpp \
    readtostream.cpp \
    map.cpp \
    character.cpp \
    visibleobject.cpp \
    setupobjects.cpp \
    playgame.cpp \
    ai.cpp \
    collisiondetection.cpp \
    textrenderer.cpp \
    mainmenu.cpp \
    pausemenu.cpp \
    menuupdate.cpp \
    highscoremenu.cpp \
    mapeditor.cpp \
    createbuffer.cpp \
    writedata.cpp

OTHER_FILES += \
    textured.vs \
    textured.fs \
    map.txt \
    maps.txt \
    character.vs \
    text.vs \
    text.fs

DISTFILES += \
    highscores.txt \
    colouredsquare.vs \
    colouredsquare.fs
