/*! \file
 * implementation functions for class qexception
 */

# include "qexception.hh"

string qexception::getInfo() {
  return info;
}

void qexception::print (ostream *os) {

#ifdef QVERBOSE
  *os << metode << ": " << info;
#else
  *os << info;
#endif
}

ostream &operator<<(ostream &os, qexception &e) {
  e.print(&os);
  return os;
}


