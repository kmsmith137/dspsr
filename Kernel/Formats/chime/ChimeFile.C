/***************************************************************************
 *
 *   Copyright (C) 2016 by Kendrick Smith
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include <stdexcept>
#include "dsp/ChimeFile.h"

using namespace std;

inline bool endswith(const string &str, const string &suffix)
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

  obs->set_nchan(ch_vdif_assembler::dspsr_handle::nfreq);
  obs->set_npol(2);
  obs->set_nbit(4);
  obs->set_telescope("CHIME");
  obs->set_centre_frequency(600.);   // MHz
  obs->set_rate(ch_vdif_assembler::dspsr_handle::sampling_rate_Hz);

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


void dsp::ChimeFile::open(const char *filelist_filename)
{
  if (assembler_handle)
    throw runtime_error("internal error: ChimeFile:open() was called twice?!");

  this->assembler_handle = ch_vdif_assembler::dspsr_handle::make(filelist_filename);
}


void dsp::ChimeFile::close()
{
  throw runtime_error("ChimeFile: File::close() called");
}


bool dsp::ChimeFile::eod()
{
  throw runtime_error("ChimeFile: Input::eod() called");
}


void dsp::ChimeFile::load_data(BitSeries *data)
{
  throw runtime_error("ChimeFile: Input::load_data() called");
}


void dsp::ChimeFile::seek(int64_t offset, int whence)
{
  throw runtime_error("ChimeFile: Input::seek() called");
}


// -------------------------------------------------------------------------------------------------


void dsp::ChimeFile::copy(const Input *input)
{
  throw runtime_error("ChimeFile: Input::copy() called");
}

void dsp::ChimeFile::seek(MJD mjd)
{
  throw runtime_error("ChimeFile: Input::seek() called");
}

void dsp::ChimeFile::rewind()
{
  throw runtime_error("ChimeFile: Seekable::rewind() called");
}

void dsp::ChimeFile::set_eod(bool eod)
{
  throw runtime_error("ChimeFile: Seekable::set_eod() called");
}

int64_t dsp::ChimeFile::load_bytes(unsigned char* buffer, uint64_t bytes)
{
  throw runtime_error("ChimeFile: Seekable::load_bytes() called");
}

int64_t dsp::ChimeFile::seek_bytes(uint64_t bytes)
{
  throw runtime_error("ChimeFile: Seekable::seek_bytes() called");
}

uint64_t dsp::ChimeFile::recycle_data(BitSeries* data)
{
  throw runtime_error("ChimeFile: Seekable::recycle_data() called");
}

uint64_t dsp::ChimeFile::get_current_sample()
{
  throw runtime_error("ChimeFile: Seekable::get_current_sample() called");
}

void dsp::ChimeFile::reopen()
{
  throw runtime_error("ChimeFile: File::reopen() called");
}

void dsp::ChimeFile::open_file(const char *filelist_filename)
{
  throw runtime_error("ChimeFile: File::open_file() called");
}

int64_t dsp::ChimeFile::fstat_file_ndat(uint64_t tailer_bytes)
{
  throw runtime_error("ChimeFile: File::fstat_file_ndat() called");
}

int64_t dsp::ChimeFile::pad_bytes(unsigned char *buffer, int64_t bytes)
{
  throw runtime_error("ChimeFile: File::pad_bytes() called");
}

void dsp::ChimeFile::set_total_samples()
{
  throw runtime_error("ChimeFile: File::set_total_samples() called");
}

void dsp::ChimeFile::open_fd(const string &filename)
{
  throw runtime_error("ChimeFile: File::open_fd() called");
}


/*
 * Local variables:
 *  c-basic-offset: 2
 * End:
 */
