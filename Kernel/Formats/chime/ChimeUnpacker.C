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
  throw runtime_error("ChimeUnpacker::unpack() called");
}


/*
 * Local variables:
 *  c-basic-offset: 2
 * End:
 */
