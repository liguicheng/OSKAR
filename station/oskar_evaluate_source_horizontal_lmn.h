/*
 * Copyright (c) 2011-2013, The University of Oxford
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

#ifndef OSKAR_EVALUATE_SOURCE_HORIZONTAL_LMN_H_
#define OSKAR_EVALUATE_SOURCE_HORIZONTAL_LMN_H_

/**
 * @file oskar_evaluate_source_horizontal_lmn.h
 */

#include <oskar_global.h>
#include <oskar_mem.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Evaluates horizontal direction cosines for sources at the given time.
 *
 * @details
 * This function converts source positions from equatorial (RA, Dec)
 * coordinates to horizontal direction cosines at the specified local
 * apparent sidereal time and latitude.
 *
 * @param[in] num_sources The number of source positions to process.
 * @param[out] l          Source horizontal l direction cosines (x-components).
 * @param[out] m          Source horizontal m direction cosines (y-components).
 * @param[out] n          Source horizontal n direction cosines (z-components).
 * @param[in]  RA         Source Right Ascension values.
 * @param[in]  Dec        Source Declination values.
 * @param[in]  last       The local apparent sidereal time, in radians.
 * @param[in]  latitude   The observer's geodetic latitude, in radians.
 * @param[in,out]  status Status return code.
 */
OSKAR_EXPORT
void oskar_evaluate_source_horizontal_lmn(int num_sources, oskar_Mem* l,
        oskar_Mem* m, oskar_Mem* n, const oskar_Mem* RA, const oskar_Mem* Dec,
        double last, double latitude, int* status);

#ifdef __cplusplus
}
#endif

#endif /* OSKAR_EVALUATE_SOURCE_HORIZONTAL_LMN_H_ */
