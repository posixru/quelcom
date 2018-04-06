/*! \file
 * implementation functions for class qwavheader
 */

# include "qwavheader.hh"
# include "qexception.hh"
# include "qmisc.hh"

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif


const u_int32_t qwavheader::HEADERSIZE = sizeof(struct header);


qwavheader::qwavheader (caddr_t p, u_int32_t wavlength=0) {

  header = (struct header *)p;

  valid();

  if (wavlength)
    validLength(wavlength);
}


void qwavheader::remap(caddr_t p) {

  header = (struct header*)p;
}


u_int32_t qwavheader::setSamples(u_int32_t n) {

 // protegir-la d'abusos... 
  header->datalength = n*header->bytespersample;
  header->rifflength = header->datalength+36;

#ifdef QVERBOSE
  cerr << "sample number set to " << n << endl;
#endif

  return getSamples();
}


u_int32_t qwavheader::addSamples(int increment) {
  
 // protegir-la d'abusos...
  setSamples(getSamples()+increment);

  return getSamples();
}


u_int32_t qwavheader::getSamples() {
  return header->datalength / header->bytespersample;
}


bool qwavheader::compatible (const qwavheader &other) {

  return (header->channels == other.header->channels)  &&
       (header->samplerate == other.header->samplerate);
}


void qwavheader::write(caddr_t p) {

  memmove(p,header,HEADERSIZE);
}


bool qwavheader::valid () {
    
  if (!strcmpn(header->riff,"RIFF",4) ||
      !strcmpn(header->wave,"WAVE",4) ||
      !strcmpn(header->fmt_,"fmt ",4) ||
      !strcmpn(header->data,"data",4) ||
      (header->fmtlength!=16        ) ||
      (header->format!=1            )) {

    throw qexception(__PRETTY_FUNCTION__,_("header format error"));
  }

  if ( (header->channels!=2 && header->channels!=1) ||
       (header->samplerate>48000 || header->samplerate<8000) ||
       (header->bytespersample!=1 &&
        header->bytespersample!=2 &&
        header->bytespersample!=4) )

    throw qexception(__PRETTY_FUNCTION__,_("header value error"));

 // caldria comprovar que bitspersample=bytespersample*channels*8
 // caldria comprovar que bytespersec=bytespersample*samplerate
 
  return true;
}


bool qwavheader::validLength (u_int32_t length) {

  if ((header->datalength != length-44) || (header->rifflength != length-8))
    throw qexception(__PRETTY_FUNCTION__,_("length mismatch"));

  return true;
}


u_int32_t qwavheader::getMsDuration() { 
  return 1000ULL*header->datalength/(header->bytespersample*header->samplerate); 
} 

u_int32_t qwavheader::getSampleRate() { return header->samplerate; } 
u_int32_t qwavheader::getChannels() { return header->channels; } 


u_int32_t qwavheader::getOffset (qvf &vf) { 

  return getSample(vf)*getBytesPerSample()+HEADERSIZE;
}


u_int32_t qwavheader::getOffset (u_int32_t sample) { 
 // assegurar que sample in (1..getSamples())
  return HEADERSIZE+(sample-1)*header->bytespersample; 
}


u_int32_t qwavheader::getSample (qvf &vf) { 

  u_int32_t sample;

  switch (vf.getFormat()) {

    case qvf::BYTES: sample = vf.getValue()/getBytesPerSample(); break;
    case qvf::KBYTES: sample = (vf.getValue()*1024)/getBytesPerSample(); break;
    case qvf::MBYTES: sample = (vf.getValue()*1024*1024)/getBytesPerSample(); break;

    case qvf::MILLISECONDS: sample = (1ULL*vf.getValue()*getSampleRate())/1000; break;
    case qvf::SECONDS: sample = vf.getValue()*getSampleRate(); break;
    case qvf::MINUTES: sample = 60*vf.getValue()*getSampleRate(); break;

    case qvf::SPECIFIC: sample = vf.getValue(); break;

    default:
      throw qexception(__PRETTY_FUNCTION__,
              string(_("format not recognized: "))+char2string(vf.getFormat()));
  }

  if (sample<1 || sample>getSamples())
    throw qexception(__PRETTY_FUNCTION__,string(_("sample out of range: "))+uint2string(sample));
  // i si poséssim el qvf, millor

  return sample;
}

u_int32_t qwavheader::getBitsPerSample() { return header->bitspersample; }
u_int32_t qwavheader::getBytesPerSample() { return header->bytespersample; }
bool qwavheader::getStereo() { return header->channels==2; }


