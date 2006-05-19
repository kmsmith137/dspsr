//-*-C++-*-

#ifndef __ASPFile_h
#define __ASPFile_h

#include "dsp/BlockFile.h"

namespace dsp {

  //! Loads BitSeries data from a MkV file
  class ASPFile : public BlockFile
  {
  public:
	  
    //! Construct and open file	  
    ASPFile (const char* filename=0, const char* headername=0);
	  
    //! Destructor
    ~ASPFile ();
	  
    //! Returns true if filename is a valid Mk5 file
    bool is_valid (const char* filename, int NOT_USED=-1) const;

  protected:

    //! Open the file
    void open_file (const char* filename);

    void skip_extra ();

  };

}

#endif // !defined(__ASPFile_h)
