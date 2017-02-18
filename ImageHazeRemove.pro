TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    hazeremovehandler.cpp \
    guidedfilter.cpp

INCLUDEPATH +=  \
    /usr/local/include/opencv     \
    /usr/local/include/opencv2  \

LIBS += \
    -L/usr/local/lib                \
    -lopencv_core -lopencv_highgui -lopencv_imgproc

HEADERS += \
    hazeremovehandler.h \
    guidedfilter.h
