# include <getopt.h>	// getopt
# include <stdio.h>	// per esborrar...
# include "qmp3frameheader.hh"
# include "qmp3.hh"
# include "qfile.hh"
# include "qtag.hh"
# include "qexception.hh"

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif

void usage () {

  cerr.form(_(" %s: check and clean mp3 streams\n"),APPNAME);
  cerr.form(_(" syntax: %s [option]... file...\n"),APPNAME);
  cerr << _("   -D, --delete: delete invalid frames and garbage. use with care\n");
  cerr << _("   -h, --help: show this help and exit\n");
  cerr << _("   -q, --quiet: no output messages\n");
  cerr << _("   -T, --delete-tag: delete tag (if exists). option -D must be set\n");
  cerr << _("   -v, --verbose: show more detailed info\n");
  cerr << _("   -V, --version: show version and exit\n");
}
     

int main (int argc, char **argv) {

  int option;
  bool verbose=false, del=false, delTag=false, quiet=false;

  static struct option long_options[] = {
    {"delete",no_argument,0,'D'},
    {"help",no_argument,0,'h'},
    {"quiet",no_argument,0,'q'},
    {"delete-tag",no_argument,0,'T'},
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

  while ((option = getopt_long(argc, argv, "DhqTvV",long_options,0)) != EOF) 
    switch (option) {
      case 'D': 
        del=true; 
      break;
      case 'h':
        usage();
        return 0;
      break;
      case 'q': 
        quiet=true; verbose=false;
      break;
      case 'T': 
        delTag=true; 
      break;
      case 'v': 
        verbose=true; quiet=false;
      break;
      case 'V':
        cerr.form(_("%s - version %s - build %s\n"),APPNAME,VERSION,__DATE__);
        return 0;
      default: 
        cerr.form(_("%s: option '%s' is not recognized or bad used\n"),
                  APPNAME,argv[optind-1]);
        usage();
        return 1;
    }

  if (delTag && !del) {
    cerr << APPNAME << _(": flag -T requires -D. ignoring -T") << endl;
    delTag = false;
  }

  if (argc<=optind) {
   // hauríem de fer *.mp3 ... ?
    cerr << APPNAME << _(": no input file(s)") << endl;
    usage();
    return 1;
  }

  argv += optind;

  u_int32_t error_files = 0;
  u_int32_t frames, stream_length;
  u_int32_t mode = del?qfile::READWRITE:qfile::READ;
  caddr_t clean_offset, clean_block_begin, clean_block_end;
  qmp3frameheader *frame;
  u_int32_t skipped, total_skipped;

  do {
    total_skipped = 0;
    try {

      qfile file(*argv,mode);

      bool hasTag; 
      qtag *tag;

      if (file.getSize()<qtag::LENGTH)
        hasTag = false;
      else {
        tag = new qtag(file);
        hasTag = tag->isValid();
        if (hasTag)
          cerr << *argv << ": " << *tag << endl;
        else
          if (verbose)
            cerr << *argv << _(": no tag") << endl;
      }
 
      frames = 0;
      stream_length = file.getSize()-(hasTag?qtag::LENGTH:0);

      try {
        frame = new qmp3frameheader(file.getMap(),file.getSize());
      }
      catch (qexception e) {
        cerr << file.getName() << ": " << e << endl;
        error_files++;
        continue;	// sembla que no xuti...potser si: quan prova de desmapejar el qfile pq surt del seu àmbit, i crec que peta sempre que vol desmapejar un fitxer de mida zero. ok.
      }

      clean_offset = file.getMap()-1;
      clean_block_begin = frame->getMap();
      skipped = frame->getMap()-file.getMap();
      if (skipped) {
        if (!quiet)
          cerr.form(_("%s: skipped %d bytes of garbage at the beginning\n"),
                    *argv,skipped);
        total_skipped += skipped;
        stream_length -= skipped;
      }
      if (stream_length >= frame->getLength()) {
        stream_length -= frame->getLength();
        clean_block_end = frame->getMap()+frame->getLength()-1;
        frames++;
      }
      else 
        if (!quiet)
          cerr.form(_("%s: first frame incomplete: %d of %d bytes\n"),
                    *argv,stream_length,frame->getLength());
             
      while (stream_length) {

        try {
          skipped = frame->setNext(stream_length);
        }
        catch (qexception e) {
          if (!quiet)
            cerr.form(_("%s: %d bytes of garbage at the end"),
                      *argv,stream_length);
          if (verbose)
            cerr.form(" (%d..%d)",file.getSize()-stream_length,file.getSize()); 
          if (!quiet)
            cerr << endl;
          total_skipped += stream_length;
          stream_length = 0;
          break;
        }

        if (skipped) {
          if (!quiet)
            cerr.form(_("%s: skipped %d bytes"),*argv,skipped);
          if (verbose)
            cerr.form(" (%d..%d)",
                      clean_block_end-file.getMap()+1,
                      frame->getMap()-file.getMap()-1); 
          if (!quiet)
            cerr << endl;

          if ( clean_block_begin!=clean_offset && 
              (clean_offset!=file.getMap()-1)) {
	    if (del) {
              if (!quiet)
                cerr.form(_("%s: moving %d bytes"),
                          *argv,
                          clean_block_end-clean_block_begin+1);
              if (verbose)
                cerr.form(" (%d..%d) <-- (%d..%d)",
                           clean_offset-file.getMap()+1,
                           clean_offset-file.getMap()+1+clean_block_end-clean_block_begin,
                           clean_block_begin-file.getMap(),
                           clean_block_end-file.getMap());
              if (!quiet)
                cerr << endl;
  
                file.move ((u_int32_t)(clean_block_begin-file.getMap()),
                           (u_int32_t)(clean_block_end-file.getMap()),
                           (u_int32_t)(clean_offset+1-file.getMap()));
            }
  	    clean_offset += clean_block_end-clean_block_begin+1;
          }
          else 
            clean_offset = clean_block_end;
          
          clean_block_begin = frame->getMap();
          total_skipped += skipped;
          stream_length -= skipped;
        }
        if (stream_length >= frame->getLength()) {
          stream_length -= frame->getLength();
          clean_block_end = frame->getMap()+frame->getLength()-1;
          frames++;
        }
        else {
          if (!quiet)
            cerr.form(_("%s: last frame (%d+1) truncated: %d of %d bytes\n"),
                      *argv,frames,stream_length,frame->getLength());
          total_skipped += stream_length;
          stream_length = 0;
          break;
        }
      }

      if (clean_block_end>clean_block_begin &&
          clean_block_begin>clean_offset && 
          (clean_offset<clean_block_begin-1)) {

	if (del) {
          if (!quiet)
            cerr.form(_("%s: moving %d bytes"),
                       *argv,clean_block_end-clean_block_begin+1);
          if (verbose)                       
            cerr.form(" (%d..%d) <-- (%d..%d)",
                       clean_offset-file.getMap()+1,
                       clean_offset-file.getMap()+1+clean_block_end-clean_block_begin,
                       clean_block_begin-file.getMap(),
                       clean_block_end-file.getMap());
	  if (!quiet)
            cerr << endl;

          file.move ((u_int32_t)(clean_block_begin-file.getMap()),
                     (u_int32_t)(clean_block_end-file.getMap()),
                     (u_int32_t)(clean_offset+1-file.getMap()));
        }
        clean_offset+=clean_block_end-clean_block_begin+1;
      }
      else
        clean_offset = clean_block_end;

      if (hasTag) {
        if (del) {
          if (delTag) {
            if (!quiet)
              cerr.form(_("%s: skipping tag"),*argv);
            if (verbose)
              cerr << " (" << qtag::LENGTH << " bytes)";
            if (!quiet)
              cerr << endl;
            total_skipped += qtag::LENGTH;
          }
          else {
            if (total_skipped) {
              file.move ((u_int32_t)(tag->getMap()-file.getMap()),
                         (u_int32_t)(tag->getMap()-file.getMap()+qtag::LENGTH-1),
                         (u_int32_t)(clean_offset+1-file.getMap()));
              if (!quiet)
                cerr.form(_("%s: moving tag (%d bytes)"),*argv,qtag::LENGTH);
              if (verbose)
                cerr.form(" (%d..%d) <-- (%d..%d)",
                          clean_offset-file.getMap()+1,
                          clean_offset-file.getMap()+1+qtag::LENGTH-1,
                          tag->getMap()-file.getMap(),
                          tag->getMap()-file.getMap()+qtag::LENGTH-1);
              if (!quiet)
                cerr << endl;
            }
            clean_offset += qtag::LENGTH;
          }
        }
        else
          clean_offset += qtag::LENGTH;
      }

      if (total_skipped) {
        if (!quiet)
          cerr.form(_("%s: %d of %d discardable bytes\n"),
                    *argv,total_skipped,file.getSize());
        if (del) {
          if (!quiet)
            cerr.form(_("%s: truncating to %d bytes\n"),
                      *argv,file.getSize()-total_skipped);
	  file.truncate(file.getSize()-total_skipped);
        }
      }

      if (verbose) {
        if (del) {
          qmp3 mp3(*argv);
          cerr << mp3 << endl;	// quedaria millor la versió 'verbose...'
        }
        else
          cerr.form(_("%s: %d frames, %d bytes\n"),
                    *argv,frames,clean_offset-file.getMap()+1);
      }

      if (hasTag)
        delete tag;
              
    } catch (qexception e) {
      cerr << *argv << ": " << e << endl;
      error_files++;
    }

    if (total_skipped)
      error_files++;

  } while (*++argv);

  if (error_files && verbose)
    cerr.form(_("%d error files found\n"),error_files);
}
