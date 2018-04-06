
MP3BIN=qmp3report qmp3check qmp3info qmp3join qmp3cut 
MP3BINOBJ=$(addsuffix .o,$(MP3BIN))

WAVBIN=qwavheaderdump qwavfade qwavsilence qwavinfo qwavjoin qwavcut
WAVBINOBJ=$(addsuffix .o,$(WAVBIN))

BINTARGETS=$(MP3BIN) $(WAVBIN) 

all: $(BINTARGETS)

#
# if you don't want to include native language support (=get quelcom messages 
#  in a different language than english) remove -DNLS and -DLOCALEDIR... below
#
CXXFLAGS=-DNLS -Wall -DVERSION=\"$(VERSION)\" -DPACKAGE=\"$(PACKAGE)\" -DLOCALEDIR=\"$(LOCALEDIR)\" -I./../lib

LDFLAGS=-lm -L./../lib

$(MP3BINOBJ) $(WAVBINOBJ): %.o: %.cc
	$(CXX) $(CXXFLAGS) -DAPPNAME=\"$*\" -o $@ -c $<

$(MP3BIN): %: %.o
	$(CXX) -o $@ $< -L./../lib -lqextra -lqmp3
	strip $@

$(WAVBIN): %: %.o
	$(CXX) -o $@ $< -L./../lib -lqextra -lqwav
	strip $@

install: all
	mkdir -p $(BINDIR)
	cp -f $(BINTARGETS) $(BINDIR)

clean:
	rm -f *.o $(BINTARGETS)
