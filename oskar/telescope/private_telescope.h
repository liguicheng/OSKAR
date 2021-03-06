/*
 * Copyright (c) 2011-2020, The University of Oxford
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

#ifndef OSKAR_PRIVATE_TELESCOPE_H_
#define OSKAR_PRIVATE_TELESCOPE_H_

#include <telescope/station/oskar_station.h>

struct oskar_Telescope
{
    /* Private structure meta-data. */
    int precision;
    int mem_location;

    /* Fundamental properties that don't depend on stations. */
    int pol_mode;                /* Polarisation mode (full or scalar). */
    double lon_rad;              /* Geodetic longitude of telescope, in radians. */
    double lat_rad;              /* Geodetic latitude of telescope, in radians. */
    double alt_metres;           /* Altitude of telescope above ellipsoid, in metres. */
    double pm_x_rad;             /* Polar motion (x-component) in radians. */
    double pm_y_rad;             /* Polar motion (y-component) in radians. */
    int phase_centre_coord_type; /* Phase centre coordinate type. */
    double phase_centre_ra_rad;  /* Right Ascension of phase centre, in radians. */
    double phase_centre_dec_rad; /* Declination of phase centre, in radians. */
    double channel_bandwidth_hz; /* Channel bandwidth, in Hz. */
    double time_average_sec;     /* Time average smearing duration, in sec. */
    double uv_filter_min;        /* Minimum allowed UV distance. */
    double uv_filter_max;        /* Maximum allowed UV distance. */
    int uv_filter_units;         /* Unit of allowed UV distance (OSKAR_METRES or OSKAR_WAVELENGTHS). */
    int noise_enabled;           /* Flag set if thermal noise is enabled. */
    unsigned int noise_seed;     /* Random generator seed. */

    /* Ionosphere parameters. */
    int ionosphere_screen_type;
    oskar_Mem* tec_screen_path;
    double tec_screen_height_km;
    double tec_screen_pixel_size_m;
    double tec_screen_time_interval_sec;

    /* Station data. */
    int supplied_coord_type;                           /* Type of coordinates specified in telescope model. */
    int num_stations;                                  /* Number of stations in the model. */
    oskar_Station** station;                           /* Array of station structure handles. */
    oskar_Mem* station_true_offset_ecef_metres[3];     /* True station coordinates, in metres (offset ECEF). */
    oskar_Mem* station_true_enu_metres[3];             /* True station coordinate, in metres (horizon). */
    oskar_Mem* station_measured_offset_ecef_metres[3]; /* Measured station coordinates, in metres (offset ECEF). */
    oskar_Mem* station_measured_enu_metres[3];         /* Measured station coordinate, in metres (horizon). */
    int max_station_size;                              /* Maximum station size (number of elements) */
    int max_station_depth;                             /* Maximum station depth. */
    int identical_stations;                            /* True if all stations are identical. */
    int allow_station_beam_duplication;                /* True if station beam duplication is allowed. */
    int enable_numerical_patterns;                     /* True if numerical element patterns are enabled. */
};

#ifndef OSKAR_TELESCOPE_TYPEDEF_
#define OSKAR_TELESCOPE_TYPEDEF_
typedef struct oskar_Telescope oskar_Telescope;
#endif /* OSKAR_TELESCOPE_TYPEDEF_ */

#endif /* OSKAR_PRIVATE_TELESCOPE_H_ */
