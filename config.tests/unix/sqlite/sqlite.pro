SOURCES = sqlite.cpp
CONFIG -= qt dylib
LIBS += $$QT_LFLAGS_SQLITE
QMAKE_CXXFLAGS *= $$QT_CFLAGS_SQLITE
