/***************************************************************************
 *
 *   Copyright (C) 2013 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "dsp/LaunchConfig.h"
#include "Error.h"

void CUDA::LaunchConfig::init ()
{
  if (device >= 0)
    return;

  cudaGetDevice(&device);
  cudaGetDeviceProperties (&device_properties, device);
}

void CUDA::LaunchConfig1D::set_nelement (unsigned N)
{
  unsigned max_nthread = device_properties.maxThreadsPerBlock;
  unsigned min_nthread = device_properties.warpSize;

  nthread = max_nthread;
  if (nthread > N)
    nthread = N;
  if (nthread < min_nthread)
    nthread = min_nthread;

  unsigned max_nblock = device_properties.maxGridSize[block_dim];

  nblock = N / nthread;
  if (N % nthread)
    nblock ++;

  if (nblock > max_nblock)
    throw Error (InvalidState, "CUDA::LaunchConfig1D::set_nelement",
		 "nblock=%u > maxGridSize[%u]=%u",
		 nblock, block_dim, max_nblock);
}
