/***************************************************************************
 *
 *   Copyright (C) 2016 by Kendrick Smith
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include <stdexcept>
#include <ch_vdif_assembler_dspsr.hpp>

#include "dsp/ChimeUnpacker.h"

using namespace std;


bool dsp::ChimeUnpacker::matches(const Observation *observation)
{
  return observation->get_machine() == "CHIME";
}


void dsp::ChimeUnpacker::unpack()
{
  // Sanity checks on input

  if (input->get_nchan() != (unsigned)ch_vdif_assembler::dspsr_handle::nfreq)
    throw Error(InvalidParam, "ChimeUnpacker::unpack", "unexpected value of input->nchan");
  if (input->get_npol() != 2)
    throw Error(InvalidParam, "ChimeUnpacker::unpack", "unexpected value of input->npol");
  if (input->get_ndim() != 2)
    throw Error(InvalidParam, "ChimeUnpacker::unpack", "unexpected value of input->ndim");
  if (input->get_ndat() != (unsigned)ch_vdif_assembler::dspsr_handle::nt_chunk)
    throw Error(InvalidParam, "ChimeUnpacker::unpack", "unexpected value of input->ndat");
  if (input->get_nbit() != 4)
    throw Error(InvalidParam, "ChimeUnpacker::unpack", "unexpected value of input->nbit");

  // Sanity checks on output

  if (output->get_order() != TimeSeries::OrderFPT)
    throw Error(InvalidParam, "ChimeUnpacker::unpack", "unexpected value of output->order");
  if (output->get_nchan() != (unsigned)ch_vdif_assembler::dspsr_handle::nfreq)
    throw Error(InvalidParam, "ChimeUnpacker::unpack", "unexpected value of output->nchan");
  if (output->get_npol() != 2)
    throw Error(InvalidParam, "ChimeUnpacker::unpack", "unexpected value of output->npol");
  if (output->get_ndim() != 2)
    throw Error(InvalidParam, "ChimeUnpacker::unpack", "unexpected value of output->ndim");
  if (output->get_ndat() != (unsigned)ch_vdif_assembler::dspsr_handle::nt_chunk)
    throw Error(InvalidParam, "ChimeUnpacker::unpack", "unexpected value of output->ndat");

  // Unpack logic.  Conveniently, ch_vdif_assembler keeps the data in the same
  // (freq, pol, time, dim) order as dspsr.

  for (int ifreq = 0; ifreq < ch_vdif_assembler::dspsr_handle::nfreq; ifreq++) {
    for (int ipol = 0; ipol < 2; ipol++) {
      float *dst = output->get_datptr(ifreq, ipol);
      const uint8_t *src = input->get_rawptr() + (2*ifreq+ipol) * ch_vdif_assembler::dspsr_handle::nt_chunk;

      // FIXME should write assembly language kernel in ch_vdif_assembler_kernels.hpp
      for (int i = 0; i < ch_vdif_assembler::dspsr_handle::nt_chunk; i++) {
	if (src[i] == 0) {
	  // invalid entry
	  dst[2*i] = dst[2*i+1] = 0.0;
	}
	else {
	  // offset decode
	  dst[2*i] = (float)((src[i] & 0xf0) >> 4) - 8.0;
	  dst[2*i+1] = (float)(src[i] & 0x0f) - 8.0;
	}
      }
    }
  }
}


/*
 * Local variables:
 *  c-basic-offset: 2
 * End:
 */
