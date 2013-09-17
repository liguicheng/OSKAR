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

#include <oskar_blank_below_horizon.h>
#include <oskar_blank_below_horizon_cuda.h>
#include <oskar_cuda_check_error.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Single precision. */
void oskar_blank_below_horizon_matrix_f(float4c* jones, int num_sources,
        const float* mask)
{
    int i;

    for (i = 0; i < num_sources; ++i)
    {
        if (mask[i] < 0.0f)
        {
            jones[i].a.x = 0.0f;
            jones[i].a.y = 0.0f;
            jones[i].b.x = 0.0f;
            jones[i].b.y = 0.0f;
            jones[i].c.x = 0.0f;
            jones[i].c.y = 0.0f;
            jones[i].d.x = 0.0f;
            jones[i].d.y = 0.0f;
        }
    }
}

void oskar_blank_below_horizon_scalar_f(float2* jones, int num_sources,
        const float* mask)
{
    int i;

    for (i = 0; i < num_sources; ++i)
    {
        if (mask[i] < 0.0f)
        {
            jones[i].x = 0.0f;
            jones[i].y = 0.0f;
        }
    }
}

/* Double precision. */
void oskar_blank_below_horizon_matrix_d(double4c* jones, int num_sources,
        const double* mask)
{
    int i;

    for (i = 0; i < num_sources; ++i)
    {
        if (mask[i] < 0.0)
        {
            jones[i].a.x = 0.0;
            jones[i].a.y = 0.0;
            jones[i].b.x = 0.0;
            jones[i].b.y = 0.0;
            jones[i].c.x = 0.0;
            jones[i].c.y = 0.0;
            jones[i].d.x = 0.0;
            jones[i].d.y = 0.0;
        }
    }
}

void oskar_blank_below_horizon_scalar_d(double2* jones, int num_sources,
        const double* mask)
{
    int i;

    for (i = 0; i < num_sources; ++i)
    {
        if (mask[i] < 0.0)
        {
            jones[i].x = 0.0;
            jones[i].y = 0.0;
        }
    }
}


/* Wrapper. */
void oskar_blank_below_horizon(oskar_Mem* data, const oskar_Mem* mask,
        int num_sources, int* status)
{
    int type, location;

    /* Check all inputs. */
    if (!mask || !data || !status)
    {
        oskar_set_invalid_argument(status);
        return;
    }

    /* Check if safe to proceed. */
    if (*status) return;

    /* Check that all arrays are co-located. */
    location = oskar_mem_location(data);
    if (oskar_mem_location(mask) != location)
        *status = OSKAR_ERR_LOCATION_MISMATCH;

    /* Check that the mask type is OK. */
    if (oskar_mem_type(mask) != OSKAR_SINGLE && oskar_mem_type(mask) != OSKAR_DOUBLE)
        *status = OSKAR_ERR_BAD_DATA_TYPE;

    /* Check that the dimensions are OK. */
    if ((int)oskar_mem_length(data) < num_sources)
        *status = OSKAR_ERR_DIMENSION_MISMATCH;

    /* Check if safe to proceed. */
    if (*status) return;

    /* Zero the value of any positions below the horizon. */
    type = oskar_mem_type(data);
    if (location == OSKAR_LOCATION_GPU)
    {
#ifdef OSKAR_HAVE_CUDA
        if (type == OSKAR_SINGLE_COMPLEX_MATRIX)
        {
            oskar_blank_below_horizon_matrix_cuda_f
            ((float4c*)data->data, num_sources, (const float*)mask->data);
        }
        else if (type == OSKAR_SINGLE_COMPLEX)
        {
            oskar_blank_below_horizon_scalar_cuda_f
            ((float2*)data->data, num_sources, (const float*)mask->data);
        }
        else if (type == OSKAR_DOUBLE_COMPLEX_MATRIX)
        {
            oskar_blank_below_horizon_matrix_cuda_d
            ((double4c*)data->data, num_sources, (const double*)mask->data);
        }
        else if (type == OSKAR_DOUBLE_COMPLEX)
        {
            oskar_blank_below_horizon_scalar_cuda_d
            ((double2*)data->data, num_sources, (const double*)mask->data);
        }
        else
            *status = OSKAR_ERR_BAD_DATA_TYPE;

        /* Report any CUDA error. */
        oskar_cuda_check_error(status);
#else
        *status = OSKAR_ERR_CUDA_NOT_AVAILABLE;
#endif
    }
    else
    {
        if (type == OSKAR_SINGLE_COMPLEX_MATRIX)
        {
            oskar_blank_below_horizon_matrix_f
            ((float4c*)data->data, num_sources, (const float*)mask->data);
        }
        else if (type == OSKAR_SINGLE_COMPLEX)
        {
            oskar_blank_below_horizon_scalar_f
            ((float2*)data->data, num_sources, (const float*)mask->data);
        }
        else if (type == OSKAR_DOUBLE_COMPLEX_MATRIX)
        {
            oskar_blank_below_horizon_matrix_d
            ((double4c*)data->data, num_sources, (const double*)mask->data);
        }
        else if (type == OSKAR_DOUBLE_COMPLEX)
        {
            oskar_blank_below_horizon_scalar_d
            ((double2*)data->data, num_sources, (const double*)mask->data);
        }
        else
            *status = OSKAR_ERR_BAD_DATA_TYPE;
    }
}

#ifdef __cplusplus
}
#endif
