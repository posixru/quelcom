/*! \file
 * header functions for qvf
 */

# ifndef _qvf_hh_
# define _qvf_hh_

# include <fstream.h>	// ostream
# include <sys/types.h>


/*! \class qvf
 * \brief class for value-format pairs.
 *
 * this class is a general porpouse class for quelcom. 
 * it allows handling values together with differents formats (units).
 */
class qvf {

public:

 /*! \enum format
  * \brief enumeration for the different formats supported
  */
  enum format { 
    BYTES, 		/*!< bytes */
    KBYTES, 		/*!< kilobytes (1024 bytes) */
    MBYTES, 		/*!< megabytes (1024 kilobytes) */
    MINUTES, 		/*!< minutes */
    SECONDS, 		/*!< seconds */
    MILLISECONDS, 	/*!< milliseconds */
    SPECIFIC, 		/*!< specific, depends on what's applied */
    UNDEFINED		/*!< no defined, error prone... */
  };

  private:
   //! the current value
    u_int32_t value;
   //! the current format
    enum format format;

   // què fashoquí???
    u_int32_t time2msec(char*);

  public:
   /*! \fn static char format2char(enum format f)
    * \brief converts the given format to its equivalent char
    * \param f the format to convert to a char
    * \return the corresponding char
    */ 
    static char format2char(enum format f);
   /*! \fn static enum format char2format(char c)
    * \brief converts the given char to its equivalent format
    * \param c the char to convert to format
    * \return the corresponding format
    */ 
    static enum format char2format(char c);

   //! default constructor
    qvf();
   /*!
    * initializes itself with the given value and format
    * \param value the initial value
    * \param format the initial format (specific by default)
    */
    qvf(u_int32_t value, enum format f=SPECIFIC);
   /*!
    * initializes itself reading from a char string
    *
    * the format for the char string must be <u_int32_t>[M|k|b|m|s|j]: a unsigned int followed optionally by a char. 
    * if there's no char, the typed is initialized as specific, otherwise it should be M (for megabytes), k (for kilobytes), b (for bytes), m (for minutes), s (for seconds) or j (for milliseconds).
    * \param str the char string containig the value and the format
    */
    qvf(char *str);

   /*! \fn u_int32_t getValue()
    * \brief gets the value
    * \return the value
    */
    u_int32_t getValue();
   /*! \fn enum format getFormat()
    * \brief gets the format
    * \return the format
    */
    enum format getFormat();

   /*! \fn void set (unsigned int v, enum format f=SPECIFIC)
    * \brief sets the value and the format
    * \param v the value
    * \param f the format
    */
    void set (unsigned int v, enum format f=SPECIFIC);

   /*! \fn void print (ostream *os) 
    * \brief print the vf characteristics on the given output stream
    * \param os the given output stream
    */
    void print (ostream *os);
};
ostream &operator<<(ostream &os, qvf &vf);

#endif
