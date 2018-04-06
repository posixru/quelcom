/*! \file
 * header functions for qcuthandler
 */

# ifndef _qcuthandler_hh_
# define _qcuthandler_hh_

# include <string>

# include "qvf.hh"
   
/*!
 * \brief class for file-cut specifications
 *
 * this class does nothing: it is used just for keeping together different parameters used to define a cut (a fragment) of a file. basically, where the cut starts, where it ends and, optionally, the filename where to copy the cut and whether it must be deleted. the class values are filled by applications (qmp3cut.cc, qwavcut.cc) and interpreted by the "library" (qwav::cut(), qmp3::cut())
 */
class qcuthandler {

  private:

   //! if specified, sets the begin of the cut using the beginning of the file as reference point
    qvf Begin;
   //! if specified, sets the begin of the cut using the end of the file as reference point
    qvf begin;
   //! if specified, sets the end of the cut using the beginning of teh file as reference point
    qvf End;
   //! if specified, sets the begin of the cut using the end of the file as reference point
    qvf end;
   //! if specified, sets the size of the cut
    qvf size;

   //! the filename where to put the cut
    string outfile;

   //! whether the cut must be deleted from the file
    bool del;

  public:
    qcuthandler();
   /*!
    * initializes itself with a string formed by two time specifiers separated by a '-'
    *
    * the string must have the form: [[h:]m:]s[.ms]-[[h:]m:]s[.ms]
    * example (one second): 0.4-1.4
    * example (six minutes and half second): 54:32.210-1:0:32.710
    */
    qcuthandler(char*);

   /*!
    * \brief sets the Begin member
    * \param B qvf object 
    */
    void setBegin (qvf &B);

   /*!
    * \brief sets the End member
    * \param E qvf object
    */
    void setEnd   (qvf &E);

   /*!
    * \brief sets the begin member
    * \param b qvf object
    */
    void setbegin (qvf &b);

   /*!
    * \brief sets the end member
    * \param e qvf object
    */
    void setend   (qvf &e);

   /*!
    * \brief sets the size member
    * \param s qvf object
    */
    void setSize  (qvf &s);

   /*!
    * \brief sets the file where the cut will be copied
    * \param o the file name
    */
    void setOutfile (string o);

   /*!
    * \brief sets whether the cut must be deleted
    * \param d bool indicating whether the cut must be deleted
    */
    void setDel (bool d);
 
   /*!
    * \brief gets the Begin member
    * \return the Begin member
    */
    qvf& getBegin();

   /*!
    * \brief gets the End member
    * \return the End member
    */
    qvf& getEnd();

   /*!
    * \brief gets the begin member
    * \return the begin member
    */
    qvf& getbegin();

   /*!
    * \brief gets the end member
    * \return the end member
    */
    qvf& getend();

   /*!
    * \brief gets the size of the cut
    * \return the size of the cut
    */
    qvf& getSize();

   /*!
    * \brief gets the file where the cut must be copied
    * \return the name of the file where the cut must be copied
    */
    string getOutfile();

   /*!
    * \brief gets whether the cut must be deleted
    * \return whether the cut must be deleted
    */
    bool getDel();

// i quedaria genial poder sumar i restar qvf per desplaçar el tall.
};
#endif
