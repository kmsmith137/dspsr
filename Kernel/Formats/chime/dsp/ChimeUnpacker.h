//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2016 by Kendrick Smith
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __ChimeUnpacker_h
#define __ChimeUnpacker_h

#include "dsp/Unpacker.h"

namespace dsp {

  class ChimeUnpacker : public Unpacker {
  public:
    ChimeUnpacker() { }
    virtual ~ChimeUnpacker() { }

  protected:
    virtual bool matches(const Observation *observation);
    virtual void unpack();
  };

}

#endif // !defined(__ChimeUnpacker_h)

/*
 * Local variables:
 *  c-basic-offset: 2
 * End:
 */
