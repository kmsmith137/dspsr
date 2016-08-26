/***************************************************************************
 *
 *   Copyright (C) 2016 by Kendrick Smith
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include <stdexcept>
#include "dsp/ChimeFile.h"

using namespace std;

dsp::ChimeFile::ChimeFile() : File("CHIME") 
{ 
  this->assembler_handle = 0;
}

dsp::ChimeFile::~ChimeFile() 
{ 
  if (assembler_handle) {
    delete assembler_handle;
    assembler_handle = 0;
  }
}

bool dsp::ChimeFile::is_valid(const char *filename) const
{
  cerr << "is_valid called: " << filename << endl;
  return true;
}

void dsp::ChimeFile::open_file(const char *filename)
{
  this->assembler_handle = ch_vdif_assembler::dspsr_handle::make(filename);
  cerr << "file opened! " << filename <<endl;
  throw runtime_error("BLAH");
}

/*
 * Local variables:
 *  c-basic-offset: 2
 * End:
 */
