/*! \file
 * implementation functions for class qframeheader
 */

# include "qmp3frameheader.hh"
# include "qexception.hh"

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif


// tanta constant i dades estàtiques en una mena de qmp3ct.cc

const u_int32_t qmp3frameheader::HEADERSIZE=sizeof(struct header);
const u_int32_t qmp3frameheader::MASK = (u_int32_t)(0x0c30cfeff); // ff fe fc c3 // la 'f' entre la 'e' i la 'c' és el bitrate. posada a zero fa que no es tinguin en compte els canvis de bitrate i, pt, s'esdevé pseudosuport pels vbr.
const u_int32_t qmp3frameheader::LAYER3 = 1;
const u_int32_t qmp3frameheader::LAYER2 = 2;
const u_int32_t qmp3frameheader::LAYER1 = 3;
const u_int32_t qmp3frameheader::VERSION1 = 3;
const u_int32_t qmp3frameheader::VERSION2 = 2;
const u_int32_t qmp3frameheader::VERSION25 = 0;
const u_int32_t qmp3frameheader::UNDEFINED_VERSION = 1;

static const char *versions[] = { "2.5", "reserved", "2.0", "1.0" };
static const char *layers[] = { "reserved", "III", "II", "I" };

static const u_int32_t samplerates [4][4] = {
  { 11025, 12000,  8000,  0 },
  {     0,     0,     0,  0 },
  { 22050, 24000, 16000,  0 },
  { 44100, 48000, 32000,  0 }
};

// potser fer -1=>0 i fer-ho sense signe...
static const int layer3_bitrates[4][16] = {
  {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,0},		// version 2.5
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},		// undefined
  {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,0},		// version 2
  {0,32,40,48,56,64,80,96,112,128,160,192,224,256,320,0}	// version 1
};

static u_int32_t samplesPerFrame[4] = { 1152, 1152, 384, 0 };

string qmp3frameheader::getLayer() { return string(layers[header->layer]);}
u_int32_t qmp3frameheader::getSamplesPerFrame() { return samplesPerFrame[header->layer]; }
string qmp3frameheader::getVersion() { return string(versions[header->version]);}


caddr_t qmp3frameheader::getMap() {return (caddr_t)header;}
void qmp3frameheader::remap(caddr_t p) {

  header = (struct header*)p;
}

u_int32_t qmp3frameheader::getSignature() {return (*(u_int32_t*)header)&MASK;}
u_int32_t qmp3frameheader::getSampleRate() {

  return samplerates[header->version][header->samplerate];
}
 

bool qmp3frameheader::isSilence () {

 // què xungo: pot ser que hi hagi crc o no, i el main data begin dins de la 
 //  side information és de nou bits

 // el problema és que és segur que existeix el capçal, però ningú no ens assegura que hi hagi alguna cosa més al darrere per tal de no fer un segfault. es podria mirar de fer una classe derivada on HEADERSIZE fos major.

  return false;
}


bool qmp3frameheader::valid (caddr_t p) {

  struct header *header = (struct header*)p;

  return   ( (header->sync1      == 0xff) &&
             (header->sync2      ==  0x7) &&
             (header->version    != UNDEFINED_VERSION) &&
             (header->layer      !=    0) &&
             (header->bitrate    != 0x0f) &&
             (header->samplerate != 0x03) &&
             (header->emphasis   != 0x02) );
}


bool qmp3frameheader::valid () {

  return ! ( (header->sync1      != 0xff) ||
             (header->sync2      !=  0x7) ||
             (header->version    == UNDEFINED_VERSION) ||
             (header->layer      ==    0) ||
             (header->bitrate    == 0x0f) ||
             (header->samplerate == 0x03) ||
             (header->emphasis   == 0x02) );
}


//caddr_t qmp3frameheader::getMap() { return header; }


bool qmp3frameheader::compatible(caddr_t p, u_int32_t signature) {
  return ((*(u_int32_t*)p)&(MASK)) == signature;
}


u_int32_t qmp3frameheader::setNext(u_int32_t bufsize) {

  caddr_t supposed= (char*)header+getLength();

  header = (struct header*) seek_header(supposed,bufsize,getSignature());

  return (char*)header-(char*)supposed;
}

u_int32_t qmp3frameheader::setNext(caddr_t endstream) {

  caddr_t previous = (caddr_t) header;

  header = (struct header*) seek_header((char*)header+getLength(),(char*)endstream-(char*)header+1,getSignature());

  return (char*)header-(char*)previous-getLength();
}


caddr_t qmp3frameheader::seek_header(caddr_t p, u_int32_t bufsize, u_int32_t signature, bool backwards) {

  u_int32_t skipped = 0;

  while ((bufsize>=HEADERSIZE) && 
         (((signature && !compatible(p,signature)) || (!signature && !valid(p))))) {
      bufsize--;
      skipped++;
      if (backwards)
        p--;
      else
        p++;
  }

  if (bufsize<HEADERSIZE)
    throw qexception(__PRETTY_FUNCTION__,_("no valid header found"));

#ifdef QVERBOSE
  if (skipped)
    cerr << "skipped " << skipped << " bytes" << endl;
#endif

  return p;
}


qmp3frameheader::qmp3frameheader(caddr_t p, u_int32_t bufsize, u_int32_t signature) {

  header = (struct header*) seek_header(p,bufsize,signature);
}


u_int32_t qmp3frameheader::getField (enum qmp3frameheaderfield f) {

  switch (f) {

    case SYNC1: 	return header->sync1;
    case SYNC2: 	return header->sync2;
    case _VERSION: 	return header->version;
    case LAYER: 	return header->layer;
    case PROTECTION: 	return header->protection;
    case BITRATE: 	return header->bitrate;
    case SAMPLERATE: 	return header->samplerate;
    case PADDING: 	return header->padding;
    case PRIVAT: 	return header->privat;
    case CHANNEL_MODE: 	return header->channel_mode;
    case MODE_EXTENSION: return header->mode_extension;
    case COPYRIGHT: 	return header->copyright;
    case ORIGINAL: 	return header->original;
    case EMPHASIS: 	return header->emphasis;
    default:
      throw qexception(__PRETTY_FUNCTION__,_("invalid field"));
     // i pintar el valor...
  }
}


void qmp3frameheader::setField (enum qmp3frameheaderfield f, u_int32_t v) {

  switch (f) {

    case SYNC1: 	header->sync1 = v; break;
    case SYNC2: 	header->sync2 = v; break;
    case _VERSION: 	header->version = v; break;
    case LAYER: 	header->layer = v; break;
    case PROTECTION: 	header->protection = v; break;
    case BITRATE: 	header->bitrate = v; break;
    case SAMPLERATE: 	header->samplerate = v; break;
    case PADDING: 	header->padding = v; break;
    case PRIVAT: 	header->privat = v; break;
    case CHANNEL_MODE: 	header->channel_mode = v; break;
    case MODE_EXTENSION: header->mode_extension = v; break;
    case COPYRIGHT: 	header->copyright = v; break;
    case ORIGINAL: 	header->original = v; break;
    case EMPHASIS: 	header->emphasis = v; break;
    default:
      throw qexception(__PRETTY_FUNCTION__,_("invalid field"));
     // i pintar el valor...
  }
}


u_int32_t qmp3frameheader::getLength() {
 
  if (header->layer!=LAYER3)
    throw qexception (__PRETTY_FUNCTION__,_("layer not supported"));
   // i dir quin valor...

  return (144 * getBitRate() * 1000) / getSampleRate() + header->padding;
}

u_int32_t qmp3frameheader::getMsDuration() {

 // this is just true for layer II and layer III
  return (1152*1000)/getSampleRate();
}


void qmp3frameheader::print (ostream *os) {

  *os << "mpeg " << getVersion() << " "
      << "layer " << getLayer() << " "
      << getSampleRate() << "Hz " << getBitRate() << "kbps";
}


u_int32_t qmp3frameheader::getBitRate() {

  if (header->layer!=LAYER3)
    throw qexception (__PRETTY_FUNCTION__,_("only layer III is supported"));

  if (header->version==UNDEFINED_VERSION)
    throw qexception (__PRETTY_FUNCTION__,_("undefined version"));

  return layer3_bitrates[header->version][header->bitrate];
}

 
ostream &operator<<(ostream &os, qmp3frameheader &fh) {

   fh.print(&os);
   return os;
} 
