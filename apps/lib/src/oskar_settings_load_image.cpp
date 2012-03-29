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

#include "apps/lib/oskar_settings_load_image.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <QtCore/QSettings>
#include <QtCore/QByteArray>
#include <QtCore/QVariant>

extern "C"
int oskar_settings_load_image(oskar_SettingsImage* im,
        const char* filename)
{
    QString temp;
    QByteArray t;
    QSettings s(QString(filename), QSettings::IniFormat);
    s.beginGroup("image");

    im->fov_deg = s.value("fov_deg", 2.0).toDouble();
    im->size = s.value("size", 256).toInt();

    im->channel_snapshots = s.value("channel_snapshots", false).toBool();
    im->channel_range[0] = s.value("channel_start", -1).toInt();
    im->channel_range[1] = s.value("channel_end", -1).toInt();

    im->time_snapshots = s.value("time_snapshots", false).toBool();
    im->time_range[0] = s.value("time_start", -1).toInt();
    im->time_range[1] = s.value("time_end", -1).toInt();

    temp = s.value("polarisation").toString().toUpper();
    if (temp.startsWith("STOKES") || temp.isEmpty())
        im->polarisation = OSKAR_IMAGE_TYPE_STOKES;
    else if (temp == "I")
        im->polarisation = OSKAR_IMAGE_TYPE_STOKES_I;
    else if (temp == "Q")
        im->polarisation = OSKAR_IMAGE_TYPE_STOKES_Q;
    else if (temp == "U")
        im->polarisation = OSKAR_IMAGE_TYPE_STOKES_U;
    else if (temp == "V")
        im->polarisation = OSKAR_IMAGE_TYPE_STOKES_V;
    else if (temp.startsWith("LINEAR"))
        im->polarisation = OSKAR_IMAGE_TYPE_POL_LINEAR;
    else if (temp == "XX")
        im->polarisation = OSKAR_IMAGE_TYPE_POL_XX;
    else if (temp == "XY")
        im->polarisation = OSKAR_IMAGE_TYPE_POL_XY;
    else if (temp == "YX")
        im->polarisation = OSKAR_IMAGE_TYPE_POL_YX;
    else if (temp == "YY")
        im->polarisation = OSKAR_IMAGE_TYPE_POL_YY;
    else
        return OSKAR_ERR_SETTINGS;

    temp = s.value("transform_type").toString().toUpper();
    if (temp == "DFT 2D" || temp.isEmpty())
        im->transform_type = OSKAR_IMAGE_DFT_2D;
    else if (temp == "DFT 3D")
        im->transform_type = OSKAR_IMAGE_DFT_3D;
    else if (temp == "FFT")
        im->transform_type = OSKAR_IMAGE_FFT;
    else
        return OSKAR_ERR_SETTINGS;

    t = s.value("input_vis_data").toByteArray();
    if (t.size() > 0)
    {
        im->input_vis_data = (char*)malloc(t.size() + 1);
        strcpy(im->input_vis_data, t.constData());
    }

    t = s.value("oskar_image").toByteArray();
    if (t.size() > 0)
    {
        im->oskar_image = (char*)malloc(t.size() + 1);
        strcpy(im->oskar_image, t.constData());
    }

    t = s.value("fits_image").toByteArray();
    if (t.size() > 0)
    {
        im->fits_image = (char*)malloc(t.size() + 1);
        strcpy(im->fits_image, t.constData());
    }

    return OSKAR_SUCCESS;
}
