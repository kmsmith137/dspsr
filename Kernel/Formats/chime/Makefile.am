noinst_LTLIBRARIES = libchime.la

nobase_include_HEADERS =  dsp/ChimeFile.h dsp/ChimeUnpacker.h

libchime_la_SOURCES =  ChimeFile.C ChimeUnpacker.C

libchime_la_LIBADD = -lch_vdif_assembler -lch_frb_io -lhdf5 -lpng

include $(top_srcdir)/config/Makefile.include
