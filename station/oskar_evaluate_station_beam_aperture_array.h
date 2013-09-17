/*
 * Copyright (c) 2012-2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef OSKAR_EVALUATE_STATION_BEAM_APERTURE_ARRAY_H_
#define OSKAR_EVALUATE_STATION_BEAM_APERTURE_ARRAY_H_

/**
 * @file oskar_evaluate_station_beam_aperture_array.h
 */

#include <oskar_global.h>
#include <oskar_station.h>
#include <oskar_WorkStationBeam.h>
#include <oskar_random_state.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Evaluates the station beam for an aperture array station.
 *
 * @details
 * This top-level function evaluates the beam for an aperture array station,
 * including any possible child stations and/or element models within the
 * station.
 *
 * The supplied station model must be fully set-up on entry, and must include
 * the element positions, the required beam coordinates, and all associated
 * options and meta-data necessary for beamforming. In particular, element
 * coordinates within the station should be in radians (i.e. pre-multiplied
 * by the wavenumber). Checks are made to ensure that data are in the correct
 * form before being used.
 *
 * The beam is evaluated at points defined by the horizontal Cartesian
 * direction cosines given in the vectors x,y,z. The output \p beam array
 * must be of the correct data type for the station element:
 * for example, it is an error to use polarised elements and supply a scalar
 * type for the output data. If in doubt, an output type of complex matrix will
 * always be acceptable, although it may be significantly slower for isotropic
 * elements.
 *
 * The work structure holds pointers to memory blocks used by routines which
 * are called by this wrapper. The structure must be initialised, but may be
 * empty. In this case, the internal buffers will be resized to the correct
 * dimensions on first use. Subsequent calls to this function should ideally
 * re-use the same work structure to ensure optimum performance and no needless
 * memory reallocation.
 *
 * The pointer to the structure holding random number states must be
 * initialised.
 *
 * Notes:
 * - random_states .. TODO how to set up this buffer
 *
 * @param[out]    beam          Station beam evaluated at x,y,z positions.
 * @param[in]     station       Fully populated station model structure.
 * @param[in]     num_points    Number of coordinates at which to evaluate
 *                              the beam.
 * @param[in]     x             Array of horizontal x coordinates at which to
 *                              evaluate the beam.
 * @param[in]     y             Array of horizontal y coordinates at which to
 *                              evaluate the beam.
 * @param[in]     z             Array of horizontal z coordinates at which to
 *                              evaluate the beam.
 * @param[in]     gast          The Greenwich Apparent Sidereal Time (as MJD).
 *                              NOTE may soon be replaced with a structure for
 *                              more precise equatorial to horizontal coordinate
 *                              conversion.
 * @param[in]     work          Initialised structure containing temporary work
 *                              buffers.
 * @param[in]     random_states Initialised structure of CURAND random-number
 *                              states used to model various errors at the
 *                              station-level.
 * @param[in,out] status        Status return code.
 */
OSKAR_EXPORT
void oskar_evaluate_station_beam_aperture_array(oskar_Mem* beam,
        const oskar_Station* station, int num_points, const oskar_Mem* x,
        const oskar_Mem* y, const oskar_Mem* z, double gast,
        oskar_WorkStationBeam* work, oskar_RandomState* random_states,
        int* status);

#ifdef __cplusplus
}
#endif

#endif /* OSKAR_EVALUATE_STATION_BEAM_APERTURE_ARRAY_H_ */
