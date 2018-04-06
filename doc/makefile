
all: quelcom.html quelcom.info quelcom.txt

quelcom.html: quelcom.texinfo
	makeinfo --force --html quelcom.texinfo

quelcom.info: quelcom.texinfo
	makeinfo --force quelcom.texinfo

quelcom.txt: quelcom.texinfo
	makeinfo --force --number-sections --no-headers -o quelcom.txt quelcom.texinfo

install: quelcom.info
	mkdir -p $(INFODIR)
	cp quelcom.info $(INFODIR)

clean:
	rm -f quelcom.{html,info,txt}

# installs the deprecated man pages
installman:
	mkdir -p $(MANDIR)/man1;
	for i in old.man.pages/*; \
	do \
	  cp $$i $(MANDIR)/man1; \
	done

