//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2005 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __InputBuffering_h
#define __InputBuffering_h

#include "dsp/BufferingPolicy.h"
#include "dsp/TimeSeries.h"
#include "dsp/Transformation.h"

namespace dsp {

  //! Buffers the Transformation input
  class InputBuffering : public BufferingPolicy {

  public:
    
    //! Default constructor
    InputBuffering (HasInput<TimeSeries>* target = 0);

    //! Set the target with input TimeSeries to be buffered
    void set_target (HasInput<TimeSeries>* input);
    
    //! Perform all buffering tasks required before transformation
    void pre_transformation ();
    
    //! Perform all buffering tasks required after transformation
    void post_transformation ();
    
    //! Set the first sample to be used from the input next time
    void set_next_start (uint64_t next_start_sample);
    
    //! Set the minimum number of samples that can be processed
    void set_minimum_samples (uint64_t samples);

    //! Get the next contiguous sample following the current buffer
    int64_t get_next_contiguous () const;

    //! Shares a single InputBuffering instance between multiple threads
    class Share;

  protected:
    
    //! The next start sample
    uint64_t next_start_sample;

    //! The requested number of samples to be reserved in the input
    uint64_t requested_reserve;

    //! The minimum number of samples that can be processed
    uint64_t minimum_samples;

    //! The target with input TimeSeries to be buffered
    HasInput<TimeSeries>* target;
    
    //! The buffer
    Reference::To<TimeSeries> buffer;
    
  };

}

#endif // !defined(__InputBuffering_h)
