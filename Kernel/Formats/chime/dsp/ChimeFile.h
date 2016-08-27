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
#include <ch_vdif_assembler_dspsr.hpp>

namespace dsp {

  class ChimeFile : public File
  {
  public:
    ChimeFile();
    virtual ~ChimeFile();
	  
    // All of these are overrides of File virtuals.
    virtual bool is_valid(const char* filelist_filename) const;
    virtual void open_file(const char* filelist_filename);
    virtual void set_total_samples();

  protected:
    ch_vdif_assembler::dspsr_handle *assembler_handle;
  };

}

#endif // !defined(__ChimeFile_h)

/*
 * Local variables:
 *  c-basic-offset: 2
 * End:
 */
