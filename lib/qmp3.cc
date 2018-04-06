/*! \file
 * implementation functions for class qmp3
 */

# include "qmp3.hh"
# include "qexception.hh"
# include "qmisc.hh"

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif


u_int32_t qmp3::append (qmp3 &mp3, bool force) {

 // compatible guarantees both files are scanned
  if (!compatible(mp3,force))
    throw qexception (__PRETTY_FUNCTION__,
                      getName() + string(": ") + 
                      mp3.getName() + string(_(" is not compatible")));
        			        
  if (hasTag) {
    qfile::truncate(getStreamLength());
    remap(qfile::getMap());
    hasTag = false;
  }

  qfile::append(mp3.qfile::getMap(),mp3.getStreamLength());
  remap(qfile::getMap());

  if (force && mp3.isVbr())
    vbr=true;

  frames += mp3.getFrames();
  msduration += mp3.getMsDuration();

  return frames;
}


bool qmp3::compatible(qmp3 &mp3, bool force) {

  if (!isScanned())
    scan();

  if (!mp3.isScanned())
    mp3.scan();

  if (force) 
    return true;

  if (isVbr() && mp3.isVbr())
    return true;

 // i la resta de paràmetres...? actualitzar a la plana man/texinfo del qmp3join
  return getBitRate()==mp3.getBitRate();
}


u_int32_t qmp3::getMsDuration() {

  return (1000ULL * getFrames() * getSamplesPerFrame()) / getSampleRate();
}


qmp3::qmp3(string name, u_int32_t flags=READ)
     : qfile (name,flags),
       qmp3frameheader (qfile::getMap()),
       qtag (qfile::getMap()+getSize()-qtag::LENGTH) {

  hasTag = isValid();

//  frames = scan(getStreamLength());
  scanned = false;
  vbr = isVbr();
  frames = getStreamLength()/qmp3frameheader::getLength();

#ifdef QVERBOSE
  cerr << "mp3 file " << *this << " successfully opened." << endl;
#endif

}

// a ugly chooped scan() to discover fastly vbr streams
bool qmp3::isVbr() {

  if (scanned)
    return vbr;

  u_int32_t bufsize = getStreamLength(); 
  qmp3frameheader *fh = new qmp3frameheader(*this);

 // if the file is little, easier to do a normal scan
 // otherwise, there should be room enough for (int i=5) frames...
  if (bufsize<10*fh->getLength()) {
    scan();
    return vbr;
  }

  u_int32_t bitrate = fh->getBitRate();
  bufsize -= fh->getLength();

  for (int i=5; i>0; i--) {

    fh->setNext();
    if (fh->getBitRate()!=bitrate)
      return true;
  }

  return false;
}

u_int32_t qmp3::scan (u_int32_t bufsize=0) {

  if (!bufsize)
    bufsize = getStreamLength(); 

  frames = 0;
  qmp3frameheader *fh = new qmp3frameheader(*this);

  u_int32_t bitrate = fh->getBitRate();
  msduration = fh->getMsDuration();

  if (bufsize<fh->getLength()) 
    throw qexception(__PRETTY_FUNCTION__,_("first frame incomplete"));

  bufsize -= fh->getLength();
  frames++;

//  char lluna[]={"OD)|(C"};

  while (bufsize>qmp3frameheader::HEADERSIZE) {

    fh->setNext();
    if (bufsize>=fh->getLength()) {
      bufsize -= fh->getLength();
      frames++;
      if (fh->getBitRate()!=bitrate)
        bitrate = 0;
      msduration += fh->getMsDuration();
//      cout << lluna[frames%6] << "\r" << flush;
    }
    else
      throw qexception(__PRETTY_FUNCTION__,
                       uint2string(bufsize)+string(_(" bytes of garbage at the end")));
  }

#ifdef QVERBOSE
  cerr << getName() << ": " << uint2string(frames) << " frames" << endl;
#endif

  if (bufsize)
    throw qexception(__PRETTY_FUNCTION__,
                     uint2string(bufsize)+string(_(" bytes of garbage at the end")));
  
  scanned = true;
  vbr = !bitrate;

  return frames;
}


void qmp3::print (ostream *os) {

  u_int32_t msduration;

  *os << getName() << ": mpeg " << getVersion()
      << "  layer " << getLayer() << " "
      << getSampleRate() << "Hz  ";

  if (!vbr)
    *os << getBitRate() << "kbps";
  else 
    *os << "[vbr]";

  msduration = getMsDuration();
  os->form("  %d:%02d.%03d  ",
           msduration/60000,(msduration/1000)%60,msduration%1000); 

  if (hasTag) 
    this->qtag::print(os);
  else
    *os << _("[no tag]");
}


ostream &operator<<(ostream &os, qmp3 &u) {
  u.print(&os);
  return os;
}


/*
void qmp3::truncate (u_int32_t frames) {

  if (frames>=getFrames())
    return;

  qmp3frameheader fh(qfile::getMap()+getOffset(frames));
  qfile::truncate(getOffset(frames)+fh.getLength());

  remap(qfile::getMap());
}
*/



u_int32_t qmp3::cut (qcuthandler &h) {

  if ((!h.getDel()) && (h.getOutfile()==""))
    return 0;	
   
  if (!scanned) {
    frames = scan();
    scanned = true;
  }

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
  
  u_int32_t mode=0;
  if (h.getBegin().getFormat()!=qvf::UNDEFINED)
    mode+=16;
  if (h.getbegin().getFormat()!=qvf::UNDEFINED)
    mode+=8;
  if (h.getEnd().getFormat()!=qvf::UNDEFINED)
    mode+=4;
  if (h.getend().getFormat()!=qvf::UNDEFINED)
    mode+=2;
  if (h.getSize().getFormat()!=qvf::UNDEFINED)
    mode+=1;
  
  u_int32_t first_frame, last_frame;

#ifdef QVERBOSE
  cerr << "cut mode: " << mode << endl;
#endif

 // comprovació de zeros: -s 3b => -s 0!!
  switch (mode) {
    case 0:
      //cerr << "cut: you must specify where to cut" << endl;
      return 0;
    case 1: 	// 1 .. s		(p)
      first_frame = 1;
      last_frame = getFrame(h.getSize());
    break;
    case 2:	// 1 .. e		(p)
      first_frame = 1;
      last_frame = getFrames()+1-getFrame(h.getend());
    break;
    case 3:	// e-s .. e		(p)
      last_frame = getFrames()+1-getFrame(h.getend());
      first_frame = last_frame-getFrame(h.getSize())+1;
    break;
    case 4:	// 1 .. E		(p)
      first_frame = 1;
      last_frame = getFrame(h.getEnd());
    break;
    case 5:	// E-s .. E		(p)
      last_frame = getFrame(h.getEnd());
      first_frame = last_frame-getFrame(h.getSize())+1;
    break;
    case 6:	// E+e => throw		(p)
    case 7:	// E+e+s => throw	(p)
    case 14:	// b+E+e => throw	(p)
    case 15:	// b+E+e+s => throw	(p)
    case 22:	// B+E+e => throw	(p)
    case 23:	// B+E+e+s => throw	(p)
      throw qexception(__PRETTY_FUNCTION__,
                       _("options -E and -e cannot be used together"));
    case 8:	// b .. EOF		(p)
      first_frame = getFrames()+1-getFrame(h.getbegin());
      last_frame = getFrames();
    break;
    case 9:	// b .. b+s		(p)
      first_frame = getFrames()+1-getFrame(h.getbegin());
      last_frame = first_frame+getFrame(h.getSize())-1;
    break;
    case 10:	// b .. e		(p)
      first_frame = getFrames()+1-getFrame(h.getbegin());
      last_frame = getFrames()+1-getFrame(h.getend());
    break;
    case 11:	// b+e+s
    case 13:	// b+E+s
    case 19:	// B+e+s
    case 21:	// B+E+s
      throw qexception(__PRETTY_FUNCTION__,
                       _("options (-b or -B), (-e or -E) and -s cannot be used together"));
    case 12:	// b .. E		(p)
      first_frame = getFrames()+1-getFrame(h.getbegin());
      last_frame = getFrame(h.getEnd());
    break;
    case 16:	// B .. EOF		(p)
      first_frame = getFrame(h.getBegin());
      last_frame = getFrames();
    break;
    case 17:	// B .. B+s		(p)
      first_frame = getFrame(h.getBegin());
      last_frame = first_frame+getFrame(h.getSize())-1;
    break;
    case 18:	// B .. e		(p)
      first_frame = getFrame(h.getBegin());
      last_frame = getFrames()+1-getFrame(h.getend());
    break;
    case 20:	// B .. E
      first_frame = getFrame(h.getBegin());
      last_frame = getFrame(h.getEnd());
    break;
    case 24 ... 31: // B+b+...
      throw qexception(__PRETTY_FUNCTION__,
                       _("options -B and -b cannot be used together"));
    default: 	// this should never happen
      cerr << "quelcom panic!" << endl;
      return 0;
  }

#ifdef QVERBOSE
  cerr << "cut: " << last_frame-first_frame+1 << " frames(" << first_frame << ".." << last_frame << ") => ";
#endif

  if (first_frame>last_frame)
    throw qexception(__PRETTY_FUNCTION__,_("last frame > first frame"));
// especificar quines mostres són

  if (last_frame>getFrames())
    last_frame = getFrames();

  if (h.getOutfile()!="") 
    getMp3(h.getOutfile(),first_frame,last_frame);

  if (h.getDel())
    cut(first_frame,last_frame);

  return frames;
}


u_int32_t qmp3::getStreamLength () {

  return getSize() - (hasTag?qtag::LENGTH:0);
}

u_int32_t qmp3::getFrames() { return frames; }

u_int32_t qmp3::getFrame (qvf &vf) { 

  u_int32_t frame;

  switch (vf.getFormat()) {

    case qvf::BYTES: frame = ((long long)vf.getValue()*getFrames())/getStreamLength(); break;
    case qvf::KBYTES: frame = (vf.getValue()*getFrames()*1024ULL)/getStreamLength(); break;
    case qvf::MBYTES: frame = (vf.getValue()*getFrames()*1024*1024ULL)/getStreamLength(); break;

    case qvf::MILLISECONDS: frame = (vf.getValue()*getFrames()*1ULL)/getMsDuration(); break;
    case qvf::SECONDS: frame = (vf.getValue()*getFrames()*1000ULL)/getMsDuration(); break;
    case qvf::MINUTES: frame = (vf.getValue()*getFrames()*1000*60ULL)/getMsDuration(); break;

    case qvf::SPECIFIC: frame = vf.getValue(); break;

    default:
      throw qexception(__PRETTY_FUNCTION__,
              string(_("format not recognized: ")));
		// hi ha una funció molt maca format2char
  }

#ifdef QVERBOSE
  cerr << "getFrame: " << vf << " => " << frame << endl;
#endif

  if (frame<1 || frame>getFrames())
    throw qexception(__PRETTY_FUNCTION__,
                     string(_("frame out of range: "))+uint2string(frame));

  return frame;
}


u_int32_t qmp3::cut (u_int32_t firstframe, u_int32_t lastframe) {

  u_int32_t offset_first = getOffset(firstframe);
  u_int32_t offset_last = getOffset(lastframe);
  qmp3frameheader fh(qfile::getMap()+offset_last);
  u_int32_t length_last = fh.getLength();

  qfile::cut(offset_first,offset_last+length_last-1);
  remap(qfile::getMap());
}

// provar donant l'@ de memòria...
u_int32_t qmp3::getOffset(u_int32_t frame) {

  if (frame<1 || frame>getFrames())
    throw qexception(__PRETTY_FUNCTION__,
                     string(_("frame out of range: "))+uint2string(frame));

  if (!isScanned())
    scan();

  if (frame==1)
    return 0;

 // let's look around there...
  long long offset;		// more than 32 bits are needed!!
  caddr_t start, backward, forward;

  offset = ((long long)frame-1)*getStreamLength()/getFrames();
  start = (caddr_t)qfile::getMap()+offset;

  forward = seek_header(start,getSize()-offset,getSignature());
  backward = seek_header(start,offset,getSignature(),true);

  if ((char*)forward-(char*)start<(char*)start-(char*)backward) {
//    cerr << "forward way: skipped " << (char*)forward-(char*)start << " bytes" << endl;
    start = forward;
  }
  else {
//    cerr << "backward way: skipped " << (char*)start-(char*)backward << " bytes" << endl;
    start = backward;
  }

#ifdef QVERBOSE
  cerr << "frame " << frame 
       << " => offset " << (char*)start-(char*)qfile::getMap()
       << "(" << offset << ")" << endl;
#endif

  return (u_int32_t)((char*)start-(char*)qfile::getMap());
}


qmp3& qmp3::getMp3(string name, u_int32_t startframe, u_int32_t endframe) {

  qfile newmp3(name,NEW);

  u_int32_t startoffset = getOffset(startframe);
  qmp3frameheader fh(qfile::getMap()+getOffset(endframe));
  u_int32_t endoffset = getOffset(endframe)+fh.getLength();

  newmp3.append((char*)qfile::getMap()+startoffset,endoffset-startoffset);
//  return ??
}


bool qmp3::isScanned() { return scanned; }

