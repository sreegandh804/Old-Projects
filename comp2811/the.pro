QT += core gui widgets multimedia multimediawidgets

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    flow_layout.cpp \
    main_window.cpp \
    tag.cpp \
    tags_pane.cpp \
    tomeo.cpp \
    video_button.cpp \
    video_grid_layout.cpp \
    video_player.cpp \
    candystyle.cpp \
    draculastyle.cpp \
    darkstyle.cpp

HEADERS += \
    flow_layout.h \
    main_window.h \
    tag.h \
    tags_pane.h \
    video_button.h \
    video_grid_layout.h \
    video_player.h \
    candystyle.h \
    darkstyle.h \
    draculastyle.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    icons/icons.qrc

