/*
 * Copyright (c) 2013, The University of Oxford
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

#ifndef OSKAR_MEM_RANDOM_FILL_H_
#define OSKAR_MEM_RANDOM_FILL_H_

/**
 * @file oskar_mem_random_fill.h
 */

#include <oskar_global.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Fills a block of memory with randomly generated floating-point numbers.
 *
 * @details
 * This function fills a block of memory with randomly generated floating-point
 * numbers. An error is returned if the base type of the memory block is not
 * OSKAR_SINGLE or OSKAR_DOUBLE.
 *
 * Numbers are generated using the C rand() function. The random number
 * generator can be reset prior to calling this function by a call to srand().
 *
 * This function is used primarily to generate data for unit tests.
 *
 * @param[in] mem        Pointer to memory block to fill.
 * @param[in] lo         The minimum value of the random number.
 * @param[in] hi         The maximum value of the random number.
 * @param[in,out] status Status return code.
 */
OSKAR_EXPORT
void oskar_mem_random_fill(oskar_Mem* mem, double lo, double hi, int* status);

#ifdef __cplusplus
}
#endif

#endif /* OSKAR_MEM_RANDOM_FILL_H_ */
