This is a fork of the [dspsr pulsar timing code](https://github.com/demorest/dspsr) which defines a 'chime'
input class for processing CHIME beamformed data.  It uses the [ch_frb_io](https://github.com/CHIMEFRB/ch_frb_io)
and [ch_vdif_assembler](https://github.com/kmsmith137/ch_vdif_assembler) libraries.

Current status: basic classes are defined, next step is to test pulsar folding on a simple simulated example
before trying real data.  This will be easiest with the help of someone who is familiar with dspsr and its prerequisites!

### Installation

The following works for me on chimer, but note that some prerequisites have already been installed (e.g. psrdada).

 - External prerequisites: libhdf5, libpng

 - CHIME-specific prerequisite: [ch_frb_io](https://github.com/CHIMEFRB/ch_frb_io), see installation instructions in
   its README.  Note that a reasonably up-to-date version is already installed in chimer:/usr/local, so you probably
   don't need to compile it.

 - CHIME-specific prerequisite: [ch_vdif_assembler](https://github.com/kmsmith137/ch_vdif_assembler), see installation 
   instructions in its README.  Note that a reasonably up-to-date version is already installed in chimer:/usr/local, 
   so you probably don't need to compile it.

 - Then do the following:
```
  cd dspsr
  echo chime > backends.list
  ./bootstrap

  # if you're not using the /usr/local version of ch_vdif_assembler, you may need to do prepend 
  # e.g. CPPFLAGS=-I$HOME/include LDFLAGS=-L$HOME/lib
  ./configure --prefix=$HOME

  make
  make install
```

 - More instructions to follow when we have a working example.

### To do

  - First we need to get a simple example working, perhaps with simulated first and then a real
    CHIME B0329 observation.

  - Right now the code is CPU-only, need to figure out how to put the BitSeries in GPU memory
    and run the Unpacker on the GPU.

  - Right now we can't run on a realtime network stream, only a disk capture.  (The realtime case
    is implemented in the ch_vdif_assembler library, and very little code is required on the dspsr
    side, but I'm guessing that won't actually be able to keep up with the realtime data until
    GPU processing is implemented.)

  - Right now we don't have real timestamp info, this will undoubtedly be an issue soon.

  - Autoconf loose end: libch_frb_io should be an optional prerequisite

  - Loose end: Is there any advantage to implementing a HistUnpacker?

  - Loose end: Is there a way to avoid the overhead of copying the bitstream from ch_vdif_assembler-managed
    memory to dspsr-managed memory?  (Low priority.)

  - Loose end: I'm unclear on a few Observation initializations in the ChimeFile constructor, in particular
    the channel ordering and the Fourier phase convention.

  - Loose end: if good CPU performance is ever a priority, an assembly language unpacking kernel will probably help.

  - The ch_vdif_assembler::dspsr_handle API could be slightly improved by replacing advance() by a pair
    of functions, one which releases the old chunk, and one which waits asynchronously for the new chunk to arrive.
