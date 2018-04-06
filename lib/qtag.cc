/*! \file
 * implementation functions for class qtag
 */

# include "qtag.hh"
# include "qmisc.hh"
# include "qexception.hh"

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif

const u_int32_t qtag::LENGTH = 128;
const u_int32_t qtag::fieldlengths[7] = { 3, 30, 30, 30, 4, 30, 1};


caddr_t qtag::getMap() { return (caddr_t) tag; }


bool qtag::isValid() {

  return tag!=0;
}


void qtag::cleanFields() {

  if (!tag)
    throw qexception(__PRETTY_FUNCTION__,_("tag is invalid"));

  int i;
  char *p;

  i = fieldlengths[ALBUM]-1;
  p = tag->album;
  while (i>=0 && (p[i]==' ')) p[i--]=0;
  i = fieldlengths[ARTIST]-1;
  p = tag->artist;
  while (i>=0 && (p[i]==' ')) p[i--]=0;
  i = fieldlengths[TITLE]-1;
  p = tag->title;
  while (i>=0 && (p[i]==' ')) p[i--]=0;
}
  

qtag::qtag (caddr_t p) {

  if (strcmpn(p,"TAG",3))
    tag = (struct tag*) p;
  else
    tag = 0;

#ifdef QVERBOSE
  if (tag)
    cerr << "tag found mapped at address " << p << endl;
#endif
}


qtag::qtag (qfile &f) {

  caddr_t p = f.getMap()+f.getSize()-LENGTH;

  if (strcmpn(p,"TAG",3))
    tag = (struct tag*) p;
  else
    tag = 0;

#ifdef QVERBOSE
  if (tag)
    cerr << "tag found mapped at address " << p << endl;
#endif
}


void qtag::get (enum field f, char *str) {

  if (!tag)
    throw qexception(__PRETTY_FUNCTION__,_("tag is invalid"));

  switch (f) {
    case     TAG: strncpy(str,tag->tagid,  fieldlengths[f]); break;
    case   TITLE: strncpy(str,tag->title,  fieldlengths[f]); break;
    case  ARTIST: strncpy(str,tag->artist, fieldlengths[f]); break;
    case   ALBUM: strncpy(str,tag->album,  fieldlengths[f]); break;
    case    YEAR: strncpy(str,tag->year,   fieldlengths[f]); break;
    case COMMENT: strncpy(str,tag->comment,fieldlengths[f]); break;
    case   GENRE: strncpy(str,tag->genre,  fieldlengths[f]); break;

    default: // this should never happen...
      throw qexception(__PRETTY_FUNCTION__,_("quelcom internal error"));
  }
}


void qtag::set (enum field f, char *str) {

  if (!tag)
    throw qexception(__PRETTY_FUNCTION__,_("tag is invalid"));

  switch (f) {
    case     TAG: strncpy(tag->tagid,  str,fieldlengths[f]); break;
    case   TITLE: strncpy(tag->title,  str,fieldlengths[f]); break;
    case  ARTIST: strncpy(tag->artist, str,fieldlengths[f]); break;
    case   ALBUM: strncpy(tag->album,  str,fieldlengths[f]); break;
    case    YEAR: strncpy(tag->year,   str,fieldlengths[f]); break;
    case COMMENT: strncpy(tag->comment,str,fieldlengths[f]); break;
    case   GENRE: strncpy(tag->genre,  str,fieldlengths[f]); break;

    default: // this should never happen...
      throw qexception(__PRETTY_FUNCTION__,_("quelcom internal error"));
  }
}


void qtag::print (ostream *os) {

  os->form ("%-0.30s - %-0.30s - %-0.30s",tag->artist,tag->album,tag->title);
}


ostream &operator<<(ostream &os, qtag &t) {
  t.print(&os);
  return os;
}
