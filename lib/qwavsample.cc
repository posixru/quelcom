/*! \file
 * implementation functions for class qsample
 */

# include "qwavsample.hh"
# include "qexception.hh"


#ifdef NLS
  # include <locale.h>
  # include <libintl.h>
 /*! \def define _(s)
  * \brief gettextize strings
  */
  # define _(s) gettext(s)
#else
 /*! \def define _(s)
  * \brief bypass gettextization
  */
  # define _(s) (s)
#endif


qwavsample::qwavsample (qwav *wav, u_int32_t s=1) {

  sample = (union sample*) (wav->getMap()+wav->getOffset(s));

  if (wav->getBitsPerSample()==16) {
    if (wav->getChannels()==2) {
      type = STEREO16;
    }
    else if (wav->getChannels()==1) {
      type = MONO16;
    }
    else
      throw qexception(__PRETTY_FUNCTION__,_("unsupported channel value"));
  }
  else if (wav->getBitsPerSample()==8) {
    if (wav->getChannels()==2) {
      type = STEREO8;
    }
    else if (wav->getChannels()==1) {
      type = MONO8;
    }
    else
      throw qexception(__PRETTY_FUNCTION__,_("unsupported channel value"));
  }
  else
    throw qexception(__PRETTY_FUNCTION__,_("unsupported bits/sample value"));

#ifdef QVERBOSE
  cerr << "sample type " << type << " mapped at address " << this->sample << endl;
#endif

}


qwavsample::qwavsample(caddr_t pointer, u_int32_t channels, u_int32_t bitspersample) {

  sample = (union sample*) pointer;
  
  if (bitspersample==16) {
    if (channels==2) {
      type = STEREO16;
      return;
    }
    else if (channels==1) {
      type = MONO16;
      return;
    }
    else
      throw qexception(__PRETTY_FUNCTION__,_("unsupported channel value"));
  }
  else if (bitspersample==8) {
    if (channels==2) {
      type = STEREO8;
      return;
    }
    else if (channels==1) {
      type = MONO8;
      return;
    }
    else
      throw qexception(__PRETTY_FUNCTION__,_("unsupported channel value"));
  }
  else
    throw qexception(__PRETTY_FUNCTION__,_("unsupported bits/sample value"));
}


qwavsample::qwavsample(caddr_t pointer, enum type t=STEREO16) {

  sample = (union sample*) pointer;
  type = t;
}

  
int qwavsample::getLeft() {
  
  switch (type) {
    case MONO8: return sample->mono8;
    case MONO16: return sample->mono16;
    case STEREO8: return sample->stereo8.left;
    case STEREO16: return sample->stereo16.left;
  }

  throw qexception(__PRETTY_FUNCTION__,_("quelcom internal error"));
}


int qwavsample::getRight() {
  
  switch (type) {
    case MONO8: return sample->mono8;
    case MONO16: return sample->mono16;
    case STEREO8: return sample->stereo8.right;
    case STEREO16: return sample->stereo16.right;
  }
  throw qexception(__PRETTY_FUNCTION__,_("quelcom internal error"));
}

void qwavsample::set(int left, int right=0) {
  
  switch (type) {
    case MONO8: sample->mono8=left; break;
    case MONO16: sample->mono16=left; break;
    case STEREO8: sample->stereo8.left=left;sample->stereo8.right=right; break;
    case STEREO16: sample->stereo16.left=left;sample->stereo16.right=right; break;
    default:
      throw qexception(__PRETTY_FUNCTION__,_("quelcom internal error"));
  }
}

bool qwavsample::isSilence(u_int32_t threshold=0) {

  switch (type) {
    case MONO8: return (u_int32_t)abs(sample->mono8)<=threshold;
    case MONO16: return (u_int32_t)abs(sample->mono16)<=threshold;
    case STEREO8: 
      return ((u_int32_t)abs(sample->stereo8.left)<=threshold) && 
             ((u_int32_t)abs(sample->stereo8.right)<=threshold);
    case STEREO16:
      return ((u_int32_t)abs(sample->stereo16.left)<=threshold) && 
             ((u_int32_t)abs(sample->stereo16.right)<=threshold);
  }
  throw qexception(__PRETTY_FUNCTION__,_("quelcom internal error"));
}


void qwavsample::setNext() {
  
  switch (type) {
    case MONO8: ++(char*)sample; break;
    case MONO16: ++(short*)sample; break;
    case STEREO8: ++(struct stereo8*)sample; break;
    case STEREO16: ++(struct stereo16*)sample; break;
    default:
      throw qexception(__PRETTY_FUNCTION__,_("quelcom internal error"));
  }

#ifdef QVERBOSE
  cerr << "sample now is mapped at " << (void*) sample << endl;
#endif
}


ostream& operator<<(ostream &os, qwavsample &s) {

  s.print(os);
  return os;
}

void qwavsample::print(ostream &os) {

  switch (type) {
    case MONO8: os.form("(%d)",sample->mono8); break;
    case MONO16: os.form("(%d)",sample->mono16); break;
    case STEREO8: os.form("(%d,%d)",sample->stereo8.left,sample->stereo8.right); break;
    case STEREO16: os.form("(%d,%d)",sample->stereo16.left,sample->stereo16.right); break;
    default:
      throw qexception(__PRETTY_FUNCTION__,_("quelcom internal error"));
  }
}


void qwavsample::prod(double d) {

  switch (type) {
    case MONO8: sample->mono8*=d; break;
    case MONO16: sample->mono16*=d; break;
    case STEREO8: sample->stereo8.left*=d; sample->stereo8.right*=d; break;
    case STEREO16: sample->stereo16.left*=d; sample->stereo16.right*=d; break;
    default:
      throw qexception(__PRETTY_FUNCTION__,_("quelcom internal error"));
  }
}
