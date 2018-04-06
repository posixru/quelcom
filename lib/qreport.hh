/*! \file
 * header functions for qreport
 */

# ifndef _report_hh_
# define _report_hh_

# include <string>
//# include <list>

# include "qfile.hh"
# include "qmp3.hh"


class qreport {

  private:
    enum type { FILE, MP3, DIR, BASE };

    u_int32_t files, directories, errors;
    u_int32_t samplerate, bitrate;
    long long msduration, size;
    enum type type;
    string name;
    bool vbr;

//    list<qreport> report_list;

  public:
    qreport ();
    qreport (qfile &f);
    qreport (qmp3 &mp3);
    qreport (const string str);

    void add (qreport r);// adds a report. behaviour depends on report type 
//    void add_error (string); // report an error
    void print (ostream *os);
    void html (ostream *os, string link="");

    u_int32_t getDuration();
    u_int32_t getBitRate();
    u_int32_t getSampleRate();
    string getName();

    u_int32_t getFiles() { return files; }

    string parameter2string();
    string print_html ();
//    setOutputFormat(...);
};

ostream &operator<<(ostream &os, qreport &r);

# endif
