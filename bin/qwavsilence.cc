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

  cerr.form(_(" %s: detect and shrink silence sequence in wav files\n"),APPNAME);
  cerr.form(_(" syntax: %s [option]... file...\n"),APPNAME);
  cerr << _("   -d, --duration <duration>[j|s|m|b|k|M]: set the minimum silence duration.\n");
  cerr << _("   -h, --help: show this help and exit\n");
  cerr << _("   -l, --length [[h:]m:]s[.ms]: set the minimum silence duration.\n");
  cerr << _("   -S, --shrink: shrink silences greater than 'duration'. use with care.\n");
  cerr << _("   -t, --threshold <threshold>: set the maximum value (in %) for silent samples\n");
  cerr << _("   -v, --verbose: show more detailed info\n");
  cerr << _("   -V, --version: show version and exit\n");
}


int main (int argc, char **argv) {

  int option;
  u_int32_t threshold=0;
  qvf duration(1,qvf::SECONDS);
  bool shrink=false, verbose=false;

  static struct option long_options[] = {
    {"duration",required_argument,0,'d'},
    {"help",no_argument,0,'h'},
    {"length",required_argument,0,'l'},
    {"shrink",no_argument,0,'S'},
    {"threshold",required_argument,0,'t'},
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

  double temp;

 // supress getopt error message
  opterr = 0;	

  while ((option = getopt_long(argc, argv, "d:hl:St:vV",long_options,0)) != EOF) 
    switch (option) {
      case 'd':
        try { duration = qvf(optarg); }
        catch (qexception e) { cerr << e << endl; }
      break;  
      case 'h':
        usage();
        return 0;
      break;
      case 'l':
        try { 
          duration = qvf(time2ms(optarg),qvf::MILLISECONDS); 
        }
        catch (qexception e) { cerr << e << endl; }
      break;  
      case 'S':
        shrink=true;
      break;
      case 't':
	temp = strtod(optarg,0);
        if (temp<0 || temp>100) {
          cerr.form(_("%s: threshold must be 0..100 (%d)\n"),APPNAME,temp);
          return 1;
        }
        threshold = (u_int32_t)temp;
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
  u_int32_t duration_samples, threshold_value;
  u_int32_t total_silent_samples, sample_number, shrinked_samples;

  while (argv[optind]) {

    files++;

    try {

      qwav wav(argv[optind],shrink?qwav::READWRITE:qwav::READ);

      duration_samples = wav.getSample(duration);
      
     // the % value depends on bits/sample        
      threshold_value = threshold*(1<<(wav.getBitsPerSample()-1))/100;

      sample_number = wav.getSamples();
      if (sample_number<1) {
        if (verbose)
          cerr << *argv << _(": empty") << endl;
        optind++; 
        continue;
      }
      if (sample_number<duration_samples) {
        if (verbose)
          cerr << *argv << _(": smaller than the silence searched for") << endl;
        optind++; 
        continue;
      }

      qwavsample *sample;

      u_int32_t sample_index = 1;
      u_int32_t silent_samples = 0;

      shrinked_samples = 0;
      u_int32_t first_shrinkable_sample;
      total_silent_samples = 0;

      while (sample_index+duration_samples <= sample_number) {

        sample = new qwavsample(&wav,sample_index);
        silent_samples=0;
        while (   sample_index<=sample_number 
               && sample->isSilence(threshold_value)) {

          silent_samples++;
          sample_index++;
          sample->setNext();
        }

        if (silent_samples) {

          if (silent_samples>=duration_samples) {

            u_int32_t mssec_begin, mssec_end;

            mssec_begin = 1000LL*(sample_index-silent_samples)/wav.getSampleRate();
            mssec_end = (1000LL*silent_samples)/wav.getSampleRate();

            if (!verbose)
              cout.form(_("%s: %d:%02d of silence at %d:%02d\n"),
                        argv[optind],
                        mssec_end/60000,(mssec_end/1000)%60,
                        mssec_begin/60000,(mssec_begin/1000)%60);
            else
              cout.form(_("%s: %d:%02d.%03d (%d samples) at %d:%02d:%03d (sample %d)\n"),
                        argv[optind],
                        mssec_end/60000,(mssec_end/1000)%60,mssec_end%1000,
			silent_samples,
                        mssec_begin/60000,(mssec_begin/1000)%60,mssec_begin%1000,
			sample_index-silent_samples);
                        
            total_silent_samples+=silent_samples;

            if (shrink) {
              u_int32_t shrinkable_samples = silent_samples - duration_samples;
              
              cerr.form(_("shrink %d samples at %d\n"),
                        shrinkable_samples,sample_index - shrinkable_samples - 1);
              if (shrinked_samples) {
                u_int32_t samples_moved = sample_index-shrinkable_samples-(first_shrinkable_sample+shrinked_samples+1);
                cerr.form(_("move %d..%d (%d samples) to %d..%d\n"),
                          first_shrinkable_sample+shrinked_samples+1,
                          sample_index-shrinkable_samples-1,
                          samples_moved,
                          first_shrinkable_sample,
                          first_shrinkable_sample+samples_moved);

                wav.move(first_shrinkable_sample+shrinked_samples+1,
                         sample_index-shrinkable_samples-1,
                         first_shrinkable_sample);
              }
              shrinked_samples += shrinkable_samples;
              first_shrinkable_sample = sample_index - shrinked_samples - 1;
            }
          }
          // else 
          //   if very-very-verbose ...
        }

       // skip non silent region by looking for non silent samples ahead
        u_int32_t test_index = duration_samples;
        while (sample_index+duration_samples <= sample_number
               && test_index>=1) { 

          qwavsample test_sample(&wav,sample_index+test_index);
          if (test_sample.isSilence(threshold_value))
            test_index >>= 1;
          else {
            sample_index += test_index;

            test_index = duration_samples;
          }
        }
        if (test_index==0)
          sample_index++;
        else {	// sortim del bucle pq s'ha acabat el fitxer...
          if (shrink && shrinked_samples) {
            u_int32_t samples_moved = sample_number-(first_shrinkable_sample+shrinked_samples+1);
            cerr.form(_("move %d..%d (%d samples) to %d..%d\n"),
                      first_shrinkable_sample+shrinked_samples+1,
                      sample_number,
                      samples_moved,
                      first_shrinkable_sample,
                      first_shrinkable_sample+samples_moved);
            wav.move(first_shrinkable_sample+shrinked_samples+1,
                     sample_number,
                     first_shrinkable_sample);
          }
        }
      }

      if (verbose) {
        u_int32_t mssec = 1000LL*total_silent_samples/wav.getSampleRate();
        cerr.form(_("%s: total silence reported: %d:%02d.%02d (%d samples)\n"),
                   argv[optind],mssec/60000,(mssec/1000)%60,(mssec%1000)/10,
                   total_silent_samples);
      }

      if (shrinked_samples) {
        wav.truncate(sample_number-shrinked_samples);
        if (verbose)
          cerr << "truncating down to " << sample_number-shrinked_samples << " samples" << endl;
      }
    }
    catch (qexception e) {
      cerr << argv[optind] << ": " << e << endl;
      errors++;
    }

    optind++;
  }

  if (errors)
    cout.form ("%u error%c\n",errors,errors>1?'s':0);
}
