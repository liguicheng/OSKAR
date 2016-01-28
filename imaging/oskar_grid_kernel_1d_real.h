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

#ifndef OSKAR_GRID_KERNEL_1D_REAL_H_
#define OSKAR_GRID_KERNEL_1D_REAL_H_

/**
 * @file oskar_grid_kernel_1d_real.h
 */

#include <oskar_global.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Simple gridding function for 1D real convolution kernel (double precision).
 *
 * @details
 * Simple gridding function for 1D real convolution kernel.
 *
 * @param[in] support       GCF support size (typ. 3; width = 2 * support + 1).
 * @param[in] oversample    GCF oversample factor, or values per grid cell.
 * @param[in] conv_func     GCF array, length oversample * (support + 1).
 * @param[in] num_vis       Number of visibility points.
 * @param[in] uu            Baseline UU coordinates, in wavelengths.
 * @param[in] vv            Baseline VV coordinates, in wavelengths.
 * @param[in] vis           Complex visibilities for each baseline.
 * @param[in] cell_size_rad Cell size, in radians.
 * @param[in] image_size    Side length of image and grid.
 * @param[out] num_skipped  Number of visibilities that fell outside the grid.
 * @param[in,out] norm      Updated grid normalisation factor.
 * @param[in,out] grid      Updated complex visibility grid.
 */
OSKAR_EXPORT
void oskar_grid_kernel_1d_real_d(const int support, const int oversample,
        const double* restrict conv_func, const int num_vis,
        const double* restrict uu, const double* restrict vv,
        const double* restrict vis, const double cell_size_rad,
        const int image_size, int* restrict num_skipped,
        double* restrict norm, double* restrict grid);

/**
 * @brief
 * Simple gridding function for 1D real convolution kernel (single precision).
 *
 * @details
 * Simple gridding function for 1D real convolution kernel.
 *
 * @param[in] support       GCF support size (typ. 3; width = 2 * support + 1).
 * @param[in] oversample    GCF oversample factor, or values per grid cell.
 * @param[in] conv_func     GCF array, length oversample * (support + 1).
 * @param[in] num_vis       Number of visibility points.
 * @param[in] uu            Baseline UU coordinates, in wavelengths.
 * @param[in] vv            Baseline VV coordinates, in wavelengths.
 * @param[in] vis           Complex visibilities for each baseline.
 * @param[in] cell_size_rad Cell size, in radians.
 * @param[in] image_size    Side length of image and grid.
 * @param[out] num_skipped  Number of visibilities that fell outside the grid.
 * @param[in,out] norm      Updated grid normalisation factor.
 * @param[in,out] grid      Updated complex visibility grid.
 */
OSKAR_EXPORT
void oskar_grid_kernel_1d_real_f(const int support, const int oversample,
        const double* restrict conv_func, const int num_vis,
        const float* restrict uu, const float* restrict vv,
        const float* restrict vis, const double cell_size_rad,
        const int image_size, int* restrict num_skipped,
        double* restrict norm, float* restrict grid);

#ifdef __cplusplus
}
#endif

#endif /* OSKAR_GRID_KERNEL_1D_REAL_H_ */
