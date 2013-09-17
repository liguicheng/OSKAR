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

#include "imaging/oskar_evaluate_image_lm_grid.h"
#include "imaging/oskar_evaluate_image_lon_lat_grid.h"
#include "math/oskar_sph_from_lm.h"
#include <oskar_mem.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void oskar_evaluate_image_lon_lat_grid(oskar_Mem* lon, oskar_Mem* lat,
        int image_size_l, int image_size_m, double fov_rad_lon,
        double fov_rad_lat, double lon_rad, double lat_rad, int* status)
{
    int num_pixels, type, location;
    oskar_Mem lon_cpu, lat_cpu;

    /* Check all inputs. */
    if (!lon || !lat || !status)
    {
        oskar_set_invalid_argument(status);
        return;
    }

    /* Get meta-data. */
    type = oskar_mem_type(lon);
    location = oskar_mem_location(lon);
    num_pixels = image_size_l * image_size_m;

    /* Ensure enough space in output arrays. */
    if ((int)oskar_mem_length(lon) < num_pixels)
        oskar_mem_realloc(lon, num_pixels, status);
    if ((int)oskar_mem_length(lat) < num_pixels)
        oskar_mem_realloc(lat, num_pixels, status);

    /* Check if safe to proceed. */
    if (*status) return;

    /* Initialise temporary memory. */
    if (location != OSKAR_LOCATION_CPU)
    {
        oskar_mem_init(&lon_cpu, type, OSKAR_LOCATION_CPU, num_pixels, 1, status);
        oskar_mem_init(&lat_cpu, type, OSKAR_LOCATION_CPU, num_pixels, 1, status);
    }
    else
    {
        oskar_mem_get_pointer(&lon_cpu, lon, 0, num_pixels, status);
        oskar_mem_get_pointer(&lat_cpu, lat, 0, num_pixels, status);
    }

    /* Check if safe to proceed. */
    if (! *status)
    {
        /* Evaluate pixel grid and convert to longitude, latitude values. */
        if (type == OSKAR_SINGLE)
        {
            oskar_evaluate_image_lm_grid_f(image_size_l, image_size_m,
                    fov_rad_lon, fov_rad_lat, (float*)lon_cpu.data,
                    (float*)lat_cpu.data);
            oskar_sph_from_lm_f(num_pixels, lon_rad, lat_rad,
                    (const float*)lon_cpu.data, (const float*)lat_cpu.data,
                    (float*)lon_cpu.data, (float*)lat_cpu.data);
        }
        else if (type == OSKAR_DOUBLE)
        {
            oskar_evaluate_image_lm_grid_d(image_size_l, image_size_m,
                    fov_rad_lon, fov_rad_lat, (double*)lon_cpu.data,
                    (double*)lat_cpu.data);
            oskar_sph_from_lm_d(num_pixels, lon_rad, lat_rad,
                    (const double*)lon_cpu.data, (const double*)lat_cpu.data,
                    (double*)lon_cpu.data, (double*)lat_cpu.data);
        }
    }

    /* Copy data to GPU and free temporary arrays, if required. */
    if (location != OSKAR_LOCATION_CPU)
    {
        oskar_mem_copy(lon, &lon_cpu, status);
        oskar_mem_copy(lat, &lat_cpu, status);
        oskar_mem_free(&lon_cpu, status);
        oskar_mem_free(&lat_cpu, status);
    }
}

#ifdef __cplusplus
}
#endif
