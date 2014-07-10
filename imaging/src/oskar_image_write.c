/*
 * Copyright (c) 2012-2014, The University of Oxford
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

#include <private_image.h>
#include <oskar_image.h>
#include <oskar_binary.h>
#include <oskar_file_exists.h>
#include <oskar_log.h>
#include <math.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void oskar_image_write(const oskar_Image* image, oskar_Log* log,
        const char* filename, int idx, int* status)
{
    int num, type;
    unsigned char grp = OSKAR_TAG_GROUP_IMAGE;
    oskar_Binary* h = 0;
    char* log_data = 0;
    size_t log_size = 0;
    const char* settings_path_str;
    const oskar_Mem* settings_path;

    /* Check all inputs. */
    if (!image || !filename || !status)
    {
        oskar_set_invalid_argument(status);
        return;
    }

    /* Check if safe to proceed. */
    if (*status) return;

    /* Get the meta-data. */
    type = oskar_mem_type(image->data);

    /* Check dimensions. */
    num = image->num_channels * image->num_times * image->num_pols *
            image->width * image->height;
    if (num != (int)oskar_mem_length(image->data))
    {
        *status = OSKAR_ERR_DIMENSION_MISMATCH;
        return;
    }

    /* Create the handle. */
    h = oskar_binary_create(filename, 'w', status);
    if (*status)
    {
        oskar_binary_free(h);
        return;
    }

    /* Write the common meta-data. */
    oskar_binary_write_metadata(h, status);

    /* If settings path is set, write out the data. */
    settings_path = image->settings_path;
    settings_path_str = oskar_mem_char_const(settings_path);
    if (settings_path_str && strlen(settings_path_str) > 0)
    {
        /* Write the settings path. */
        oskar_binary_write_mem(h, settings_path, OSKAR_TAG_GROUP_SETTINGS,
                OSKAR_TAG_SETTINGS_PATH, idx, 0, status);

        /* Check the file exists */
        if (oskar_file_exists(settings_path_str))
        {
            /* Write the settings file. */
            oskar_Mem* temp;
            temp = oskar_mem_read_binary_raw(settings_path_str, OSKAR_CHAR,
                    OSKAR_CPU, status);
            oskar_binary_write_mem(h, temp, OSKAR_TAG_GROUP_SETTINGS,
                    OSKAR_TAG_SETTINGS, idx, 0, status);
            oskar_mem_free(temp, status);
        }
    }

    /* If log exists, then write it out. */
    log_data = oskar_log_file_data(log, &log_size);
    if (log_data)
    {
        oskar_binary_write(h, OSKAR_CHAR, OSKAR_TAG_GROUP_RUN,
                OSKAR_TAG_RUN_LOG, idx, log_size, log_data, status);
        free(log_data);
    }

    /* Write dimensions. */
    oskar_binary_write_int(h, grp,
            OSKAR_IMAGE_TAG_NUM_PIXELS_WIDTH, idx, image->width, status);
    oskar_binary_write_int(h, grp,
            OSKAR_IMAGE_TAG_NUM_PIXELS_HEIGHT, idx, image->height, status);
    oskar_binary_write_int(h, grp,
            OSKAR_IMAGE_TAG_NUM_POLS, idx, image->num_pols, status);
    oskar_binary_write_int(h, grp,
            OSKAR_IMAGE_TAG_NUM_TIMES, idx, image->num_times, status);
    oskar_binary_write_int(h, grp,
            OSKAR_IMAGE_TAG_NUM_CHANNELS, idx, image->num_channels, status);

    /* Write the dimension order. */
    oskar_binary_write(h, OSKAR_INT, grp,
            OSKAR_IMAGE_TAG_DIMENSION_ORDER, idx,
            sizeof(image->dimension_order), image->dimension_order, status);

    /* Write other image meta-data. */
    oskar_binary_write_int(h, grp,
            OSKAR_IMAGE_TAG_IMAGE_TYPE, idx, image->image_type, status);
    oskar_binary_write_int(h, grp,
            OSKAR_IMAGE_TAG_DATA_TYPE, idx, type, status);
    oskar_binary_write_double(h, grp,
            OSKAR_IMAGE_TAG_TIME_START_MJD_UTC, idx, image->time_start_mjd_utc,
            status);
    oskar_binary_write_double(h, grp,
            OSKAR_IMAGE_TAG_TIME_INC_SEC, idx, image->time_inc_sec, status);
    oskar_binary_write_double(h, grp,
            OSKAR_IMAGE_TAG_FREQ_START_HZ, idx, image->freq_start_hz, status);
    oskar_binary_write_double(h, grp,
            OSKAR_IMAGE_TAG_FREQ_INC_HZ, idx, image->freq_inc_hz, status);
    oskar_binary_write_int(h, grp, OSKAR_IMAGE_TAG_GRID_TYPE, idx,
            image->grid_type, status);
    oskar_binary_write_int(h, grp, OSKAR_IMAGE_TAG_COORD_FRAME, idx,
            image->coord_frame, status);
    if (image->grid_type == OSKAR_IMAGE_GRID_TYPE_RECTILINEAR)
    {
        oskar_binary_write_double(h, grp, OSKAR_IMAGE_TAG_CENTRE_LONGITUDE,
                idx, image->centre_lon_deg, status);
        oskar_binary_write_double(h, grp, OSKAR_IMAGE_TAG_CENTRE_LATITUDE,
                idx, image->centre_lat_deg, status);
        oskar_binary_write_double(h, grp, OSKAR_IMAGE_TAG_FOV_LONGITUDE,
                idx, image->fov_lon_deg, status);
        oskar_binary_write_double(h, grp, OSKAR_IMAGE_TAG_FOV_LATITUDE,
                idx, image->fov_lat_deg, status);
    }
    else if (image->grid_type == OSKAR_IMAGE_GRID_TYPE_HEALPIX)
    {
        oskar_binary_write_int(h, grp, OSKAR_IMAGE_TAG_HEALPIX_NSIDE,
                idx, image->healpix_nside, status);
    }

    /* Write the image data. */
    oskar_binary_write_mem(h, image->data, grp, OSKAR_IMAGE_TAG_IMAGE_DATA,
            idx, 0, status);

    /* Release the handle. */
    oskar_binary_free(h);
}

#ifdef __cplusplus
}
#endif
