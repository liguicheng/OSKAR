/*
 * Copyright (c) 2016, The University of Oxford
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

#ifndef OSKAR_IMAGER_PHASE_ROTATE_H_
#define OSKAR_IMAGER_PHASE_ROTATE_H_

/**
 * @file oskar_imager_phase_rotate.h
 */

#include <oskar_global.h>
#include <oskar_mem.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Performs visibility amplitude phase rotation.
 *
 * @details
 * Note that the coordinates uu, vv, ww are in wavelengths.
 *
 * Ref:
 * Cornwell, T.J., & Perley, R.A., 1992,
 * "Radio-interferometric imaging of very large fields"
 */
OSKAR_EXPORT
void oskar_imager_phase_rotate(size_t num_vis, const oskar_Mem* uu,
        const oskar_Mem* vv, const oskar_Mem* ww, oskar_Mem* amps,
        double delta_l, double delta_m, double delta_n);

#ifdef __cplusplus
}
#endif

#endif /* OSKAR_IMAGER_PHASE_ROTATE_H_ */
