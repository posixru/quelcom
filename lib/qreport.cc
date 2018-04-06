/*! \file
 * implementation functions for class qreport
 */

# include <stdio.h>		// sprintf
# include <string>
# include "qreport.hh"
# include "qexception.hh"
# include "qmisc.hh"

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif

/*
  qreportar directoris visitats i directoris amb mp3
  qreportar fitxers visitats i fitxers mp3
*/

// create a base empty qreport
qreport::qreport () {

  type = BASE;
  files = directories = errors = 0;
  msduration = size = 0;
  
  bitrate = samplerate = 0;
}

// report a normal file
qreport::qreport (qfile &f) {

  type = FILE;
  samplerate = 0;
  bitrate = 0;
  msduration = 0;
  size = f.getSize();
  name = f.getName();
  files = 1; 
  directories = 0;
}

// report an mp3 file
qreport::qreport (qmp3 &mp3) {

  type = MP3;
  samplerate = mp3.getSampleRate();
  bitrate = mp3.getBitRate();
  msduration = mp3.getMsDuration();
  size = mp3.getSize();
  name = mp3.getName();
  if (vbr = mp3.isVbr())
    bitrate=(u_int32_t)(-1);
  files = 1; 
  directories = 0;
}

// report a directory
qreport::qreport (const string dir) {

  type = DIR;
  files = errors = 0;
  directories = 1;
  msduration = size = 0;

  bitrate = samplerate = 0;
  name = dir;
}


void qreport::add (qreport r) {

 /*
    provar:
      if (type==file) throw
      if (r.type==base) throw
      sumar
    i llavors implementar el '+='
*/

  switch (type) {
    case FILE: 
    case MP3:
      type = BASE;
    case DIR:
    case BASE:
      switch (r.type) {
        case BASE:
          return ; // do nothing
          // throw qexception(__PRETTY_FUNCTION__,"a base qreport cannot be added anywhere");
        case DIR:
	case MP3:
          if (bitrate==0)
            bitrate = r.bitrate;
          else 
            if (bitrate!=r.bitrate)
              bitrate = -1;
          if (samplerate==0)
            samplerate = r.samplerate;
          else 
            if (samplerate!=r.samplerate)
              samplerate = -1;
          msduration += r.msduration;
        case FILE:
          files += r.files;
          directories+=r.directories;
         size += r.size;
        break;
        default:
          throw qexception(__PRETTY_FUNCTION__,"invalid qreport type");
      }
    break;
    default:
      throw qexception(__PRETTY_FUNCTION__,"invalid qreport type");
  }

//  report_list.push_back(r);
}

//    add_error (string); // qreport an error


string qreport::parameter2string() {

  char str_duration[15], str_bitrate[10], str_size[15], str_files[15];
  
  if (msduration>0) {
    int duration = (int)(msduration/1000);
    sprintf(str_duration,"%3d:%02d:%02d",duration/3600,(duration%3600)/60,duration%60);
  }

  if (bitrate>0)
    sprintf(str_bitrate,"%3d kbps",bitrate);
  else
    sprintf(str_bitrate,"        ");

  if (size>0)
    sprintf(str_size,"%4.2f Mb",(double)size/(1024*1024));

  if (type==DIR)
    sprintf(str_files,"%d files",files);	// and dirs?
  else
    str_files[0]=0;	// and dirs?
   
  return string(str_duration) + string(" - ") + string(str_bitrate) + string(" - ") + string(str_size) + string(" - ") + string(str_files);
}

string qreport::getName() {

  return name;
}


void qreport::html(ostream *os, string link) {

  if (files==0 && directories==0) {
    *os << "[empty report]";
    return;
  }

//  if (msduration>0) {
   // no ho deixis així!!!!
    int dur = msduration;
    os->form("%3d:%02d:%02d",dur/3600000,(dur%3600000)/60000,(dur%60000)/1000);
//  }
//  else 
//    *os << "         ";

  *os << " - ";

  switch (bitrate) {
    case 0:               *os << "        "; break;
    case (u_int32_t)(-1): *os << "  [vbr] "; break;
    default:
      os->form("%3d kbps",bitrate);
  }

  *os << " - ";

/*
	si verbose...
  if (samplerate>0)
    os->form("%5d Hz  ",samplerate);
  else
    *os << "          ";
*/

//  if (size>0)
    os->form("%4.2f Mb",(double)size/(1024*1024));
//  else
//    *os << "          ";

  switch (type) {
    case BASE:
      *os << " - " << directories << " directories and " << files << " files";
    break;
    case FILE:
    case MP3:
      if (link=="")
        *os << " - " << "<a href=\"" << htmlize(name) << "\">" << name << "</a>";
      else
        *os << " - " << "<a href=\"" << htmlize(link) << "\">" << name << "</a>";
    break;
    case DIR:
      if (link=="")
        *os << " - " << "<a href=\"" << htmlize(name) << "\">" << name << "</a>";
      else
        *os << " - " << "<a href=\"" << htmlize(link) << "\">" << name << "</a>";
      if (directories>1)
        *os << " - " << directories-1 << " directories and " << files << " files";
      else
        *os << " - " << files << " files";
    break;
    default:
      *os << "quelcom internal error" << endl;
  }

return;

  switch (type) {
    case BASE:
      if (directories>0)
        *os << directories << _(" directories  "); 
      *os << files << _(" files  ");
    break;
    case FILE:
/*
      if (htmloutput)
        *os << "<a href=\"file://" << htmlize(name) << "\">" << name << "</a>: "; 
      else
*/
        os->form("%-64s",name.c_str());
    break;
    case MP3:
//      *os << "bliblablo";
        os->form("%-64s",name.c_str());
    break;
    case DIR:
/*
      if (htmloutput)
        *os << "<a href=\"file://" << htmlize(name) << "\">" << name << "</a>: "; 
      else
*/
        os->form("%-64s  ",name.c_str());
      if (directories>1) // other than itself
        *os << directories-1 << _(" directories  "); 
      os->form("%3d file%c  ",files,(files==1)?' ':'s');
    break;
  }

}


void qreport::print(ostream *os) {

  if (files==0 && directories==0) {
    *os << "[empty report]";
    return;
  }

//  if (msduration>0) {
   // no ho deixis així!!!!
    int dur = msduration;
    os->form("%3d:%02d:%02d",dur/3600000,(dur%3600000)/60000,(dur%60000)/1000);
//  }
//  else 
//    *os << "         ";

  *os << " - ";

// això s'hauria d'assegurar. pex: bitrate==(u_int32_t)(-1) => vbr
  switch (bitrate) {
    case 0:               *os << "        "; break;
    case (u_int32_t)(-1): *os << "  [vbr] "; break;
    default:
      os->form("%3d kbps",bitrate);
  }

  *os << " - ";

/*
	si verbose...
  if (samplerate>0)
    os->form("%5d Hz  ",samplerate);
  else
    *os << "          ";
*/

//  if (size>0)
    os->form("%4.2f Mb",(double)size/(1024*1024));
//  else
//    *os << "          ";

  switch (type) {
    case BASE:
      *os << " - " << directories << " directories and " << files << " files";
    break;
    case FILE:
    case MP3:
      *os << " - " << name;
    break;
    case DIR:
      *os << " - " << name;
      if (directories>1)
        *os << " - " << directories-1 << " directories and " << files << " files";
      else
        *os << " - " << files << " files";
    break;
    default:
      *os << "quelcom internal error" << endl;
  }

return;

  switch (type) {
    case BASE:
      if (directories>0)
        *os << directories << _(" directories  "); 
      *os << files << _(" files  ");
    break;
    case FILE:
/*
      if (htmloutput)
        *os << "<a href=\"file://" << htmlize(name) << "\">" << name << "</a>: "; 
      else
*/
        os->form("%-64s",name.c_str());
    break;
    case MP3:
//      *os << "bliblablo";
        os->form("%-64s",name.c_str());
    break;
    case DIR:
/*
      if (htmloutput)
        *os << "<a href=\"file://" << htmlize(name) << "\">" << name << "</a>: "; 
      else
*/
        os->form("%-64s  ",name.c_str());
      if (directories>1) // other than itself
        *os << directories-1 << _(" directories  "); 
      os->form("%3d file%c  ",files,(files==1)?' ':'s');
    break;
  }

}


ostream &operator<<(ostream &os, qreport &r) {
  r.print(&os);
  return os;
}

