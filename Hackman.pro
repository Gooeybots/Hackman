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
    loadtexture.hpp

SOURCES += \
    window.cpp \
    main.cpp \
    setupgame.cpp \
    lodepng.cpp \
    loadtexture.cpp
