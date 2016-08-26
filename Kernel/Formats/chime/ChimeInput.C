/***************************************************************************
 *
 *   Copyright (C) 2016 by Kendrick Smith
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include <stdexcept>
#include "dsp/ChimeInput.h"

using namespace std;


dsp::ChimeInput::ChimeInput(const std::string &filelist)
  : Input("chime")
{
  std::cerr << "ChimeInput constructed: " << filelist << "\n";
}

// Placeholder
dsp::ChimeInput::~ChimeInput() { }

bool dsp::ChimeInput::eod()
{
  throw std::runtime_error("ChimeInput::eod() called");
}

void dsp::ChimeInput::set_eod(bool eod)
{
  throw std::runtime_error("ChimeInput::set_eod() called");
}

void dsp::ChimeInput::load_data(dsp::BitSeries *data)
{
  throw std::runtime_error("ChimeInput::load_data() called");  
}


/*
 * Local variables:
 *  c-basic-offset: 2
 * End:
 */
