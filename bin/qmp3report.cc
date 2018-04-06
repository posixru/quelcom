# include <iostream>
# include <string>
# include <sys/stat.h>	// stat
# include <getopt.h>	// getopt
# include <dirent.h>	// scandir
# include <unistd.h>	// getcwd
# include <errno.h>	// errno
# include "qmp3.hh"
# include "qfile.hh"
# include "qexception.hh"
# include "qreport.hh"
# include "qmisc.hh"

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif

struct options {
  bool all, html, recursive, 
       showall, showdirs, showfiles, showsummary, 
       split, verbose;
  string outfilename;
  ofstream *out;
};


void usage () {

  cerr.form(_(" %s: generate reports from mp3 files and directories\n"),APPNAME);
  cerr.form(_(" syntax: %s [option]... file...\n"),APPNAME);
  cerr << _(" options:\n");
  cerr << _("   -a, --all-files: report all files, not just *.mp3\n");
  cerr << _("   -A, --show-all: implies -d, -f, -s\n");
//  cerr << _("   --all-mp3: consider files as mp3 streams"\n");
  cerr << _("   -d, --dirs: show a report for every directory\n");
  cerr << _("   -f, --files: show a report for every file\n");
  cerr << _("   -h, --help: show this help and exit\n");
  cerr << _("   -H, --html: output in html format (default is text)\n");
//  cerr << _("   -o, --outfilename <file>: output file (by default stdout)\n");
  cerr << _("   -r, --recursive: scan directories\n");
//  cerr << _("   -s: scan streams for validity (slower but more accurate)\n");
  cerr << _("   -s, --summary: show a summary report\n");
  cerr << _("   -S, --split: split report across visited directories\n");
  cerr << _("   -v, --verbose: show more detailed info\n");
  cerr << _("   -V, --version: show version and exit\n");
// allow user defined format for showing
// treure segons com i quan el temps que triga
}
     

qreport report_file (string filename, struct options &opts) {

  try {
    if (opts.all) {
      qfile f(filename);
      if (opts.verbose)
        cerr.form (_("reporting file '%s'\n"),filename.c_str());
      qreport r(f);
      if (opts.showfiles) {
        if (opts.html) {
          r.html(opts.out);
          *opts.out << "<br>" << endl;
        }
        else
          *opts.out << r << endl;
      }
      return r;
    }
  }
  catch (qexception e) {
    cerr << filename << ": " << e << endl;
  }

  return qreport();
}


qreport report_mp3 (string mp3name, struct options &opts) {

  try {
    qmp3 mp3(mp3name);
    if (mp3.isVbr()) {
      if (opts.verbose)
        cerr << mp3.getName() << _(": vbr detected => automatic check") << endl;
      mp3.scan();
    }
    if (opts.verbose)
      cerr.form (_("reporting mp3 file '%s'\n"),mp3name.c_str());
    qreport r(mp3);
    if (opts.showfiles) {
      if (opts.html) {
        r.html(opts.out);
        *opts.out << "<br>" << endl;
      }
      else
        *opts.out << r << endl;
    }
    return r;
  }
  catch (qexception e) {
    cerr << mp3name << ": " << e << endl;
    return qreport();
  }
}

qreport report_directory (string dirname, struct options &opts) {

  if (opts.verbose)
    cerr.form (_("reporting directory '%s'\n"),dirname.c_str());

  qreport dirreport(dirname);

  if (!opts.recursive) {
    if (opts.verbose)
      cerr.form(_("%s: '%s' is a directory\n"),APPNAME,dirname.c_str());
    return dirreport;
  }

  char previous_wd[1025],wd[1025];

  ofstream *out;
  if (opts.split) {
    if (getcwd(previous_wd,sizeof(previous_wd))==NULL) {
      cerr.form(_("cannot get current directory (%s): %s\n"),
                dirname.c_str(),strerror(errno));
      return dirreport;
    }
    if (chdir(dirname.c_str())) {
      cerr.form(_("cannot move to dir %s: %s\n"),dirname.c_str(),strerror(errno));
      return dirreport;
    }
    if (getcwd(wd,sizeof(wd))==NULL) {
      cerr.form(_("cannot get current directory (%s): %s\n"),
                dirname.c_str(),strerror(errno));
      return dirreport;
    }
    char *index = strrchr(wd,'/');
    string outfilename;
    if (*index && index!=wd)
      outfilename = string((index+1));
    else
      outfilename = string ("qmp3report");
    if (opts.html)
      outfilename += string(".html");
    else
      outfilename += string(".txt");
    
    if (opts.verbose)
      cerr.form(_("opening output file '%s'\n"),outfilename.c_str());

    out = opts.out;
    opts.out = new ofstream(outfilename.c_str());

    dirname = string(".");
  } 

  int n;
  struct dirent **namelist;

  if ((n = scandir(dirname.c_str(),&namelist,0,alphasort))<0) {
    cerr.form(_("%s: cannot scan '%s'\n"),APPNAME,dirname.c_str());
    return dirreport;
  }

  for (int i=0; i<n; i++) {

   // skip infinite recursion on the same directory or on the parent directory
    if (!strcmp(namelist[i]->d_name,".") || 
        !strcmp(namelist[i]->d_name,"..")) 
      continue;

    struct stat filestat;
    string filepathname;
    if (dirname==".")
      filepathname = string(namelist[i]->d_name);
    else
      filepathname = dirname + string("/") + string(namelist[i]->d_name);

    if (stat(filepathname.c_str(),&filestat)) {
      cerr.form(_("%s: error reading '%s' status: %s\n"),
                APPNAME,dirname.c_str(),strerror(errno));
      continue;
    }

    if (S_ISDIR(filestat.st_mode)) {
      dirreport.add(report_directory(filepathname,opts));
      continue;
    }

    if (S_ISREG(filestat.st_mode)) {
      int length = filepathname.length();
      if (length>4 && !strncasecmp((char*)(&(filepathname.c_str()[length-4])),".mp3",4))
        dirreport.add(report_mp3(filepathname,opts));
      else {
        if (opts.all)
          dirreport.add(report_file(filepathname,opts));
      }
      continue;
    }

    if (opts.verbose)
      cerr << filepathname << _(" is a special file. skipping.") << endl;
  }

  if (opts.split) {

    if (opts.html)
      *opts.out << "&nbsp<br>" << endl;
    else
      *opts.out << endl;

    *opts.out << dirreport << endl;

    opts.out->close();
    opts.out = out;
    chdir (previous_wd);
  }

  if (opts.showdirs && dirreport.getFiles()>0) {
    if (opts.html) {
     // if split fer que en lloc d'assenyalar al directori apunti al report
     //  que conté.
      dirreport.html(opts.out,dirreport.getName()+string("/")+dirreport.getName()+string(".html"));
      *opts.out << "<br>" << endl;
    }
    else
      *opts.out << dirreport << endl;
  }

  return dirreport;
}


int main (int argc, char **argv) {

  int option;
  struct options opts={false,false,false,false,false,false,false,false,false,""};

  static struct option long_options[] = {
    {"all-files",no_argument,0,'a'},
    {"show-all",no_argument,0,'A'},
//    {"all-mp3",no_argument,0,'Q'},	??????
    {"dirs",no_argument,0,'d'},
    {"files",no_argument,0,'f'},
    {"help",no_argument,0,'h'},
    {"html",no_argument,0,'H'},
    {"recursive",no_argument,0,'r'},
    {"summary",no_argument,0,'s'},
    {"split",no_argument,0,'S'},
    {"verbose",no_argument,0,'v'},
    {"version",no_argument,0,'V'},
    {0,0,0,0}
  };

#ifdef NLS
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);
#endif

 // un altre dia, per defecte llegir *.mp3 ...
  if (argc==1) {
    usage();
    return 1;
  }

 // supress getopt error message
  opterr = 0;	

  while ((option = getopt_long(argc, argv, "aAdfhHo:rsSvV",long_options,0)) != EOF) 
    switch (option) {
      case 'a':	opts.all = true;	break;
      case 'A': opts.showdirs=opts.showfiles=opts.showsummary = true;	break;
      case 'd': opts.showdirs = true;	break;
      case 'f': opts.showfiles = true;	break;
      case 'h': usage(); return 0;	break;
      case 'H': opts.html = true;	break;
      case 'r': opts.recursive = true;	break;
      case 's': opts.showsummary = true;break;
      case 'S': opts.split = true;	break;
      case 'v': opts.verbose = true;	break;
      case 'V': 
        cerr.form(_("%s - version %s - build %s\n"),APPNAME,VERSION,__DATE__);
        return 0;
      break;
      case '?':
      default: 
        cerr.form(_("%s: option '%s' is not recognized or bad used\n"),
                  APPNAME,argv[optind-1]);
        usage();
        return 1;
    } 

  if (argc<=optind) {
   // hauríem de fer *.mp3 ...
    cerr << APPNAME << _(": no input file(s)") << endl;
    usage();
    return 1;
  }

  qreport base;
  
  opts.out = new ofstream(1);

  if (opts.split)
    opts.out = new ofstream("/dev/null");
  else {
    opts.out = new ofstream(1);
    if (opts.html)
      *opts.out << "<html>\n<head><title>qmp3report</title>\n</head>\n\n<body>\n";
  }

  for (; optind<argc; optind++) {

    struct stat filestat;

    if (stat(argv[optind],&filestat)) {
      cerr.form(_("%s: error reading '%s' status: %s\n"),
                APPNAME,argv[optind],strerror(errno));
      continue;
    }

    try { 

      if (S_ISDIR(filestat.st_mode)) {
        base.add(report_directory(argv[optind],opts));
        continue;
      }

      if (S_ISREG(filestat.st_mode)) {

        int length = strlen(argv[optind]);

        if (length>4 && !strncasecmp((char*)(&(argv[optind][length-4])),".mp3",4))
          base.add(report_mp3(argv[optind],opts));
        else {
          if (opts.all)
            base.add(report_file(argv[optind],opts));
        }

        continue;
      }

      if (opts.verbose)
        cerr << argv[optind] << _(" is a special file. skipping.") << endl;
    }
    catch (qexception e) {
      cerr << e << endl;
    }
  }

  if (opts.showsummary) {
    if (!opts.html || opts.split) {
      cout << base << endl;
    }
    else {
      base.html(new ofstream(1));
      cout << "<br>" << endl;
    }
  }

  if (!opts.split) {

    if (opts.html) {
      *opts.out << "\n</body></html>\n";
    }
  }

  opts.out->close();
}  

