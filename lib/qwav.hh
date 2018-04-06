/*! \file
 * header functions for qwav
 */

# ifndef _qwav_hh_
# define _qwav_hh_

# include "qfile.hh"
# include "qwavheader.hh"
# include "qcuthandler.hh"
# include <sys/types.h>


/*! \class qwav
 * \brief class for wav files.
 *
 * this class models a wav file mapped in memory
 */ 
class qwav : public qfile, public qwavheader {

  public:

   /*!
    * initializes itself with the given filename and flags
    * \param filename the file to initialize the wav
    * \param flags the file open mode (see ...)
    */
    qwav (string filename, u_int32_t flags = READ);

   /*!
    * \brief compares both wavs whether they're compatible or not
    * \param wav the wav to check for compatibility
    * \return whether compatible or not
    */
    bool compatible(qwav &wav);
   /*!
    * \brief print the wav characteristics on the given output stream
    * \param os the given output stream
    */
    void print (ostream *os);
   /*!
    * \brief append the given wav at the end
    * \param wav the wav to append at the end
    */
    u_int32_t append (qwav wav);
   /*!
    * \brief perform a cut operation
    * \param ch the cuthandler which specifies how to cut
    * \return ???
    */
    u_int32_t cut (qcuthandler &ch);
   /*!
    * \brief deletes part of the samples
    * \param first the first sample to delete
    * \param last the last sample to delete
    * \return ??? check this!
    */
    u_int32_t cut (u_int32_t first, u_int32_t last);
   /*!
    * \brief truncates the file to the given number of samples
    * \param samples the number of samples to keep
    */
    void truncate (u_int32_t samples);
   /*!
    * \brief move samples in the file
    * \param start_sample first sample to move
    * \param end_sample last sample to move
    * \param dest_sample first sample to be overwritten
    */
    void move (u_int32_t sample_start, u_int32_t sample_end, u_int32_t sample_dest);
   /*!
    * \brief copy some samples to the given filename
    * \param filename the file where to put the given range of samples
    * \param start_sample the first sample to copy
    * \param end_sample the last sample to copy
    * \return the newly created wav
    */
    qwav& getWav(string filename, u_int32_t startsample, u_int32_t endsample);
};

ostream &operator<<(ostream &os, qwav &w);

# endif
