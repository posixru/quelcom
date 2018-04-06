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

  cerr.form(_(" %s: join mp3 files\n"),APPNAME);
  cerr.form(_(" syntax: %s [option]... file1 file2...\n"),APPNAME);
  cerr << _("   -f, --force: force join bypassing bit rate checks\n");
  cerr << _("   -h, --help: show this help and exit\n");
  cerr << _("   -o, --output <file>: send output to <file>. otherwise, append to <file1>\n");
  cerr << _("   -v, --verbose: verbose\n");
  cerr << _("   -V, --version: show version and exit\n");
}
     

int main (int argc, char **argv) {

  bool verbose=false, force=false;
  string outfile;

  static struct option long_options[] = {
    {"force",no_argument,0,'f'},
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

  while ((option = getopt_long(argc, argv, "fho:vV",long_options,0)) != EOF) 
    switch (option) {
      case 'f':
        force = true;
      break;
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

  qmp3 *mp3;

  try {

   // resolve which is the first file
    if (outfile!="") {
      mp3 = new qmp3(*argv);
      if (verbose)
        cerr.form(_("copying '%s' to '%s'..."),*argv,outfile.c_str());
      mp3->dup(outfile);
      if (verbose)
        cerr << "ok." << endl;
      delete mp3;
      mp3 = new qmp3 (outfile,qmp3::READWRITE);
    }
    else
      mp3 = new qmp3(*argv,qmp3::READWRITE);

    if (verbose)
      cerr.form(_("scanning '%s'..."),mp3->getName().c_str());

    mp3->scan();
    if (verbose)
      cerr << "ok." << endl;

   // append the rest of the files
    while (*++argv) {
      qmp3 mm(*argv);
 
      if (verbose)
        cerr.form(_("scanning '%s'..."),mm.getName().c_str());
      mm.scan();
      if (verbose)
        cerr << "ok." << endl;

      if (verbose)
        cerr.form(_("appending '%s' to '%s'..."),
                  mm.getName().c_str(),mp3->getName().c_str());
      mp3->append(mm,force);
      if (verbose)
        cerr << "ok." << endl;
    }
  }
  catch (qexception e) {
    cerr << *argv << ": " << e << endl;
    return 1;
  }

  if (verbose)
    cerr << *mp3 << endl;

  delete mp3;
  return 0;
}
