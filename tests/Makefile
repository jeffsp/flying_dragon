# Flying Dragon Tests Makefile
#
# Copyright (C) 2008 Jeffrey S. Perry
#
# This program is free software: you can redistribute it
# and/or modify it under the terms of the GNU General
# Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public
# License along with this program.  If not, see
# <http://www.gnu.org/licenses/>.
#
# contact: jeffsp@gmail.com

.PHONY: all check clean pro release

SOURCES=$(shell ls -1 *.cc)
TARGETS=$(basename $(SOURCES))
DUMMY_FILES=$(addsuffix .dummy,$(TARGETS))
CHECK_FILES=$(addsuffix .check,$(TARGETS))
MAKE_FILES=$(addsuffix .Makefile,$(TARGETS))
PRO_FILES=$(addsuffix .pro,$(TARGETS))
QFLAGS="CONFIG+=debug"

all: $(DUMMY_FILES) $(MAKE_FILES) $(PRO_FILES)

%.dummy: %.Makefile
	$(MAKE) -f $<

%.Makefile: %.pro
	qmake -o $*.Makefile $*.pro

%.pro: Makefile
	qmake -project -nopwd -o $*.pro $*.cc \
		QT+=network \
		INCLUDEPATH+=.. \
		INCLUDEPATH+=../../horny-toad \
		INCLUDEPATH+=../../jack-rabbit \
		INCLUDEPATH+=../../screech-owl \
		HEADERS+=../camera_controller.h \
		HEADERS+=../camera_controller_widget.h \
		HEADERS+=../camera_controls_dialog.h \
		HEADERS+=../camera_control_widgets.h \
		HEADERS+=../camera_dialog.h \
		HEADERS+=../camera_manager.h \
		HEADERS+=../camera_settings_dialog.h \
		HEADERS+=../camera_setting_widgets.h \
		HEADERS+=../client.h \
		HEADERS+=../client_widget.h \
		HEADERS+=../connection_exceptions.h \
		HEADERS+=../connection.h \
		HEADERS+=../connection_manager.h \
		HEADERS+=../connection_manager_widget.h \
		HEADERS+=../connections_view.h \
		HEADERS+=../exception_enabled_app.h \
		HEADERS+=../frame.h \
		HEADERS+=../frame_manager.h \
		HEADERS+=../message.h \
		HEADERS+=../message_manager.h \
		HEADERS+=../message_manager_widget.h \
		HEADERS+=../new_connection_dialog.h \
		HEADERS+=../persistent_dialog.h \
		HEADERS+=../server.h \
		HEADERS+=../server_widget.h \
		SOURCES+=../../screech-owl/v4l2_camera.cc \
		SOURCES+=../../screech-owl/cnull.cc \
		RESOURCES+=../flying_dragon.qrc

check: $(CHECK_FILES)

%.check:
	./$*

clean:
	rm -f moc_* *.o $(TARGETS) $(PRO_FILES) $(MAKE_FILES)

release:
	$(MAKE) QFLAGS=\"CONFIG+=release\"
