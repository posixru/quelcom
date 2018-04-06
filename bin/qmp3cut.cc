# include <getopt.h>	// getopt
# include "qmp3.hh"
# include "qexception.hh"

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif

void usage () {

  cerr.form(_(" %s: extract and/or delete parts of a mp3 file\n"),APPNAME);
  cerr.form(_(" syntax: %s [option]... file\n"),APPNAME);
  cerr << _(" options:\n");
  cerr << _("   -B, --set-begin <cut point>: set the first frame from the beginning of the file\n");
  cerr << _("   -b, --set-begin-from-eof <cut point>: set the first frame from the end of the file\n");
  cerr << _("   -d, --delete: deletes the fragment\n");
  cerr << _("   -E, --set-end <cut point>: set the last frame from the beginning of the file\n");
  cerr << _("   -e, --set-end-from-eof <cut point>: set the last frame from the end of the file\n");
  cerr << _("   -h, --help: show this help and exit\n");
  cerr << _("   -o, --output <file>: output file where the fragment is stored\n");
  cerr << _("   -s, --size <cut point>: specifies the size of the cut\n");
  cerr << _("   -S, --slice <time spec>-<time spec>: set the cut as a time slice\n");
  cerr << _("   -v, --verbose: verbose\n");
  cerr << _("   -V, --version: show version and exit\n");
  cerr << _(" <cut point> = <value>[j|s|m|b|k|M]\n");
  cerr << _(" <time spec> = [[h:]m:]s[.ms]\n");
  cerr << _(" see info manual for more info about options and usage\n");
}
     

int main (int argc, char **argv) {

  string outfile;
  string infile;
  bool del = false, verbose=false, slice_defined = false;
  qvf vfbegin,vfSize,vfend,vfBegin,vfEnd; 

  static struct option long_options[] = {
    {"set-begin",required_argument,0,'B'},
    {"set-begin-from-eof",required_argument,0,'b'},
    {"delete",no_argument,0,'d'},
    {"set-end",required_argument,0,'E'},
    {"set-end-from-eof",required_argument,0,'e'},
    {"help",no_argument,0,'h'},
    {"output",required_argument,0,'o'},
    {"size",required_argument,0,'s'},
    {"slice",required_argument,0,'S'},
    {"verbose",no_argument,0,'v'},
    {"version",no_argument,0,'V'},
    {0,0,0,0}
  };

#ifdef NLS
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);
#endif

  int option;
  qcuthandler h;

 // supress getopt error messages:
  opterr = 0;

  while ((option = getopt_long(argc, argv, "b:B:de:E:ho:s:S:vV",long_options,0)) != EOF) 
    try {
      switch (option) {
        case 'b':
          vfbegin = qvf(optarg);
        break;  
        case 'B':
          vfBegin = qvf(optarg);
        break;  
        case 'd':
          del=true;
        break;
        case 'e':
          vfend = qvf(optarg);
        break;  
        case 'E':
          vfEnd = qvf(optarg);
        break;  
        case 'h':
          usage();
          return 1;
        break;
        case 'o': 
          outfile=optarg; 
        break;
        case 's':
          vfSize = qvf(optarg);
        break;  
        case 'S':
          slice_defined = true;
          h = qcuthandler(optarg);
        break;
        case 'v':
          verbose=true;
        break;
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
    }
    catch (qexception e) {
      cerr << e << endl;
    }

  if (argc-optind!=1) {
    cerr << APPNAME << _(": only one file to cut must be specified") << endl;
    usage();
    return 1;
  }
  
  infile = argv[optind];

  if (!slice_defined) {
    h.setBegin(vfBegin);
    h.setbegin(vfbegin);
    h.setEnd(vfEnd);
    h.setend(vfend);
  }
  h.setSize(vfSize);
  h.setDel(del);
  h.setOutfile(outfile);

  try {
    u_int32_t mode = del?qmp3::READWRITE:qmp3::READ;
    qmp3 mp3 (infile,mode); 

    if (verbose)
      cerr.form(_("scanning '%s'..."),infile.c_str());
    mp3.scan();
    if (verbose)
      cerr << "ok." << endl;

    mp3.scan();
    if (mp3.isVbr()) {
      cerr << APPNAME << _(": vbr format is not supported") << endl;
      return 1;
    }

    mp3.cut(h);

    if (verbose) {
      cerr << mp3 << endl;
      if (outfile!="") {
        qmp3 mp3out(outfile);
        cerr << mp3out << endl;
      }
    }
  }
  catch (qexception e) {
    cerr << e << endl;
    return 1;
  }

  return 0;
}
