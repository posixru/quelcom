# include <getopt.h>	// getopt
# include <sys/types.h>

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

  cerr.form(_(" %s: show info from wav files\n"),APPNAME);
  cerr.form(_(" syntax: %s [option]... file...\n"),APPNAME);
  cerr << _("   -h, --help: show this help and exit\n");
  cerr << _("   -s, --summary-only: show only the summary\n");
  cerr << _("   -v, --verbose: verbose\n");
  cerr << _("   -V, --version: show version and exit\n");
}


int main (int argc, char **argv) {

  int option;
  bool verbose=false;
  bool summarize=false;

  static struct option long_options[] = {
    {"help",no_argument,0,'h'},
    {"summary-only",no_argument,0,'s'},
    {"verbose",no_argument,0,'v'},
    {"version",no_argument,0,'V'},
    {0,0,0,0}
  };

#ifdef NLS
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);
#endif

 // un altre dia, per defecte llegir *.wav ...
  if (argc==1) {
    usage();
    return 1;
  }

 // supress getopt error message
  opterr = 0;	

  while ((option = getopt_long(argc, argv, "hsvV",long_options,0)) != EOF) 
    switch (option) {
      case 'h':
        usage();
        return 0;
      break;
      case 's':
        summarize=true;
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

  if (argc<=optind) {
   // hauríem de fer *.wav ...
    cerr << APPNAME << _(": no input file(s)") << endl;
    usage();
    return 1;
  }

  u_int32_t files=0, errors=0;
  u_int32_t msduration, bytes, samples;
  u_int32_t total_bytes=0, total_samples=0, total_msduration=0;

  while (argv[optind]) {

    files++;

    try {

      qwav p(argv[optind]);

      msduration = p.getMsDuration();
      total_msduration += msduration;

      if (!summarize)
        cout << p;

      if (verbose) {
        bytes = p.getSize();
        samples = p.getSamples();

        if (!summarize)
          cout << "  " <<  bytes << " bytes  " << samples << " samples";

        total_bytes += bytes;
        total_samples += samples;
      }             

      if (!summarize)
        cout << endl;
    }
    catch (qexception e) {
      cerr << argv[optind] << ": " << e << endl;
      errors++;
    }

    optind++;
  }

  cout.form (_("%u file%c"),files,files>1?'s':0);
  if (errors)
    cout.form (" (%u error%c)",errors,errors>1?'s':0);
  if (verbose)
    cout.form (_(" => %d:%02d.%03d  %u samples  %u bytes\n"),
               total_msduration/60000,(total_msduration/1000)%60,
               total_msduration%1000,total_samples,total_bytes);
  else
    cout.form (" => %d:%02d\n",
               total_msduration/60000,(total_msduration/1000)%60);
}
