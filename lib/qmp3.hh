/*! \file
 * header functions for qmp3
 */

# ifndef _qmp3_hh_
# define _qmp3_hh_

# include <string>

# include "qfile.hh"
# include "qtag.hh"
# include "qmp3frameheader.hh"
# include "qcuthandler.hh"
# include "qvf.hh"

/*! \class qmp3
 * \brief class for mp3 files.
 *
 * this class models a mp3 file mapped in memory
 */ 
class qmp3 : public qfile, public qmp3frameheader, public qtag {

  private:

   //! whether the entire file has been scanned searching for errors
    bool scanned;

   /*!
    * \brief the number of frames
    * \warning this value is consistent only if scanned==true
    */
    u_int32_t frames;
   /*!
    * \brief the duration time expressed in milliseconds
    * \warning this value is consistent only if scanned==true
    */
    u_int32_t msduration;

   //! whether the mp3 is vbr
    bool vbr;
   //! whether the mp3 has tag
    bool hasTag;
   /*!
    * \brief the tag of the mp3
    * \warning this value is consistent only if hasTag==true
    */
    qtag *tag;

//    void remap (caddr_t);
  public:

   /*!
    * initializes itself with the given filename and flags
    * \param filename the file to initialize the mp3
    * \param flags the file open mode
    * \see qfile::qfile()
    */
    qmp3 (string name, u_int32_t flags = READ);

//    u_int32_t scan (void *p, u_int32_t bufsize, u_int32_t signature=0);
   /*!
    * \brief scan the file for validity
    * \param bufsize the number of skippable bytes to find the first valid header. zero by default -> no garbage allowed
    * \return the number of frames of the mp3
    */
    u_int32_t scan (u_int32_t bufsize=0);
   /*!
    * \brief whether the stream has been scanned or not
    * \return whether the stream has been scanned or not
    */
    bool isScanned();
   /*!
    * \brief whether the stream has frames with different bit rate or not
    * \return whether the stream has frames with different bit rate or not
    */
    bool isVbr();
   /*!
    * \brief gets the duration of the stream
    * \return the duration of the stream expressed in milliseconds
    */
    u_int32_t getMsDuration();

   /*!
    * \brief print the mp3 characteristics on the given output stream
    * \param os the given output stream
    */
    void print (ostream *os);

   /*!
    * \brief append the given mp3 at the end.
    * \param mp3 the mp3 to append at the end
    * \param force whether to force the appending if the parameters don't match
    * \return the new number of frames of the mp3
    */
    u_int32_t append (qmp3 &mp3, bool force=false);
   /*!
    * \brief perform a cut operation
    * \param ch the cuthandler which specifies how to cut
    * \return the new number of frames of the mp3
    */
    u_int32_t cut (qcuthandler&);
   /*!
    * \brief deletes part of the frames
    * \param first the first sample to delete
    * \param last the last sample to delete
    * \return the new number of frames of the mp3
    */
    u_int32_t cut (u_int32_t firstframe, u_int32_t lastframe);
   /*!
    * \brief truncates the file to the given number of frames
    * \param frames the number of samples to keep
    */
    void truncate (u_int32_t frames);
   /*!
    * \brief move frams in the file
    * \param start_sample first frames to move
    * \param end_sample last frame to move
    * \param dest_sample first frame to be overwritten
    */
    void move (u_int32_t frame_start, u_int32_t frame_end, u_int32_t frame_dest);
    
   /*!
    * \brief gets the stream lenght (does not include the tag)
    * \return the number of bytes in the stream
    */
    u_int32_t getStreamLength ();

   /*!
    * \brief gets the number of frames in the stream
    * \warning this value is an aproximation if the stream isn't scanned
    * \return the number of frames in the stream
    */
    u_int32_t getFrames();
   /*!
    * \brief get the frame number of the given frame in value-format format
    * \param vf the frame in value-format format
    * \return the number of frame in range 1..getFrames()
    */
    u_int32_t getFrame(qvf &);
   /*!
    * \brief get the offset of the given frame
    * \param frame the frame 
    * \return the offset of the given frame from the start of the file
    */
    u_int32_t getOffset(u_int32_t frame);
   /*!
    * \brief copy some frames to the given filename
    * \param filename the file where to put the given range of frames
    * \param start_frame the first frame to copy
    * \param end_frame the last frame to copy
    * \return the newly created mp3
    */
    qmp3& getMp3(string name, u_int32_t start_frame, u_int32_t end_frame);
   /*!
    * \brief compares the mp3 parameters whether they're compatible or not
    * \param mp3 the mp3 to check for compatibility
    * \param force whether to force the compatibility if the parameters don't match
    * \return whether compatible or not
    */
    bool compatible(qmp3 &mp3, bool force=false);	// operador ==

//    tag getTag();
};

ostream &operator<<(ostream &os, qmp3 &m);

# endif
