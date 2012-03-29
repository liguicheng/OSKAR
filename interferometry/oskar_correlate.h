/*
 * Copyright (c) 2011, The University of Oxford
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

#ifndef OSKAR_CORRELATE_H_
#define OSKAR_CORRELATE_H_

/**
 * @file oskar_correlate.h
 */


#include "oskar_global.h"
#include "interferometry/oskar_Visibilities.h"
#include "interferometry/oskar_TelescopeModel.h"
#include "math/oskar_Jones.h"
#include "sky/oskar_SkyModel.h"
#include "utility/oskar_Mem.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Multiply a set of Jones matrices with a set of source brightness
 * matrices to form visibilities (i.e. V = J B J).
 *
 * @details
 * The source brightness matrices are specified in by means of an OSKAR sky
 * model.
 *
 * The Jones matrices should have dimensions corresponding to the number of
 * sources in the brightness matrix and the number of stations.
 *
 * Station u,v coordinates are specified in radians at the frequency of the
 * simulation (i.e. multipled by the wavenumber).
 *
 * @param[out] vis        Visibility amplitudes.
 * @param[in]  J          Set of Jones matrices/
 * @param[in]  telescope  OSKAR telescope model.
 * @param[in]  sky        OSKAR sky model.
 * @param[in]  u          Station u coordinates, in radians.
 * @param[in]  v          Station v coordinates, in radians.
 *
 * @return An error code.
 */
OSKAR_EXPORT
int oskar_correlate(oskar_Mem* vis, const oskar_Jones* J,
        const oskar_TelescopeModel* telescope, const oskar_SkyModel* sky,
        const oskar_Mem* u, const oskar_Mem* v);

#ifdef __cplusplus
}
#endif

#endif /* OSKAR_CORRELATE_H_ */
