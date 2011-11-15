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

#ifndef OSKAR_VISIBILITIES_INSERT_H_
#define OSKAR_VISIBILITIES_INSERT_H_

/**
 * @file oskar_visibilities_insert.h
 */


#include "oskar_global.h"
#include "interferometry/oskar_Visibilities.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Inserts the specified visibility structure at the specified
 * time index.
 *
 * @details
 * The structure being inserted must contain the same baseline
 * and channel dimensions are the structure being appended to.
 * To use this function the visibility structure being inserted must
 * be preallocated to sufficiently large dimensions that the time index
 * plus the number of times in the structure being inserted doesn't exceed
 * the number of times in the destination.
 *
 * FIXME is this used for anything anymore? - if not deprecate it.
 * NOTE This function is potentially dangerous as it allows for inserting
 * data out of time and frequency order.
 *
 * @param[out] dst             Visibility structure to insert into.
 * @param[in]  src             Visibility structure to insert.
 * @param[in]  channel_index   Channel index which to insert at.
 * @param[in]  time_index      Time index which to insert at.
 *
 * @return An error code.
 */
OSKAR_EXPORT
int oskar_visibilities_insert(oskar_Visibilities* dst,
        const oskar_Visibilities* src, int channel_index, int time_index);

#ifdef __cplusplus
}
#endif
#endif /* OSKAR_VISIBILITIES_INSERT_H_ */
