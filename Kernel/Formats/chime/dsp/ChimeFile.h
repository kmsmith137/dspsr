//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2016 by Kendrick Smith
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __ChimeFile_h
#define __ChimeFile_h

#include "dsp/File.h"

namespace dsp {

  class ChimeFile : public File
  {
  public:
    ChimeFile();
    virtual ~ChimeFile();
	  
    virtual bool is_valid(const char* filename) const;
    virtual void open_file(const char* filename);

  protected:
  };

}

#endif // !defined(__ChimeFile_h)

/*
 * Local variables:
 *  c-basic-offset: 2
 * End:
 */
