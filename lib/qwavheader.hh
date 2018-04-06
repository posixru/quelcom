/*! \file
 * header functions for qwavheader
 */

#ifndef _qwavheader_hh_
#define _qwavheader_hh_

# include <sys/types.h>
# include "qvf.hh"

/*!
 * \brief class for wav headers.
 *
 * this class models a 'canonical' wav header mapped in memory
 */ 
class qwavheader {

  private:

   /*! 
    * \brief struct for wav headers
    *
    * this struct is defined to match exactly with a 'canonical' wav header with no change on a little-endian machine
    */
    struct header {

     // riff chunk
      char riff[4];		/*!< should be 'RIFF' */
      u_int32_t rifflength;	/*!< should be file size - 8 */
      char wave[4];		/*!< should be 'WAVE' */

     // fmt chunk
      char fmt_[4];		/*!< should be 'fmt ' */
      u_int32_t fmtlength;	/*!< should be 16 (length of the fmt data) */
      u_int16_t format;		/*!< should be 1 (pcm) */
      u_int16_t channels;	/*!< stereo (1) or mono (0) */
      u_int32_t samplerate;	/*!< samples per second */
      u_int32_t bytespersec;	/*!< bytes per second: block align * sample rate */
      u_int16_t bytespersample;	/*!< bytes per sample (block align) */
      u_int16_t bitspersample;	/*!< bits per sample: 8 or 16 */
    
      char data[4];		/*!< should be 'data' */
      u_int32_t datalength;	/*!< data block length (file size - 44) */
    };

   //! pointer to the header (memory location where it is located)
    struct header *header;

  public:

   //! length in bytes of the wav header
    static const u_int32_t HEADERSIZE;
  
   /*!
    * initializes itself with a memory address and validates with the given wav length
    * \param p the memory address where the header is located
    * \param length length of the wav file or zero to ignore
    */
    qwavheader (caddr_t p, u_int32_t length=0);

   /*!
    * \brief gets the number of samples
    * \return the number of samples
    */
    u_int32_t getSamples();

   /*!
    * \brief modify the number of samples by adding a positive or negative increment
    * \param increment the value to be added to the current number of samples
    * \return the new number of samples
    */
    u_int32_t addSamples(int increment);
   /*!
    * \brief set the number of samples
    * \param number the new number of samples
    * \return the new number of samples
    */
    u_int32_t setSamples(u_int32_t number);
   /*!
    * \brief get the offset of the given sample in value-format format
    * \param vf the sample in value-format format
    * \return the offset of the given sample from the start of the file
    */
    u_int32_t getOffset (qvf &vf);
   /*!
    * \brief get the offset of the given sample
    * \param sample the sample
    * \return the offset of the given sample from the start of the file
    */
    u_int32_t getOffset (u_int32_t sample);
   /*!
    * \brief get the sample number of the given sample in value-format format
    * \param vf the sample in value-format format
    * \return the number of sample in range 1..getSamples()
    */
    u_int32_t getSample (qvf &vf);
   /*!
    * \brief get the bits per sample (bps) value
    * \return the bits per sample value
    */
    u_int32_t getBitsPerSample();
   /*!
    * \brief get the bytes per sample value
    * \return the bytes per sample value
    */
    u_int32_t getBytesPerSample();
   /*!
    * \brief gets the number of channels
    * \return the number of channels
    */
    u_int32_t getChannels();
   /*!
    * \brief get whether is stereo or not
    * \return whether is stereo or not
    */
    bool getStereo();
   /*!
    * \brief gets the sample rate
    * \return the sample rate
    */
    u_int32_t getSampleRate();
   /*!
    * \brief get the duration in milliseconds
    * \return the duration in milliseconds
    */
    u_int32_t getMsDuration();

   /*!
    * \brief remaps (changes) the location of the header
    * \param a the new address of the header
    */
    void remap(caddr_t);
   /*!
    * \brief compares both headers whether they're compatible or not
    * \param h the header to check for compatibility
    * \return whether compatible or not
    */
    bool compatible(const qwavheader&);
   /*!
    * \brief checks the validity of the header. 
    * \return whether is valid or not
    */
    bool valid();
   /*!
    * \brief checks the validity of the header for a wav file of the given length
    * \param length the length to check for validity
    * \return whether is valid or not
    */
    bool validLength(u_int32_t length);
   /*!
    * \brief write (copy) the header to the given address
    * \param a the memory address where to write the header
    */
    void write(caddr_t );
};

# endif
