/***************************************************************************
 *
 *   Copyright (C) 2016 by Kendrick Smith
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include <cstring>
#include <stdexcept>
#include "dsp/ChimeFile.h"
#include "dsp/BitSeries.h"

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
  obs->set_machine("CHIME");
  obs->set_centre_frequency(600.);   // MHz
  obs->set_rate(ch_vdif_assembler::dspsr_handle::sampling_rate_Hz);

  // I'm not sure if these values are correct!
  obs->set_bandwidth(400.);   // "negative = lsb; positive = usb"
  obs->set_swap(true);        // "set true if frequency channels are out of order"

  // The stream size is not known in advance, so let's try a hack: as data arrives,
  // we call Observation::set_ndat() to keep the stream size equal to the amount of
  // data processed so far, plus one chunk.  (See ChimeFile::load_data() below.)
  obs->set_ndat(ch_vdif_assembler::dspsr_handle::nt_chunk);

  // This must come after the call to Observation::set_ndat().
  this->set_block_size(ch_vdif_assembler::dspsr_handle::nt_chunk);

  // Note: the following members of class Observation are not initialized.
  //   - source, dispersion_measure, rotation_measure
  //   - coordinates, start_time
  //   - a bunch of strings which didn't seem important (receiver, identifier, etc.)
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
    throw Error(InvalidState, "ChimeFile::open", "stream already open");

  this->assembler_handle = ch_vdif_assembler::dspsr_handle::make(filelist_filename);
}


void dsp::ChimeFile::close()
{
  if (!assembler_handle)
    throw Error(InvalidState, "ChimeFile::close", "stream already closed (or never opened)");

  delete assembler_handle;
  this->assembler_handle = 0;
}


bool dsp::ChimeFile::eod()
{
  if (!assembler_handle)
    throw Error(InvalidState, "ChimeFile::eod", "stream closed, or never opened");

  return (assembler_handle->curr_chunk_ix >= 0) && (assembler_handle->curr_data == NULL);
}


void dsp::ChimeFile::load_data(BitSeries *data)
{
  uint64_t chunk_nbytes = 2 * ch_vdif_assembler::dspsr_handle::nfreq * ch_vdif_assembler::dspsr_handle::nt_chunk;

  if (!assembler_handle)
    throw Error(InvalidState, "ChimeFile::load_data", "stream closed, or never opened");

  if (assembler_handle->curr_chunk_ix < 0)
    assembler_handle->advance();  // start stream and read first chunk

  if ((int64_t)get_load_sample() != assembler_handle->curr_chunk_ix * ch_vdif_assembler::dspsr_handle::nt_chunk)
    throw Error(InvalidParam, "ChimeFile::load_data", "unexpected value of load_sample");
  if ((int64_t)get_load_size() != ch_vdif_assembler::dspsr_handle::nt_chunk)
    throw Error(InvalidParam, "ChimeFile::load_data", "unexpected value of load_size");
  if (assembler_handle->curr_data == NULL)
    throw Error(InvalidState, "ChimeFile::load_data", "called at end-of-stream");

  // Some checks on the BitSeries.
  if ((data->get_nchan() != (unsigned)ch_vdif_assembler::dspsr_handle::nfreq) || (data->get_npol() != 2) || (data->get_ndim() != 2) || (data->get_nbit() != 4) || !data->get_rawptr())
    throw Error(InvalidState, "ChimeFile::load_data", "BitSeries failed sanity test");
  if (data->get_size() < chunk_nbytes)
    throw Error(InvalidState, "ChimeFile::load_data", "BitSeries is underallocated");    

  // Just copy the data here -- reordering and unpacking to float is done in ChimeUnpacker.
  memcpy(data->get_rawptr(), assembler_handle->curr_data, chunk_nbytes);
  data->set_ndat(ch_vdif_assembler::dspsr_handle::nt_chunk);

  // Advance to next chunk.
  assembler_handle->advance();

  if (assembler_handle->curr_data != NULL)
    get_info()->set_ndat((assembler_handle->curr_chunk_ix+1) * ch_vdif_assembler::dspsr_handle::nt_chunk);
}


// -------------------------------------------------------------------------------------------------
//
// Long list of "non-virtuals" (see comment in ChimeFile.h)


void dsp::ChimeFile::copy(const Input *input)
{
  throw Error(InvalidState, "Chimefile::copy", "virtual function not implemented, see ChimeFile.C");
}

void dsp::ChimeFile::seek(MJD mjd)
{
  throw Error(InvalidState, "Chimefile::seek(MJD)", "virtual function not implemented, see ChimeFile.C");
}

void dsp::ChimeFile::rewind()
{
  throw Error(InvalidState, "ChimeFile::rewind", "virtual function not implemented, see ChimeFile.C");
}

void dsp::ChimeFile::set_eod(bool eod)
{
  throw Error(InvalidState, "ChimeFile::set_eod", "virtual function not implemented, see ChimeFile.C");
}

int64_t dsp::ChimeFile::load_bytes(unsigned char* buffer, uint64_t bytes)
{
  throw Error(InvalidState, "ChimeFile::load_bytes", "virtual function not implemented, see ChimeFile.C");
}

int64_t dsp::ChimeFile::seek_bytes(uint64_t bytes)
{
  throw Error(InvalidState, "ChimeFile::seek_bytes", "virtual function not implemented, see ChimeFile.C");
}

uint64_t dsp::ChimeFile::recycle_data(BitSeries* data)
{
  throw Error(InvalidState, "ChimeFile::recycle_data", "virtual function not implemented, see ChimeFile.C");
}

uint64_t dsp::ChimeFile::get_current_sample()
{
  throw Error(InvalidState, "ChimeFile::get_current_sample", "virtual function not implemented, see ChimeFile.C");
}

void dsp::ChimeFile::reopen()
{
  throw Error(InvalidState, "ChimeFile::reopen", "virtual function not implemented, see ChimeFile.C");
}

void dsp::ChimeFile::open_file(const char *filelist_filename)
{
  throw Error(InvalidState, "ChimeFile::open_file", "virtual function not implemented, see ChimeFile.C");
}

int64_t dsp::ChimeFile::fstat_file_ndat(uint64_t tailer_bytes)
{
  throw Error(InvalidState, "ChimeFile::fstat_file_ndat", "virtual function not implemented, see ChimeFile.C");
}

int64_t dsp::ChimeFile::pad_bytes(unsigned char *buffer, int64_t bytes)
{
  throw Error(InvalidState, "ChimeFile::pad_bytes", "virtual function not implemented, see ChimeFile.C");
}

void dsp::ChimeFile::set_total_samples()
{
  throw Error(InvalidState, "ChimeFile::set_total_samples", "virtual function not implemented, see ChimeFile.C");
}

void dsp::ChimeFile::open_fd(const string &filename)
{
  throw Error(InvalidState, "ChimeFile::open_fd", "virtual function not implemented, see ChimeFile.C");
}


/*
 * Local variables:
 *  c-basic-offset: 2
 * End:
 */
