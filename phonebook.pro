QT += widgets
requires(qtConfig(listview))

SOURCES   = adddialog.cpp \
            contactwidget.cpp \
            main.cpp \
            mainwindow.cpp \
            hintwidget.cpp \
            tablemodel.cpp
HEADERS   = adddialog.h \
            contactwidget.h \
            mainwindow.h \
            hintwidget.h \
            tablemodel.h

# install
QT += sql
