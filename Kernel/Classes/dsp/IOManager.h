//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2002 by Aidan Hotan
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* $Source: /cvsroot/dspsr/dspsr/Kernel/Classes/dsp/IOManager.h,v $
   $Revision: 1.18 $
   $Date: 2006/07/09 13:27:10 $
   $Author: wvanstra $ */


#ifndef __IOManager_h
#define __IOManager_h

#include "dsp/Operation.h"

namespace dsp {

  class Observation;
  class Input;
  class BitSeries;
  class Unpacker;
  class TimeSeries;

  //! Convenience interface to different file formats
  /*! This class defines a common interface to backend-specific file
    formats by constructing the appropriate header-loading and
    data-unpacking routines.  */
  class IOManager : public Operation {

  public:
    
    //! Constructor
    IOManager ();
    
    //! Destructor
    virtual ~IOManager ();
    
    //! Prepare the appropriate Input and Unpacker
    virtual void open (const string& id);

    //! Get the information about the data source
    const Observation* get_info () const;
    Observation* get_info ();

    //! Return pointer to the appropriate Input
    const Input* get_input () const;
    Input* get_input ();
 
    //! Set the Input operator (should not normally need to be used)
    void set_input (Input* input);

     //! Return pointer to the appropriate Unpacker
    const Unpacker* get_unpacker () const;
    Unpacker* get_unpacker ();

    //! Set the Unpacker (should not normally need to be used)
    void set_unpacker (Unpacker* unpacker);

    //! Set the TimeSeries into which data will be loaded and unpacked
    virtual void set_output (TimeSeries* output);

    //! Set the BitSeries into which data will be loaded
    // (should not normally need to be used)
    virtual void set_output (BitSeries* output);

    //! Load and convert the next block of data
    virtual void load (TimeSeries* data);


  protected:

    //! Load the BitSeries and/or TimeSeries specified with set_output
    virtual void operation ();

    //! Appropriate Input subclass
    Reference::To<Input> input;

    //! Appropriate Unpacker subclass
    Reference::To<Unpacker> unpacker;

    //! The container in which the TimeSeries data is unpacked
    Reference::To<BitSeries> output;

    //! The container in which the TimeSeries data is unpacked
    Reference::To<TimeSeries> data;

  };

}

#endif // !defined(__IOManager_h)
