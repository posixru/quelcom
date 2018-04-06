/*! \file
 * header functions for qfile
 */

# ifndef _qfile_hh_
# define _qfile_hh_

# include <string>	// string
# include <sys/stat.h>	// struct stat, fstat
# include <sys/types.h>	// u_int32_t


/*!
 * \brief class for mapped files.
 *
 * this class models a files mapped in memory allowing better performance and easier programmer use than no mapped files
 */
class qfile {

  private:
   //! file descriptor
    int fd;
   //! flags used to open the file
    u_int32_t flags;
   //! flags used to map the file in memory
    u_int32_t mapflags;
   //! name of the file
    string name;
   //! memory location where the file is mapped
    caddr_t map;
   //! size of the file
    u_int32_t size;

  public:

    static const u_int32_t READ, WRITE, READWRITE, TRUNCATE;
    static const u_int32_t NEW;

   /*!
    * initializes itself with the given name and flags
    * \param name the name of the file to open
    * \param flags the flags to select the opening mode
    */
    qfile(string name, u_int32_t flags=READ);

   /*!
    * \brief destructor
    *
    * unmaps the file from memory flushing it to disk
    */
    ~qfile();

// synchronize (protected)
   /*!
    * \brief deletes part of the file
    * \param start first byte to delete
    * \param end last byte to delete
    * \return the new size in bytes of the file
    */
    u_int32_t cut (u_int32_t start, u_int32_t end);
   /*!
    * \brief truncates the file to the given number of bytes
    * \param size the number of bytes to keep
    * \return the new size in bytes of the file
    */
    u_int32_t truncate (u_int32_t size);
   /*!
    * \brief move data in the file
    * \param start offset of the first byte to move
    * \param end offset of the last byte to move
    * \param dest offset of the first byte to overwrite
    * \return the number of bytes moved
    */
    u_int32_t move (u_int32_t begin, u_int32_t end, u_int32_t dest);

// synchronize (protected)
   /*!
    * \brief append a file at the end
    * \param f the file to append
    * \return the new size in bytes of the file
    */
    u_int32_t append (qfile f);				// operador << ?
   /* HAS BUGS!!!
    * \brief append part of a file at the end
    * \param f the file to append partially
    * \param start the first byte of f to append
    * \param end the last byte of f to append
    * \return ???
    */
//    u_int32_t append (qfile f, u_int32_t start, u_int32_t end);
   /*!
    * \brief append a block of data at the end
    * \param pointer pointer to the start of the data to append
    * \param size number of bytes to append
    * \return the new size in bytes of the file
    */
    u_int32_t append (caddr_t pointer, u_int32_t size);
    
   /*!
    * \brief gets where the file is mapped
    * \return return the memory location where the file is mapped
    */
    caddr_t getMap(); 
   /*!
    * \brief gets the size of the file
    * \return the number of bytes in the file
    */
    u_int32_t getSize();
   /*!
    * \brief gets the name of the file
    * \return a string with the name of the file
    */
    string getName();

   /*!
    * \brief print the wav characteristics on the given output stream
    * \param os the given output stream
    */
    void print (ostream *os);
 
   /*!
    * \brief creates a copy of the file with the given name
    * \param copyname the name of the file where to copy the file contents
    * \return an instantiation of the newly created file
    */
    qfile dup (string copyname);
};

ostream& operator<<(ostream&, qfile&);
# endif
