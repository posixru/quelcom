/*! \file
 * header functions for qexception
 */

# ifndef _qexception_hh_
# define _qexception_hh_

#include <string>

/*!
 * \brief class for quelcom specific exceptions 
 *
 * simple class for raising and catching quelcom specific exceptions. it includes info from the method which raised the exception for debugging purpouses.
 */
class qexception {

  private:
   //! the method where the exception is raised
    string method;
   //! info describing the cause of the exception raising
    string info;
    
  public:
   /*!
    * initializes itself with the given method and info
    * \param m the method where the exception has been raised
    * \param i info explaining the cause of the exception raising
    */
    qexception (string m=string(), string i=string()):method(m),info(i) { }

   /*!
    * \brief gets the info of the exception
    * \return a string containg the info of the exception
    */
    string getInfo();

   /*!
    * \brief print the wav characteristics on the given output stream
    * \param os the given output stream
    */
    void print (ostream *os);
};

ostream &operator<<(ostream &os, qexception &e);
# endif
