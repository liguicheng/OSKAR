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

#include "oskar_global.h"
#include "interferometry/oskar_Visibilities.h"
#include "utility/oskar_binary_file_read.h"
#include "utility/oskar_BinaryTag.h"
#include "utility/oskar_mem_element_size.h"
#include "utility/oskar_mem_binary_file_read.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
#endif
oskar_Visibilities* oskar_visibilities_read(const char* filename, int* status)
{
    // Visibility metadata.
    int num_channels = 0, num_times = 0, num_baselines = 0, amp_type = 0;
    int err = 0;
    oskar_BinaryTagIndex* index = NULL;
    oskar_Visibilities* vis     = NULL;
    char grp[] = "VISIBILITY";

    // Sanity check on inputs.
    if (filename == NULL)
    {
        if (status) *status = OSKAR_ERR_INVALID_ARGUMENT;
        return NULL;
    }

    // Read visibility dimensions.
    err = oskar_binary_file_read_int(filename, &index, grp,
            "NUM_CHANNELS", 0, &num_channels);
    if (err)
    {
        if (status) *status = err;
        return NULL;
    }
    err = oskar_binary_file_read_int(filename, &index, grp,
            "NUM_TIMES", 0, &num_times);
    if (err)
    {
        if (status) *status = err;
        return NULL;
    }
    err = oskar_binary_file_read_int(filename, &index, grp,
            "NUM_BASELINES", 0, &num_baselines);
    if (err)
    {
        if (status) *status = err;
        return NULL;
    }
    err = oskar_binary_file_read_int(filename, &index, grp,
            "AMP_TYPE", 0, &amp_type);
    if (err)
    {
        if (status) *status = err;
        return NULL;
    }

    // Create the visibility structure.
    vis = new oskar_Visibilities(amp_type, OSKAR_LOCATION_CPU,
            num_channels, num_times, num_baselines);

    // Read visibility metadata.
    err = oskar_binary_file_read_double(filename, &index, grp,
            "FREQ_START_HZ", 0, &vis->freq_start_hz);
    if (err)
    {
        if (status) *status = err;
        delete vis;
        return NULL;
    }
    err = oskar_binary_file_read_double(filename, &index, grp,
            "FREQ_INC_HZ", 0, &vis->freq_inc_hz);
    if (err)
    {
        if (status) *status = err;
        delete vis;
        return NULL;
    }
    err = oskar_binary_file_read_double(filename, &index, grp,
            "TIME_START_MJD_UTC", 0, &vis->time_start_mjd_utc);
    if (err)
    {
        if (status) *status = err;
        delete vis;
        return NULL;
    }
    err = oskar_binary_file_read_double(filename, &index, grp,
            "TIME_INC_SEC", 0, &vis->time_inc_seconds);
    if (err)
    {
        if (status) *status = err;
        delete vis;
        return NULL;
    }

    // Read the baseline coordinate arrays.
    err = oskar_mem_binary_file_read(&vis->uu_metres, filename, &index, grp,
            "BASELINE_UU", 0);
    if (err)
    {
        if (status) *status = err;
        delete vis;
        return NULL;
    }
    err = oskar_mem_binary_file_read(&vis->vv_metres, filename, &index, grp,
            "BASELINE_VV", 0);
    if (err)
    {
        if (status) *status = err;
        delete vis;
        return NULL;
    }
    err = oskar_mem_binary_file_read(&vis->ww_metres, filename, &index, grp,
            "BASELINE_WW", 0);
    if (err)
    {
        if (status) *status = err;
        delete vis;
        return NULL;
    }

    // Read the visibility data.
    err = oskar_mem_binary_file_read(&vis->amplitude, filename, &index, grp,
            "AMPLITUDE", 0);
    if (err)
    {
        if (status) *status = err;
        delete vis;
        return NULL;
    }

    if (status) *status = OSKAR_SUCCESS;

    return vis;
}
