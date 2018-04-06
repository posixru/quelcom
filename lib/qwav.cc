/*! \file
 * implementation functions for class qwav
 */

# include "qwav.hh"
# include "qexception.hh"

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif


u_int32_t qwav::append (qwav w) {

  if (!compatible(w))
    throw qexception (__PRETTY_FUNCTION__,
                      getName() + string(": ") + w.getName() +
                      string(_(" is not compatible")));
  
  qfile::append(w.getMap()+HEADERSIZE,w.getSize()-HEADERSIZE);

  remap(getMap());
  addSamples(w.getSamples());

  return getSamples();
}


void qwav::print (ostream *os) {

  u_int32_t msduration;

  msduration = getMsDuration();

 // cal posar les milèssimes?
  os->form("%s: %u Hz  %u bits  %s  %2u:%02u.%02u",
	   getName().c_str(),
           getSampleRate(),getBitsPerSample(),
           ((getChannels()==1)?"mono":"stereo"),
           msduration/60000,(msduration/1000)%60,(msduration%1000)/10);

}


qwav::qwav(string name, u_int32_t flags=READ) 
     : qfile (name,flags), 
       qwavheader(getMap(),getSize()) { 

}


bool qwav::compatible (qwav &w) {

  return (getStereo() == w.getStereo())    &&
       (getChannels() == w.getChannels())  &&
     (getSampleRate() == w.getSampleRate());
}


u_int32_t qwav::cut (u_int32_t firstsample, u_int32_t lastsample) {

 // s'hauria de comprovar la consistència dels paràmetres...

  qfile::cut(getOffset(firstsample),getOffset(lastsample)+getBytesPerSample()-1);

  remap(getMap());
  setSamples((getSize()-qwavheader::HEADERSIZE)/getBytesPerSample());

  return getSamples();
}


void qwav::move (u_int32_t sample_start, u_int32_t sample_end, u_int32_t sample_dest) {

  if (sample_end<sample_start) {
    return; 	// ...
  }

  if (sample_start>getSamples()) {
    return; 	// ...
  }

  if (sample_end>getSamples()) {
    sample_end = getSamples();	// ...
  }

 // revisar d'ací en avall...
  u_int32_t sample_number = sample_end-sample_start;
  
  if (sample_number+sample_dest-1>getSamples()) {
    return;	// ...
  }

  qfile::move(getOffset(sample_start),
              getOffset(sample_end)+getBytesPerSample()-1,
              getOffset(sample_dest));
}


void qwav::truncate (u_int32_t samples) {

  if (samples>=getSamples())
    return;

  qfile::truncate(HEADERSIZE+samples*getBytesPerSample());

  remap(getMap());

  setSamples(samples);
}


u_int32_t qwav::cut (qcuthandler &h) {

  if ((!h.getDel()) && (h.getOutfile()==""))
    return 0;	
   
  //
  // an schema with the cut specifications and what will be done
  //  B => begin point, from the beginning of the file
  //  E => end point, from the beginning of the file
  //  b => begin point, from the end of the file
  //  e => end point, from the end of the file
  //  s => size of the fragment
  // 
  //  0 means not specified 	1 means specified
  //
  //   B b E e s
  //   0 0 0 0 0	throw
  //   0 0 0 0 1	0 .. s
  //   0 0 0 1 0	0 .. e
  //   0 0 0 1 1	e-s .. e
  //   0 0 1 0 0	0 .. E
  //   0 0 1 0 1	E-s .. E
  //   0 0 1 1 0	throw
  //   0 0 1 1 1	throw
  //   0 1 0 0 0	b .. EOF
  //   0 1 0 0 1	b .. b+s
  //   0 1 0 1 0	b .. e
  //   0 1 0 1 1	throw
  //   0 1 1 0 0	b .. E
  //   0 1 1 0 1	throw
  //   0 1 1 1 0	throw
  //   0 1 1 1 1	throw
  //   1 0 0 0 0	B .. EOF
  //   1 0 0 0 1	B .. B+s
  //   1 0 0 1 0	B .. e
  //   1 0 0 1 1	throw
  //   1 0 1 0 0	B .. E
  //   1 0 1 0 1	throw
  //   1 0 1 1 0	throw
  //   1 0 1 1 1	throw
  //   1 1 0 0 0	throw
  //   1 1 0 0 1	throw
  //   1 1 0 1 0	throw
  //   1 1 0 1 1	throw
  //   1 1 1 0 0	throw
  //   1 1 1 0 1	throw
  //   1 1 1 1 0	throw
  //   1 1 1 1 1	throw

  // it is ugly, actually, but the table above should make things clear
  
  u_int32_t cutmode=0;
  if ( h.getBegin().getFormat()!=qvf::UNDEFINED ) cutmode+=16;
  if ( h.getbegin().getFormat()!=qvf::UNDEFINED ) cutmode+=8;
  if (   h.getEnd().getFormat()!=qvf::UNDEFINED ) cutmode+=4;
  if (   h.getend().getFormat()!=qvf::UNDEFINED ) cutmode+=2;
  if (  h.getSize().getFormat()!=qvf::UNDEFINED ) cutmode+=1;

  u_int32_t first_sample, last_sample;

#ifdef QVERBOSE
  cerr << "cut mode: " << cutmode << endl;
#endif

 // comprovació de zeros: -s 3b => -s 0!!
  switch (cutmode) {
    case 0:
      //cerr << "cut: you must specify where to cut" << endl;
      return 0;
    case 1: 	// 1 .. s		(p)
      first_sample = 1;
      last_sample = getSample(h.getSize());
    break;
    case 2:	// 1 .. e		(p)
      first_sample = 1;
      last_sample = getSamples()+1-getSample(h.getend());
    break;
    case 3:	// e-s .. e		(p)
      last_sample = getSamples()+1-getSample(h.getend());
      first_sample = last_sample-getSample(h.getSize())+1;
    break;
    case 4:	// 1 .. E		(p)
      first_sample = 1;
      last_sample = getSample(h.getEnd());
    break;
    case 5:	// E-s .. E		(p)
      last_sample = getSample(h.getEnd());
      first_sample = last_sample-getSample(h.getSize())+1;
    break;
    case 6:	// E+e => throw		(p)
    case 7:	// E+e+s => throw	(p)
    case 14:	// b+E+e => throw	(p)
    case 15:	// b+E+e+s => throw	(p)
    case 22:	// B+E+e => throw	(p)
    case 23:	// B+E+e+s => throw	(p)
      throw qexception(__PRETTY_FUNCTION__,string(_("options -E and -e cannot be used together")));
    case 8:	// b .. EOF		(p)
      first_sample = getSamples()+1-getSample(h.getbegin());
      last_sample = getSamples();
    break;
    case 9:	// b .. b+s		(p)
      first_sample = getSamples()+1-getSample(h.getbegin());
      last_sample = first_sample+getSample(h.getSize())-1;
    break;
    case 10:	// b .. e		(p)
      first_sample = getSamples()+1-getSample(h.getbegin());
      last_sample = getSamples()+1-getSample(h.getend());
    break;
    case 11:	// b+e+s
    case 13:	// b+E+s
    case 19:	// B+e+s
    case 21:	// B+E+s
      throw qexception(__PRETTY_FUNCTION__,string(_("options (-b or -B), (-e or -E) and -s cannot be used together")));
    case 12:	// b .. E		(p)
      first_sample = getSamples()+1-getSample(h.getbegin());
      last_sample = getSample(h.getEnd());
    break;
    case 16:	// B .. EOF		(p)
      first_sample = getSample(h.getBegin());
      last_sample = getSamples();
    break;
    case 17:	// B .. B+s		(p)
      first_sample = getSample(h.getBegin());
      last_sample = first_sample+getSample(h.getSize())-1;
    break;
    case 18:	// B .. e		(p)
      first_sample = getSample(h.getBegin());
      last_sample = getSamples()+1-getSample(h.getend());
    break;
    case 20:	// B .. E
      first_sample = getSample(h.getBegin());
      last_sample = getSample(h.getEnd());
    break;
    case 24 ... 31: // B+b+...
      throw qexception(__PRETTY_FUNCTION__,string(_("options -B and -b cannot be used together")));
    default: 	// this should never happen
      cerr << "quelcom panic!" << endl;
      return 0;
  }

#ifdef QVERBOSE
  cerr << "cut: " << last_sample-first_sample+1 << " samples(" << first_sample << ".." << last_sample << ") => ";
#endif

  if (first_sample>last_sample)
    throw qexception(__PRETTY_FUNCTION__,
                     string(_("cut: last sample > first sample")));
// especificar quines mostres són

  if (last_sample>getSamples())
    last_sample = getSamples();

  if (h.getOutfile()!="") 
    getWav(h.getOutfile(),first_sample,last_sample);

  if (h.getDel())
    cut(first_sample,last_sample);

  return 1;
}


qwav& qwav::getWav(string name, u_int32_t startsample, u_int32_t endsample) {

 // s'ha de poder resoldre més elegantment...: un constructor nul de wav (nom de fitxer + paràmetres i cap mostra) + un append
  qfile newwav(name,NEW);

  newwav.append(getMap(),qwavheader::HEADERSIZE);
  newwav.append(getMap()+getOffset(startsample),(endsample-startsample+1)*getBytesPerSample());

  qwavheader h(newwav.getMap());
  h.setSamples(endsample-startsample+1);

 // return ???
}


ostream &operator<<(ostream &os, qwav &u) {
  u.print(&os);
  return os;
}
