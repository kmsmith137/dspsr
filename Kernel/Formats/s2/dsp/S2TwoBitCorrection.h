//-*-C++-*-

/* $Source: /cvsroot/dspsr/dspsr/Kernel/Formats/s2/dsp/S2TwoBitCorrection.h,v $
   $Revision: 1.2 $
   $Date: 2002/10/07 01:48:37 $
   $Author: wvanstra $ */

#ifndef __S2TwoBitCorrection_h
#define __S2TwoBitCorrection_h

#include <vector>

#include "TwoBitCorrection.h"

namespace dsp {

  class TwoBitTable;

  //! Converts a S2 Timeseries from 2-bit digitized to floating point values
  class S2TwoBitCorrection: public TwoBitCorrection {

  public:

    //! Constructor
    S2TwoBitCorrection (TwoBitTable::Type type = TwoBitTable::SignMagnitude);

  protected:

    //! Unpacking interface
    void unpack ();
  };
  
}

#endif
