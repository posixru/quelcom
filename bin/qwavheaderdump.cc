# include <getopt.h>	// getopt
# include <sys/types.h>	// u_int16_t, u_int32_t
# include "qfile.hh"
# include "qexception.hh"

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif

// see qwavheader.hh for more info about the wav header struct
struct header {

  char riff[4];
  u_int32_t rifflength;
  char wave[4];
     
  char fmt_[4];
  u_int32_t fmtlength;
  u_int16_t format;
  u_int16_t channels;
  u_int32_t samplerate;
  u_int32_t bytespersec;
  u_int16_t bytespersample;
  u_int16_t bitspersample;
    
  char data[4];
  u_int32_t datalength;
};

const unsigned int HEADERSIZE = sizeof(struct header);


void usage () {

  cerr.form(_(" %s: dump (and fix) wav header\n"),APPNAME);
  cerr.form(_(" syntax: %s [option]... file...\n"),APPNAME);
  cerr << _("   -F, --fix: correct header. use with care\n");
  cerr << _("   -h, --help: show this help and exit\n");
  cerr << _("   -q, --quiet: no output messages\n");
  cerr << _("   -V, --version: show version and exit\n");
}


int main (int argc, char **argv) {

  int option;
  bool quiet=false, fix=false;

  static struct option long_options[] = {
    {"fix",no_argument,0,'F'},
    {"help",no_argument,0,'h'},
    {"quiet",no_argument,0,'q'},
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

  while ((option = getopt_long(argc, argv, "FhqV",long_options,0)) != EOF) 
    switch (option) {
      case 'F':
        fix=true;
      break;
      case 'h':
        usage();
        return 0;
      break;
      case 'q':
        quiet=true;
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

  u_int32_t openmode = fix?qfile::READWRITE:qfile::READ;

  while (argv[optind]) {

    try {

      qfile f(argv[optind],openmode);
      
      if (f.getSize()<HEADERSIZE) {
        cerr.form(_("%s: '%s' has not enough room for a wav header\n"),
                  APPNAME,f.getName().c_str());
        optind++;
        continue;
      }

      struct header *header = (struct header*) f.getMap();

      cout << f.getName() << " (" << f.getSize() << " bytes):" << endl;

     // the file should have an integer number of samples...

      cout.form("\triff: '%.4s'\n",header->riff);
      if (strncmp(header->riff,"RIFF",4)) {
        if (!quiet) 
          cerr.form(_("\t\triff field should be 'RIFF'\n"));
        if (fix) {
          strncpy(header->riff,"RIFF",4);
          if (!quiet)
            cerr << _("\t\tfixed\n");
        }
      }
      
      cout.form(_("\triff length: %d\n"),header->rifflength);
      if (header->rifflength!=f.getSize()-8) {
        if (!quiet)
          cerr.form(_("\t\triff length field should be %d\n"),f.getSize()-8);
        if (fix) {
          header->rifflength = f.getSize()-8;
          if (!quiet)
            cerr << _("\t\tfixed\n");
        }
      }

      cout.form("\twave: '%.4s'\n",header->wave);
      if (strncmp(header->wave,"WAVE",4)) {
        if (!quiet) 
          cerr.form(_("\t\twave field should be 'WAVE'\n"));
        if (fix) {
          strncpy(header->wave,"WAVE",4);
          if (!quiet)
            cerr << _("\t\tfixed\n");
        }
      }
      
      cout.form("\tfmt: '%.4s'\n",header->fmt_);
      if (strncmp(header->fmt_,"fmt ",4)) {
        if (!quiet) 
          cerr.form(_("\t\tfmt  field should be 'fmt '\n"));
        if (fix) {
          strncpy(header->fmt_,"fmt ",4);
          if (!quiet)
            cerr << _("\t\tfixed\n");
        }
      }

      cout.form(_("\tfmt length: %d\n"),header->fmtlength);
      if (header->fmtlength!=16) {
        if (!quiet)
          cerr.form(_("\t\tfmt length field should be %d\n"),16);
        if (fix) {
          header->fmtlength = 16;
          if (!quiet)
            cerr << _("\t\tfixed\n");
        }
      }

      cout.form("\tformat: %d\n",header->format);
      if (header->format!=1) {
        if (!quiet)
          cerr.form(_("\t\tformat field should 1 (pcm tag)\n"),16);
        if (fix) {
          header->format = 1;
          if (!quiet)
            cerr << _("\t\tfixed\n");
        }
      }

      cout.form(_("\tchannels: %d\n"),header->channels);
      if (header->channels!=2 && header->channels!=1) {
        if (!quiet)
          cerr.form(_("\t\tchannels field should be 1 (mono) or 2 (stereo)\n"));
        if (fix) {
          cerr << _("\t\tdon't know which value must be set...\n") << endl;
          if (!quiet)
            cerr << _("\t\tfixed\n");
        }
      }

      cout.form(_("\tsample rate: %d\n"),header->samplerate);
      if (header->samplerate>48000 || header->samplerate<8000) {
        if (!quiet)
          cerr.form(_("\t\tsample rate field should be between 8000 and 48000\n"));
        if (fix)
          cerr << _("\t\tdon't know which value must be set...\n") << endl;
      }

      cout.form(_("\tbytes/second: %d\n"),header->bytespersec);


      cout.form(_("\tbytes/sample: %d\n"),header->bytespersample);
      if (header->bytespersample!=1 &&
          header->bytespersample!=2 &&
          header->bytespersample!=4) {
        if (!quiet)
          cerr.form(_("\t\t bytes/sample field should be 1, 2 or 4\n"));
        if (fix)
          cerr << _("\t\tdon't know which value must be set...\n") << endl;
      }

      cout.form(_("\tbits/sample: %d\n"),header->bitspersample);

      cout.form("\tdata: '%.4s'\n",header->data);
      if (strncmp(header->data,"data",4)) {
        if (!quiet) 
          cerr.form(_("\t\tdata field should be 'data'\n"));
        if (fix) {
          strncpy(header->data,"data",4);
          if (!quiet)
            cerr << _("\t\tfixed\n");
        }
      }

      cout.form(_("\tdata length: %d\n"),header->datalength);
      if (header->datalength!=f.getSize()-44) {
        if (!quiet)
          cerr.form(_("\t\tdata length field should be %d\n"),f.getSize()-44);
        if (fix) {
          header->datalength = f.getSize()-44;
          if (!quiet)
            cerr << _("\t\tfixed\n");
        }
      }
    }
    catch (qexception e) {
      cerr << argv[optind] << ": " << e << endl;
    }

    optind++;
  }
}
