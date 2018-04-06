# include <getopt.h>	// getopt
# include "qwav.hh"
# include "qwavsample.hh"
# include "qexception.hh"
# include "qmisc.hh"

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif

void usage () {

  cerr.form(_(" %s: fade in/out wav files\n"),APPNAME);
  cerr.form(_(" syntax: %s [option]... file...\n"),APPNAME);
  cerr << _("   -d, --duration <duration>[j|s|m|b|k|M]: set the fade duration\n");
  cerr << _("   -h, --help: show this help and exit\n");
  cerr << _("   -i, --in: just fade in\n");
  cerr << _("   -l, --length [[h:]m:]s[.ms]: set the fade duration.\n");
//  cerr << _("   -m, --mode <mode>: select the type of fade {linear,quadpos,quadneg} \n");
  cerr << _("   -o, --out: just fade out\n");
  cerr << _("   -t, --test: don't modify. create and fade a test file\n");
  cerr << _("   -v, --verbose: show more detailed info\n");
  cerr << _("   -V, --version: show version and exit\n");
  cerr << _(" see info manual for more info about options and usage\n");
}

 
int main (int argc, char **argv) {

  int option;
  qvf duration(5,qvf::SECONDS);
  bool in=true,out=true,verbose=false,test=false;
  

  static struct option long_options[] = {
    {"duration",required_argument,0,'d'},
    {"help",no_argument,0,'h'},
    {"in",no_argument,0,'i'},
    {"length",required_argument,0,'l'},
//    {"mode",required_argument,0,'m'},
    {"out",no_argument,0,'o'},
    {"test",no_argument,0,'t'},
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

  while ((option = getopt_long(argc, argv, "d:hil:otvV",long_options,0)) != EOF) 
    switch (option) {
      case 'd':
        try { duration = qvf(optarg); }
        catch (qexception e) { cerr << e << endl; }
      break;  
      case 'h':
        usage();
        return 0;
      break;
      case 'i':
        in=true; 
        out=false;
      break;
      case 'l':
        try { 
          duration = qvf(time2ms(optarg),qvf::MILLISECONDS); 
        }
        catch (qexception e) { cerr << e << endl; }
      break;  
      case 'o':
        out=true;
        in=false; 
      break;
      case 't':
	test = true;
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

  u_int32_t wavmode = test?qwav::READ:qwav::READWRITE;

  while (argv[optind]) {
  
    if (verbose)
      cerr.form(_("fading '%s'...\n"),argv[optind]);

    qwav *wav = new qwav(argv[optind],wavmode);
    u_int32_t nsamples = wav->getSample(duration);	// dins del rang?
  
    if (in && out && nsamples>wav->getSamples()/2) {
      cerr << APPNAME << _(": both fades overlap. they'd be done separately\n");
      optind++;
      continue;
    }
  
    if (in) {	// fade in:
  
      if (test) {
  
        string name(string("fadein.")+wav->getName());
        qvf size(nsamples+5*wav->getSampleRate());
  
        qcuthandler h;
        h.setOutfile(name);
        h.setSize(size);
        wav->cut(h);
        
        if (verbose)
          cerr.form(_("created testfile '%s'\n"),name.c_str());

        delete wav;
        wav = new qwav(name,qwav::READWRITE);
      } 
  
      qwavsample sample(wav);
      double factor;
      for (u_int32_t i=0; i<nsamples; i++) {
        factor = (double)i/(double)nsamples;
        sample.prod(factor);
        sample.setNext();
      }

      if (verbose)
        cerr.form(_("faded in %d samples in '%s'\n"),nsamples,wav->getName().c_str());
    }
  
    delete wav;
    wav = new qwav(argv[optind],wavmode);
  
    if (out) {	// fade out
  
      if (test) {
  
        string name(string("fadeout.")+wav->getName());
        qvf begin(nsamples+5*wav->getSampleRate());
  
        qcuthandler h;
        h.setOutfile(name);
        h.setbegin(begin);
        wav->cut(h);
  
        if (verbose)
          cerr.form(_("created testfile '%s'\n"),name.c_str());

        delete wav;
        wav = new qwav(name,qwav::READWRITE);
      } 
  
      qwavsample sample(wav,wav->getSamples()-nsamples+1);
      double factor;
      for (int i=nsamples; i>0; i--) {
        factor = (double)i/(double)nsamples;
        sample.prod(factor);
        sample.setNext();
      }

      if (verbose)
        cerr.form(_("faded out %d samples in '%s'\n"),nsamples,wav->getName().c_str());
    }

    optind++;
  }
}
