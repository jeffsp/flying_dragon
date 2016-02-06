# Flying Dragon Project File
#
# Copyright (C) 2008
# Jeffrey S Perry
#
# jsp Tue Feb  5 17:10:31 CST 2008

TEMPLATE = app
TARGET = flying_dragon
CONFIG += warn_on
DEPENDPATH += .
INCLUDEPATH+=../horny-toad
INCLUDEPATH+=../jack-rabbit
INCLUDEPATH+=../screech-owl
HEADERS += camera_controller.h
HEADERS += camera_controller_widget.h
HEADERS += camera_dialog.h
HEADERS += camera_setting_widgets.h
HEADERS += camera_settings_dialog.h
HEADERS += client.h
HEADERS += client_widget.h
HEADERS += connection.h
HEADERS += connection_manager.h
HEADERS += connection_manager_widget.h
HEADERS += exception_enabled_app.h
HEADERS += main_window.h
HEADERS += message.h
HEADERS += message_manager.h
HEADERS += persistent_dialog.h
HEADERS += server.h
HEADERS += server_widget.h
FORMS += flying_dragon.ui
SOURCES += flying_dragon.cc
SOURCES += ../screech-owl/v4l2_camera.cc
SOURCES += ../screech-owl/cnull.cc
SOURCES += ../horny-toad/argv.cc
RESOURCES += flying_dragon.qrc
QT += network
