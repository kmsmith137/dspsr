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
  
  Observation *obs = this->get_info();
  
  // Linear polarization (see chimer:/usr/local/include/Basis.h)
  obs->set_basis(Signal::Linear);

  // I'm not sure if this is correct!
  // "In-phase and Quadrature sampled voltages" (see chimer:/usr/local/include/Types.h)
  obs->set_state(Signal::Analytic);
  
  // The other plausible option here is Signal::Unknown (see chimer:/usr/local/include/Types.h)
  obs->set_type(Signal::Pulsar);

  // The "2" here means { real, complex }.
  obs->set_ndim(2);

  obs->set_nchan(ch_vdif_assembler::dspsr_handle::get_nchan());
  obs->set_npol(2);
  obs->set_nbit(4);
  obs->set_telescope("CHIME");
  obs->set_centre_frequency(600.);   // MHz
  obs->set_rate(ch_vdif_assembler::dspsr_handle::get_rate());

  // I'm not sure if these values are correct!
  obs->set_bandwidth(400.);   // "negative = lsb; positive = usb"
  obs->set_swap(true);        // "set true if frequency channels are out of order"

  //
  // Note: the following members of class Observation are not initialized.
  //   - ndat, since there is no way to set it for a real-time stream
  //   - source, dispersion_measure, rotation_measure
  //   - coordinates, start_time
  //   - a bunch of strings which didn't seem important (receiver, identifier, etc.)
  //
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
