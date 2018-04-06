/*! \file
 * miscellaneous functions
 */

# include <string>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include "qexception.hh"
# include <iostream.h>
# include <math.h>

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif

u_int32_t time2ms (char *str) {

//  char a[]="7.7";
  char *endptr = (char*)!0;	// no-null
  u_int32_t h=0,m=0,s=0,ms=0;

  s = strtol(str,&endptr,10);
  if (str==endptr) 
    throw qexception(__PRETTY_FUNCTION__,_("invalid time specification"));

  str = endptr;

  if (*str==':') {
    str++;
    m = s;  
    s = strtol(str,&endptr,10);
    if (str==endptr)
      throw qexception(__PRETTY_FUNCTION__,_("invalid time specification"));
    str = endptr;
  }
  
  if (*str==':') {
    str++;
    h = m;  
    m = s;  
    s = strtol(str,&endptr,10);
    if (str==endptr)
      throw qexception(__PRETTY_FUNCTION__,_("invalid time specification"));
    str = endptr;
  }
  
//  printf("%g\n",strtod(a,&endptr));

  if (*str=='.') {
    *--str='0';
//    printf("%s\n",str);
//    printf("%g %g %d\n",strtod(str,&endptr),1000*strtod(str,&endptr),(unsigned int)rint(strtod(str,&endptr)*1000LL));
    ms = (u_int32_t)rint(strtod(str,&endptr)*1000LL);
//    printf("%g %d\n",strtod(str,&endptr),ms);
    if (str==endptr)
      throw qexception(__PRETTY_FUNCTION__,_("invalid time specification"));
    str=endptr;
  }

  if (*str)
    throw qexception(__PRETTY_FUNCTION__,_("invalid time specification"));
  else
    return 3600000*h + 60000*m + 1000*s + ms;
}

u_int32_t time2msec (char *str) {

  char *index;
  char *secure;

  u_int32_t milliseconds = 0;

 // sure there aren't invalid chars
  if (strlen(str)!=strspn(str,"0123456789:."))
    throw qexception(__PRETTY_FUNCTION__,
                     string("invalid char in ")+string(str));

 // look for milliseconds part
  index = strrchr(str,'.');
  if (index) {
    *index=0;
    milliseconds += (u_int32_t) strtod (index+1,&secure);
    if (*secure!=0)
      throw qexception(__PRETTY_FUNCTION__,
                       string("invalid char in ")+string(index+1));
  }

  if (!strlen(str))
    return milliseconds;

 // look for seconds part  
  index = strrchr(str,':');
  if (index) {
    *index=0;
    milliseconds += 1000 * (u_int32_t) strtod (index+1,&secure);
    if (*secure!=0)
      throw qexception(__PRETTY_FUNCTION__,
                       string("invalid char in ")+string(index+1));
  }
  else {
    milliseconds += 1000 * (u_int32_t) strtod (str,&secure);
    if (*secure!=0)
      throw qexception(__PRETTY_FUNCTION__,
                       string("invalid char in ")+string(index+1));
    return milliseconds;
  }

cerr << "time2sec: ms=" << milliseconds << endl;

 // look for minutes part  
  index = strrchr(str,':');
  if (index) {
    *index=0;
    milliseconds += 60 * 1000 * (u_int32_t) strtod (index+1,&secure);
    if (*secure!=0)
      throw qexception(__PRETTY_FUNCTION__,
                       string("invalid char in ")+string(index+1));
  }
  else {
    milliseconds += 60 * 1000 * (u_int32_t) strtod (str,&secure);
    if (*secure!=0)
      throw qexception(__PRETTY_FUNCTION__,
                       string("invalid char in ")+string(index+1));
    return milliseconds;
  }

cerr << "time2sec: ms=" << milliseconds << endl;

 // look for hours part
  milliseconds += 60 * 60 * 1000 * (u_int32_t) strtod (str,&secure);
  if (*secure!=0)
    throw qexception(__PRETTY_FUNCTION__,
                       string("invalid char in ")+string(index+1));

  return milliseconds;
}


string uint2string (u_int32_t ui) {

  char buf[16];

  sprintf(buf,"%u",ui);

  return string(buf);
}

string char2string (char c) {

  char buf[2];

  sprintf(buf,"%c",c);

  return string(buf);
}


bool strcmpn (char *str1, char *str2, u_int32_t n) {

  while (n && (*str1==*str2)) {
    str1++;
    str2++;
    n--;
  }

  return n==0;
}


string htmlize (string str) {

  int pos;

  while ((pos = str.find(" "))!=-1)
    str.replace(pos,1,"%20");

  return str;
}
    
