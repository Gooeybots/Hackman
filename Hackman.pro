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
    pausemenu.hpp

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
    pausemenu.cpp

OTHER_FILES += \
    textured.vs \
    textured.fs \
    map.txt \
    maps.txt \
    character.vs \
    text.vs \
    text.fs
