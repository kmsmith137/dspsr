/***************************************************************************
 *
 *   Copyright (C) 2016 by Kendrick Smith
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include <stdexcept>
#include "dsp/ChimeFile.h"

using namespace std;

inline bool endswith(const std::string &str, const std::string &suffix)
{
  return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}


// -------------------------------------------------------------------------------------------------


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


bool dsp::ChimeFile::is_valid(const char *filelist_filename) const
{
  if (endswith(filelist_filename, ".txt"))
    return true;

  std::cerr << "warning: filelist_filename '" << filelist_filename << "' doesn't end in .txt, this is not a valid chime filelist_filename\n";
  return false;
}


void dsp::ChimeFile::open_file(const char *filelist_filename)
{
  if (assembler_handle)
    throw runtime_error("internal error: ChimeFile:open_file() was called twice?!");

  this->assembler_handle = ch_vdif_assembler::dspsr_handle::make(filelist_filename);
  cerr << "assembler_handle constructed! " << filelist_filename <<endl;
}


// Overrides File::set_total_samples().  We have no way of determining the sample count
// in advance, so we just don't initialize it.  I think this is OK, since class DADABuffer
// does the same thing.
void dsp::ChimeFile::set_total_samples()
{
  return;
}


/*
 * Local variables:
 *  c-basic-offset: 2
 * End:
 */
