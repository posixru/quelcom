# include <getopt.h>	// getopt
# include "qwav.hh"
# include "qexception.hh"

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif


void usage () {

  cerr.form(_(" %s: join wav files\n"),APPNAME);
  cerr.form(_(" syntax: %s [option]... file1 file2...\n"),APPNAME);
  cerr << _("   -h, --help: show this help and exit\n");
  cerr << _("   -o, --output <file>: send output to <file>. otherwise, append to <file1>\n");
  cerr << _("   -v, --verbose: verbose\n");
  cerr << _("   -V, --version: show version and exit\n");
}
     

int main (int argc, char **argv) {

  bool verbose=false;
  string outfile;

  static struct option long_options[] = {
    {"help",no_argument,0,'h'},
    {"output",required_argument,0,'o'},
    {"verbose",no_argument,0,'v'},
    {"version",no_argument,0,'V'},
    {0,0,0,0}
  };

#ifdef NLS
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);
#endif

  if (argc==1) {
    usage();
    return 1;
  }

  int option;

 // supress getopt error message
  opterr = 0;	

  while ((option = getopt_long(argc, argv, "ho:vV",long_options,0)) != EOF) 
    switch (option) {
      case 'h':
        usage();
        return 0;
      break;
      case 'o': 
        outfile=optarg; 
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

  if (argc-optind<2) {
    cerr << APPNAME << _(": at least two files to join must be specified\n");
    usage();
    return 1;
  }

  argv += optind;

  qwav *wav;

  try {

   // resolve which is the first file
    if (outfile!="") {
      wav = new qwav(*argv);
      if (verbose)
        cerr.form(_("copying '%s' to '%s'..."),*argv,outfile.c_str());
      wav->dup(outfile);
      if (verbose)
        cerr << "ok." << endl;
      delete wav;
      wav = new qwav (outfile,qwav::READWRITE);
    }
    else
      wav = new qwav(*argv,qwav::READWRITE);

   // append the rest of the files
    while (*++argv) {
      qwav ww(*argv);

      if (verbose)
        cerr.form(_("appending '%s' to '%s'..."),
                  ww.getName().c_str(),wav->getName().c_str());
      wav->append(ww);
      if (verbose)
        cerr << "ok." << endl;
    }
  }
  catch (qexception e) {
    cerr << *argv << ": " << e << endl;
    return 1;
  }

  if (verbose)
    cerr << *wav << endl;

  delete wav;
  return 0;
}
