//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2016 by Kendrick Smith
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __ChimeFile_h
#define __ChimeFile_h

#include "dsp/File.h"
#include <ch_vdif_assembler_dspsr.hpp>

namespace dsp {

  // Note: we use a subclass of File, even though a subclass of Input would make more sense.
  // This is so we can add ChimeFile to the File registry, and access it through the usual dspsr
  // command-line syntax (a similar kludge is used in e.g. class DADABuffer).

  class ChimeFile : public File
  {
  public:
    ChimeFile();
    virtual ~ChimeFile();

    // File virtuals.  Note that it's slightly easier to override open() than open_file().
    virtual bool is_valid(const char *filelist_filename) const;
    virtual void open(const char *filelist_filename);
    virtual void close();

    // Input virtuals.
    virtual bool eod();
    virtual void load_data(BitSeries *data);

    //
    // Here is a list of Input virtuals which we don't override, since the default implementations seem OK.
    //   seek(): can't actually override this, since 'load_sample' is a private (not protected) member of class Input
    //   get_output(), set_output(), has_output()
    //   get_block_size(), set_block_size()
    //   get_overlap(), set_overlap(), get_stride()
    //   get_info(), get_total_samples()
    //   operation()
    //   mark_output()

    // 
    // Everything after this point is a "non-virtual": a virtual function which has been overridden to
    // throw an exception.  This is because according to my understanding none of these functions should
    // ever be called, so I like having the exception as a signal to revisit and understand things.
    //
    virtual void copy(const Input *input);
    virtual void seek(MJD mjd);

    // Seekable virtuals which currently throw exceptions.
    virtual void rewind();
    virtual void set_eod(bool eod);
    virtual int64_t load_bytes(unsigned char *buffer, uint64_t bytes);
    virtual int64_t seek_bytes(uint64_t bytes);
    virtual uint64_t recycle_data(BitSeries *data);
    virtual uint64_t get_current_sample();
    
    // File virtuals which currently throw exceptions.
    virtual void reopen();
    virtual void open_file(const char *filelist_filename);
    virtual int64_t fstat_file_ndat(uint64_t tailer_bytes);
    virtual int64_t pad_bytes(unsigned char *buffer, int64_t bytes);
    virtual void set_total_samples();   // note that get_total_samples() is a member of Input, but set_total_samples() is a member of File
    virtual void open_fd(const std::string &filename);

  protected:
    ch_vdif_assembler::dspsr_handle *assembler_handle;
  };
}

#endif // !defined(__ChimeFile_h)

/*
 * Local variables:
 *  c-basic-offset: 2
 * End:
 */
