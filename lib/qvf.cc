/*! \file
 * implementation functions for class qvf
 */

# include <stdio.h>	// sscanf

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif

# include "qvf.hh"
# include "qexception.hh"
# include "qmisc.hh"

char qvf::format2char(enum format f) {
 
  switch (f) {
    case BYTES: return 'b';
    case KBYTES: return 'k';
    case MBYTES: return 'M';
    case MINUTES: return 'm';
    case SECONDS: return 's';
    case MILLISECONDS: return 'j';
    case SPECIFIC: return ' ';
    case UNDEFINED: return 'X';
  }
  return '?';
}


enum qvf::format qvf::char2format(char c) {

  switch (c) {
    case 'b': return BYTES;
    case 'k': return KBYTES;
    case 'M': return MBYTES;
    case 'm': return MINUTES;
    case 's': return SECONDS;
    case 'j': return MILLISECONDS;
    case ' ': return SPECIFIC;
  }

  throw qexception(__PRETTY_FUNCTION__,
                   string(_("invalid format specifier: "))+char2string(c));  
}

qvf::qvf() { 

  format=UNDEFINED; 
} 


qvf::qvf(u_int32_t v, enum format f=SPECIFIC):value(v),format(f) { }


qvf::qvf(char *s) {

  char c;

 // valors <0 es llegeixen com a MAX-valor ...
  switch (sscanf(s,"%u%c",&value,&c)) {
    case 1:
      c = ' ';
    case 2:
      format = char2format(c);
    break;
    default: 
      throw qexception(__PRETTY_FUNCTION__,
                       string(_("error reading: "))+string(s));
  }
}


unsigned int qvf::getValue() { 

  if (format==UNDEFINED)
    throw qexception(__PRETTY_FUNCTION__,_("format is undefined"));

  return value; 
}


enum qvf::format qvf::getFormat() { return format; }
void qvf::set(unsigned int v, enum format f=SPECIFIC) { value=v; format=f; }

void qvf::print (ostream *os) {
  *os << "{" << value << "," << format2char(format) << "}";
}
ostream &operator<<(ostream &os, qvf &q) {
  q.print(&os);
  return os;
}
