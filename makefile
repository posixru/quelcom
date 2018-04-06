
# 
# makefile for quelcom 0.4.0
#

# all variables exported by default to sub-makefiles
export

PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
LIBDIR=$(PREFIX)/lib
LOCALEDIR=$(PREFIX)/share/locale
INFODIR=$(PREFIX)/info
MANDIR=$(PREFIX)/man

PACKAGE=quelcom
VERSION=0.4.0

CXX=g++

DIRS=lib bin doc po

all:
	for i in $(DIRS); \
	do \
	  make -C $$i; \
	done

clean:
	for i in $(DIRS); \
	do \
	  make -C $$i clean; \
	done


# installs the deprecated man pages
installman:
	make -C doc installman

install: all
	for i in $(DIRS); \
	do \
	  make -C $$i install; \
	done


package:
# include generated doc un .mo files
	make -C doc
	make -C po
# clean package
	rm -rf quelcom-$(VERSION)
	mkdir -p quelcom-$(VERSION)
	cp -f makefile ChangeLog INSTALL README TODO quelcom-$(VERSION)
# binary files
	mkdir -p quelcom-$(VERSION)/bin
	cp bin/*.cc bin/makefile quelcom-$(VERSION)/bin
# library files
	mkdir -p quelcom-$(VERSION)/lib
	cp lib/*.{cc,hh} lib/makefile quelcom-$(VERSION)/lib
# documentation files
	mkdir -p quelcom-$(VERSION)/doc
	cp -f doc/*.texinfo doc/quelcom.* doc/texinfo.tex doc/makefile quelcom-$(VERSION)/doc
	cp -Rf doc/old.man.pages quelcom-$(VERSION)/doc
# po/mo files
	mkdir -p quelcom-$(VERSION)/po
	cp -f po/*.{po,mo} po/makefile quelcom-$(VERSION)/po
# build tar file
	tar cvzf quelcom-$(VERSION).tar.gz quelcom-$(VERSION)

tgz:
	tar cvzf quelcom-$(VERSION)-`date +%Y%m%d-%H%M.tgz` bin/*.cc bin/makefile makefile [A-Z]* doc/* tudú* bin/pendents/* Doxyfile lib/*.{cc,hh} lib/makefile po/*.{po,mo} po/makefile

