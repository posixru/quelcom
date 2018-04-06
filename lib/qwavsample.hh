/*! \file
 * header functions for qwavsample
 */

#ifndef _qwavsample_hh
#define _qwavsample_hh_

# include "qwav.hh"
# include <sys/types.h>


/*! \class qwavsample
 * \brief class for wav samples.
 *
 * this class models a wav sample: mono or stereo, 8 or 16 bit
 */
class qwavsample {

  private:

   //! struct for 8 bit mono wav samples
    struct mono8 { char m8; };
   //! struct for 16 bit mono wav samples
    struct mono16 { short m16; };
   //! struct for 8 bit stereo wav samples
    struct stereo8 { char left; char right; };
   //! struct for 16 bit stereo wav samples
    struct stereo16 { short left; short right; };

   //! enum to define the type of sample
    enum type { 
      MONO8, 	/*!< 8 bit mono wav sample */
      MONO16, 	/*!< 16 bit mono wav sample */
      STEREO8, 	/*!< 8 bit stereo wav sample */
      STEREO16 	/*!< 16 bit stereo wav sample */
    };

   //! the type of sample
    enum type type;
 
   //! union for wav samples
    union sample {
      char mono8;
      short mono16;
      struct stereo8 stereo8;
      struct stereo16 stereo16;
    };

  //! pointer to the sample (memory location where it is mapped)
    union sample *sample;

  public:

   /*!
    * initializes itself with the given sample of the given wav
    * \param wav pointer to a wav file to initialize the sample
    * \param sample the sample to read (the first one by default)
    */ 
    qwavsample(qwav *wav, u_int32_t sample=1);
   /*!
    * initializes itself with a memory address and the given sample type
    * \param a the memory address where the sample is located
    * \param type the type of sample (16 bit stereo by default)
    */ 
    qwavsample(caddr_t a, enum type=STEREO16);
   /*!
    * initializes itself with a memory address and the given sample info
    *   from which the sample type is deduced
    * \param a the memory address where the sample is located
    * \param channels the number of channels of the sample (1/2)
    * \param bps the number of bits per sample (8/16)
    */ 
    qwavsample(caddr_t a, u_int32_t channels, u_int32_t bps);
 
   /*! 
    * \fn int getLeft()
    * \brief gets the left channel value
    * \return the left channel value or simply the sample if mono
    */
    int getLeft();
   /*! \fn int getRight()
    * \brief gets the right channel value
    * \return the left channel value or simply the sample if mono
    */
    int getRight();
   /*! \fn void setNext()
    * \brief sets the sample to the next contiguous sample in memory
    */
    void setNext();
   /*! \fn bool isSilence(u_int32_t threshold=0)
    * \brief returns whether the sample value is under the threshold or not
    * \param threshold (zero by default=>absolute silence)
    * \return whether the sample value is under the threshold
    */
    bool isSilence(u_int32_t threshold=0);
   /*! \fn void print (ostream &os)
    * \brief print the sample characteristics on the given output stream
    * \param os the given output stream
    */
    void print (ostream &os);
   /*! \fn void prod(double f)
    * \brief multiplies the sample value with the given factor
    * \param f the factor to multiply the sample value with
    */
    void prod(double d);
   /*! \fn void set (int l, int r=0)
    * \brief sets a new value for the sample
    * \param l the left channel value, or the sample value if mono
    * \param r the right channel value. omit if mono
    */
    void set (int l, int r=0);
};

ostream& operator<<(ostream &os,qwavsample &s);
#endif
