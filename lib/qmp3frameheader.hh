/*! \file
 * header functions for qmp3frameheader
 */

# ifndef _qmp3frameheader_hh_
# define _qmp3frameheader_hh_

# include <sys/types.h>
# include <string>

/*!
 * \brief class for mp3 frame headers
 *
 * this class models a mp3 frame header mapped in memory
 */
class qmp3frameheader {

  private:

   /*!
    * \brief struct for mp3 frame headers
    *
    * this struct is defined to match exactly with a mp3 frame header with no change on a little-endian machine
    */
    struct header {
      u_int32_t 
	sync1:8,		/*!< syncro field. should be 0x0ff */
	protection:1,		/*!< whether or not the frame has crc */
        layer:2,		/*!< mpeg frame layer */
        version:2,		/*!< mpeg frame version */
        sync2:3,		/*!< syncro field. should be 0x7 */
	privat:1,		/*!< bit for private use */
        padding:1,		/*!< whether the frame is one byte larger */
        samplerate:2,		/*!< sample rate */
        bitrate:4,		/*!< bit rate */
	emphasis:2,		/*!< which type of pre-emphasis must be used */
        original:1,		/*!< original bit stream (1) or copy (0) */
        copyright:1,		/*!< whether or not is a copyrighted frame */
        mode_extension:2,	/*!< type of joint stereo coding */
        channel_mode:2;		/*!< mono or which type of stereo */
    };
        
   //! pointer to the header (memory location where it is located)
    struct header *header;

  public:

   /*!
    * \brief gets where the frame header is mapped
    * \return the memory location where the frame header is mapped
    */
    caddr_t getMap();

   /*!
    * \brief remaps (changes) the location of the header
    * \param a the new address of the header
    */
    void remap(caddr_t a);

   //! length in bytes of a mp3 frame header
    static const u_int32_t HEADERSIZE;
   
    static const u_int32_t MASK;

   /*!
    * \brief looks for (and reuses itself with) the next frame
    * \param bufsize the number of bytes where to find the header
    * \return the number of (garbage) skipped bytes
    */
    u_int32_t setNext(u_int32_t bufsize=HEADERSIZE);

   /*!
    * \brief looks for (and reuses itself with) the next frame
    * \param endstream 
    * \return the number of (garbage) skipped bytes
    */
    u_int32_t setNext(caddr_t endstream);

    static const u_int32_t LAYER1, LAYER2, LAYER3;
    static const u_int32_t VERSION1, VERSION2, VERSION25, UNDEFINED_VERSION;

   /*!
    * \brief look for a frame header
    * \param a the start searching point
    * \param bufsize the number of bytes where to find the header
    * \param signature the header must be compatible with this signature
    * \param backwards whether to search forwards (by default) or backwards
    * \return the address where the header has been found
    */
    static caddr_t seek_header(caddr_t a, u_int32_t bufsize=HEADERSIZE, u_int32_t signature=0, bool backwards=false);

    static bool compatible(caddr_t p, u_int32_t signature);
    bool compatible(qmp3frameheader &h);
    bool compatible(u_int32_t);

    void print (ostream *os);

    bool valid();
    static bool valid(caddr_t);

    qmp3frameheader();
    qmp3frameheader(caddr_t p, u_int32_t bufsize = HEADERSIZE, u_int32_t signature=0);
 
   // direct access to the header
    u_int32_t get();
    void set(u_int32_t h);

   // direct access to the header fields (without checking!!):
    enum qmp3frameheaderfield { 
      SYNC1, SYNC2, _VERSION, LAYER, PROTECTION, BITRATE, SAMPLERATE, 
      PADDING, PRIVAT, CHANNEL_MODE, MODE_EXTENSION, COPYRIGHT,
      ORIGINAL, EMPHASIS
    };
    u_int32_t getField (enum qmp3frameheaderfield f);
    void setField (enum qmp3frameheaderfield f, u_int32_t v);

    string getVersion();
    string getLayer();
    u_int32_t getSamplesPerFrame();
    u_int32_t getLength();
    u_int32_t getSampleRate();
    u_int32_t getBitRate();
    u_int32_t getMsDuration();
    u_int32_t getSignature();
    bool isSilence();

    void write(int fd);
};
ostream &operator<<(ostream &os, qmp3frameheader &fh);


# endif
