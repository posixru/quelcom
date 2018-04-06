/*! \file
 * implementation functions for class qfile
 */

# include <sys/types.h>	// lseek
# include <sys/mman.h>	// mmap, munmap
# include <unistd.h>	// close, ftruncate, write
# include <string.h>	// strerror, memmove
# include <errno.h>	// errno
# include <fcntl.h>	// open
# include <string>	// string

// AFEGIR UN QVERBOSE AL FINAL DE CADA FUNCIO

# include "qfile.hh"
# include "qexception.hh"
# include "qmisc.hh"

#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
  # define _(s) gettext (s)
#else
  # define _(s) (s)
#endif

// revisar l'umask. el millor seria fer l'open normal sense això i el creat, o bé passar-lo a un open+creat+trunc, o bé posar-hi un valor que no afecti a l'umask i així es passa la "culpa" a l'usuari...
# define CREATION_MASK	(S_IRUSR|S_IWUSR)

const u_int32_t qfile::READ=O_RDONLY, 
           qfile::WRITE=O_WRONLY, 
           qfile::READWRITE=O_RDWR, 
           qfile::TRUNCATE=O_TRUNC;

const u_int32_t qfile::NEW = qfile::READWRITE|qfile::TRUNCATE|O_CREAT;


qfile::qfile(string filename, u_int32_t f) {
  
  name = filename;
  flags = f;

  if (flags&WRITE || flags&TRUNCATE || flags&READWRITE)
    mapflags=PROT_READ|PROT_WRITE;
  else
    mapflags=PROT_READ;

  if ((fd=open(filename.c_str(),flags,CREATION_MASK))==-1)
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));
 
  struct stat stat;

  if (fstat(fd,&stat)) {
    close(fd);
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));
  }
  
  if (!S_ISREG(stat.st_mode)) {
    close(fd);
    throw qexception(__PRETTY_FUNCTION__,_("not a regular file"));
  }
 
  size = stat.st_size;

  if ((map=(caddr_t)mmap((caddr_t)0,size,mapflags,MAP_SHARED,fd,0))==MAP_FAILED) {
    close(fd);
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));
  }

#ifdef QVERBOSE
  cerr << "file '" << *this << "' successfully mapped at address " << (void*)getMap() << endl;
#endif
} 


qfile::~qfile() {

 // munmap always gives an error when unmapping empty files...
  if (size>0)
    if (munmap(map,size))
      throw qexception(__PRETTY_FUNCTION__,strerror(errno));

  if (close(fd))
#ifdef QVERBOSE
    cerr << getName() << ": altre cop el close" << endl;
#else
    ;
#endif
}


u_int32_t qfile::truncate (u_int32_t newsize) {

  if (newsize>=size)
    return size;
 
  if (munmap(map,size))
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));

  if (ftruncate(fd,newsize))
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));

  size = newsize;
 
  if ((map=(caddr_t)mmap(map,size,mapflags,MAP_SHARED,fd,0))==MAP_FAILED)
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));

  return size;
} 


u_int32_t qfile::move (u_int32_t start, u_int32_t end, u_int32_t dest) {

  if (start>end)
    throw qexception(__PRETTY_FUNCTION__,
                     string("move: end(")+uint2string(end)+string(")<start(")+
                                          uint2string(start)+string(")"));

  if (start>size)
    throw qexception(__PRETTY_FUNCTION__,
                     string("move: start(")+uint2string(start)+
                     string(") is out of file"));
    
  if (end>size)
    end = size;

  if (dest+end-start+1>size)
    end = start+(size-dest)-1;

  char *base = (char*)map;

  memmove(base+dest,base+start,end-start+1);
   
  return end-start+1;
}


u_int32_t qfile::cut (u_int32_t start, u_int32_t end) {

  if (start>size)
    return size;

  if (end<start)
    throw qexception(__PRETTY_FUNCTION__,
                     string("cut: end(")+uint2string(end)+string(")<start(")+
                                         uint2string(start)+string(")"));

  if (end>=size)
    end = size-1;

  memmove((char*)map+start,(char*)map+end+1,size-1-end);

  truncate(size-(end-start+1));

  return size;
}


u_int32_t qfile::append (qfile f) {

  if (munmap(map,size))
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));
  
  if (lseek(fd,0,SEEK_END)==-1)
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));

  if (write(fd,f.getMap(),f.getSize())!=(int)f.getSize())
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));

  size += f.getSize();

  if ((map=(caddr_t)mmap(map,size,mapflags,MAP_SHARED,fd,0))==MAP_FAILED)
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));
                     
  return size;
}

/*
u_int32_t qfile::append (qfile f, u_int32_t start, u_int32_t end) {

cerr << "this code is buggy. use another append!!" << endl;

  if (start>end)
    throw qexception(__PRETTY_FUNCTION__,
                     string("append: end(")+uint2string(end)+string(")<start(")+
                                            uint2string(start)+string(")"));
                     
  if (start>=f.getSize())
    return size;

  if (end>=f.getSize())
    end = f.getSize()-1;

  if (munmap(map,size))
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));
  
  if (lseek(fd,0,SEEK_END)==-1)
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));

  if (write(fd,f.getMap()+start,end-start+1)!=(int)(end-start+1))
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));

  size += end-start+1;

  if ((map=(caddr_t)mmap(map,size,mapflags,MAP_SHARED,fd,0))==MAP_FAILED)
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));

  return size;
}
*/

u_int32_t qfile::append (caddr_t pointer, u_int32_t appsize) {

  if (size)
    if (munmap(map,size))
      throw qexception(__PRETTY_FUNCTION__,strerror(errno));

  if (lseek(fd,0,SEEK_END)==-1)
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));

  if (write(fd,pointer,appsize)!=(int)appsize)
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));
  
  size += appsize;

  if ((map=(caddr_t)mmap(map,size,mapflags,MAP_SHARED,fd,0))==MAP_FAILED)
    throw qexception(__PRETTY_FUNCTION__,strerror(errno));

  return size;
}


qfile qfile::dup(string newname) {

  int newfd;

  if ((newfd=creat(newname.c_str(),CREATION_MASK))==-1)
    throw qexception(__PRETTY_FUNCTION__,newname+string(strerror(errno)));
 
  if (write(newfd,getMap(),getSize())!=(int)getSize())
    throw qexception(__PRETTY_FUNCTION__,newname+string(strerror(errno)));

  close (newfd);

  return qfile(newname);
}

caddr_t qfile::getMap() { return map; }
u_int32_t qfile::getSize() { return size; }
string qfile::getName() { return name; }

void qfile::print (ostream *os) { *os << getName(); }

ostream& operator<<(ostream &os, qfile &f) {
  f.print(&os);
  return os;
}
