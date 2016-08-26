//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2016 by Kendrick Smith
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __ChimeInput_h
#define __ChimeInput_h

#include "dsp/Input.h"
#include <ch_vdif_assembler_dspsr.hpp>

namespace dsp {

  class ChimeInput : public Input
  {
  public:
    ChimeInput(const std::string &filelist_filename);
    virtual ~ChimeInput();

    // We only define the pure virtuals for now
    virtual bool eod();
    virtual void set_eod(bool);
    virtual void load_data(BitSeries *data);

  protected:
    ch_vdif_assembler::dspsr_handle *assembler_handle;
  };

}

#endif // !defined(__ChimeInput_h)

/*
 * Local variables:
 *  c-basic-offset: 2
 * End:
 */
