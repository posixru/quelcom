
LIBTARGETS=libqextra.so libqmp3.so libqwav.so

all: $(LIBTARGETS)

#
# if you don't want to include native language support (=get quelcom messages 
#  in a different language than english) remove -DNLS below
#
CXXFLAGS=-DNLS -Wall
LDFLAGS=-lm

MP3LIB=qmp3 qmp3frameheader qtag qreport
MP3LIBSOURCES=$(addsuffix .cc,$(MP3LIB))
MP3LIBHEADERS=$(addsuffix .hh,$(MP3LIB))
MP3LIBOBJECTS=$(addsuffix .o,$(MP3LIB))

WAVLIB=qwav qwavheader qwavsample
WAVLIBSOURCES=$(addsuffix .cc,$(WAVLIB))
WAVLIBHEADERS=$(addsuffix .hh,$(WAVLIB))
WAVLIBOBJECTS=$(addsuffix .o,$(WAVLIB))

EXTRA=qexception qvf qcuthandler qfile qmisc
EXTRASOURCES=$(addsuffix .cc,$(EXTRA))
EXTRAHEADERS=$(addsuffix .hh,$(EXTRA))
EXTRAOBJECTS=$(addsuffix .o,$(EXTRA))

$(EXTRAOBJECTS) $(MP3LIBOBJECTS) $(WAVLIBOBJECTS): %.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) -o $@ -c $<

libqextra.so: $(EXTRAHEADERS) $(EXTRASOURCES) $(EXTRAOBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ -shared $(EXTRAOBJECTS)
	strip $@

libqwav.so: $(WAVLIBSOURCES) $(WAVLIBHEADERS) $(WAVLIBOBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ -shared $(WAVLIBOBJECTS)
	strip $@

libqmp3.so: $(MP3LIBSOURCES) $(MP3LIBHEADERS) $(MP3LIBOBJECTS)
	$(CXX) $(CXXLIBFLAGS) -o $@ -shared $(MP3LIBOBJECTS)
	strip $@

install: all
	mkdir -p $(LIBDIR)
	cp -f $(LIBTARGETS) $(LIBDIR)


clean:
	rm -f *.o $(LIBTARGETS)
