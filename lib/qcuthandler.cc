/*! \file
 * implementation functions for class qcuthandler
 */

# include "qcuthandler.hh"
# include "qmisc.hh"
# include "qexception.hh"


#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif


qcuthandler::qcuthandler():del(false) { }

qcuthandler::qcuthandler(char *str):del(false) { 

  char *left,*right;

  left = str;
  right = strchr(str,'-');
  if (!right)
    throw qexception(__PRETTY_FUNCTION__,_("no '-' found"));
  else 
    *right++=0;

 // if left or right time specifiers are empty, take default values:
 //  beginning and end of file respectively.
 // notice that '-' is now 0.
  if (*left)
    Begin = qvf(time2ms(left),qvf::MILLISECONDS);  
cerr << Begin << endl << flush;
  if (*right)
    End = qvf(time2ms(right),qvf::MILLISECONDS);  
cerr << End << endl << flush;

// es pot comprovar ja ací que begin<=end...

// cerr << *this << endl << flush;
}

void qcuthandler::setBegin (qvf &B) { Begin = B; } 
void qcuthandler::setEnd   (qvf &E) { End = E; }
void qcuthandler::setbegin (qvf &b) { begin = b; }
void qcuthandler::setend   (qvf &e) { end = e; }
void qcuthandler::setSize  (qvf &s) { size = s; }
void qcuthandler::setOutfile (string o) { outfile = o; }
void qcuthandler::setDel (bool d)   { del = d; }
 
qvf& qcuthandler::getBegin() { return Begin; }
qvf& qcuthandler::getEnd()   { return End; }
qvf& qcuthandler::getbegin() { return begin; }
qvf& qcuthandler::getend()   { return end; }
qvf& qcuthandler::getSize()  { return size; }
string qcuthandler::getOutfile () { return outfile; }
bool qcuthandler::getDel()   { return del; }
