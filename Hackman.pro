LIBS += \
    -lGLEW \
    -lglfw \
    -lGL

QMAKE_CXXFLAGS += \
    -std=c++11 \
    -std=gnu++11

HEADERS += \
    window.hpp

SOURCES += \
    window.cpp \
    main.cpp
