TEMPLATE = app
TARGET = ui_app

QT += webkitwidgets network core

INCLUDEPATH += /home/bill/qt-everywhere-opensource-src-5.5.1/qmqtt-master/include
LIBS += -L/home/bill/qt-everywhere-opensource-src-5.5.1/qmqtt-master/lib -lqmqtt

HEADERS += include/common.h \
           include/webview.h \
           include/maincontrol.h \
		   include/com_https.h \
		   include/com_qmqtt.h \
                   include/localserver.h\
		   include/fw_upgrade.h
		   
SOURCES += mainwindow/main.cpp \
           mainwindow/maincontrol.cpp \
		   mainwindow/webview.cpp \
		   mainwindow/com_https.cpp \
		   mainwindow/com_qmqtt.cpp \
                   mainwindow/localserver.cpp \
		   mainwindow/fw_upgrade.cpp


target.path = $PWD/ui_mainwindow
INSTALLS += target
