/*! \file
 * header functions for qtag
 */

# ifndef __qtag_hh__
# define __qtag_hh__

# include "qfile.hh"
# include <sys/types.h>


/*!
 * \brief class for mp3 tags
 *
 * this class models (partially) a mp3 tag v1.0
 */
class qtag {

  private:
    static const u_int32_t fieldlengths[];

   //! struct for mp3 tags
    struct tag {
      char tagid[3],	/*!< 'TAG' */
      title[30],	/*!< title */
      artist[30],	/*!< artist */
      album[30],	/*!< album name */
      year[4],		/*!< year */
      comment[30],	/*!< free comment */
      genre[1];		/*!< one byte music genre identifier */
    };

   //! memory location where the tag is mapped
    struct tag *tag;

  public:

   //! length of the tag
    static const u_int32_t LENGTH;

   //! list of tag fields
    enum field { TAG=0, TITLE, ARTIST, ALBUM, YEAR, COMMENT, GENRE };

   /*!
    * initializes itself with a tag in memory
    * \param a the address where the tag is found
    */
    qtag (caddr_t a);
   /*!
    * initializes itself with a tag in a file
    * \param f the file where to look for the tag
    */
    qtag (qfile &f);

   /*!
    * \brief gets the location in memory where the tag is placed
    * \return the memory address of the tag
    */
    caddr_t getMap();
   // treballar amb strings!!
    void get (enum field f, char *); 
    void set (enum field f, char *); 

   /*
    * \brief whether the tag is (found and) valid
    * \return whether is a valid tag or not
    */
    bool isValid();

    u_int32_t fieldlength (enum field f) { return fieldlengths[f]; }
    void cleanFields ();

   /*!
    * \brief print the wav characteristics on the given output stream
    * \param os the given output stream
    */
    void print (ostream *os);
}; 

ostream &operator<<(ostream &os, qtag &t);

# endif
