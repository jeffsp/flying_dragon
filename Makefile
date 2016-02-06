# Flying Dragon Makefile
#
# Copyright (C) 2008
# Jeffrey S Perry
#
# jsp Sat Jan 26 16:25:17 CST 2008

.PHONY: all check clean doc release

QFLAGS="CONFIG+=debug"

all: flying_dragon.Makefile
	$(MAKE) -f flying_dragon.Makefile
#$(MAKE) -C tests

check: all
	$(MAKE) -C tests check

clean:
	$(MAKE) -C tests clean
	$(MAKE) -f flying_dragon.Makefile clean
	rm -rf doc
	rm -f flying_dragon.Makefile

doc:
	doxygen flying_dragon.doxygen

flying_dragon.Makefile: flying_dragon.pro
	qmake $(QFLAGS) -o $@ $<

release:
	#$(MAKE) QFLAGS=\"CONFIG+=release\"
	$(MAKE) QFLAGS=\"DEFINES+=QT_NO_DEBUG_OUTPUT\"
