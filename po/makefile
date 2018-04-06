
FILES=ca es
MOFILES=$(addsuffix .mo,$(FILES))

all: $(MOFILES)

$(MOFILES): %.mo: %.po
	msgfmt -o $@ $<

install: all
	for i in $(FILES); \
	do \
	  mkdir -p $(LOCALEDIR)/$$i/LC_MESSAGES; \
	  cp $$i.mo $(LOCALEDIR)/$$i/LC_MESSAGES/$(PACKAGE).mo; \
	done

clean:
	rm -f *.mo
