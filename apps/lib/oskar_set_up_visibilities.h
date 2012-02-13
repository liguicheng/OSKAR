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

#ifndef OSKAR_SET_UP_VISIBILITIES_H_
#define OSKAR_SET_UP_VISIBILITIES_H_

/**
 * @file oskar_set_up_visibilities.h
 */

#include "oskar_global.h"
#include "apps/lib/oskar_Settings.h"
#include "interferometry/oskar_TelescopeModel.h"
#include "interferometry/oskar_Visibilities.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Creates a visibility structure on the CPU for the whole simulation.
 *
 * @details
 * This function creates and returns an empty visibility structure in CPU
 * memory for the simulation to fill.
 *
 * @param[in] settings A reference to the settings object.
 * @param[in] tel_cpu  A pointer to the telescope model.
 * @param[in] type The type of the visibility data (must be a complex type).
 */
OSKAR_EXPORT
oskar_Visibilities* oskar_set_up_visibilities(const oskar_SettingsNew* settings,
        const oskar_TelescopeModel* tel_cpu, int type);

#ifdef __cplusplus
}
#endif

#endif // OSKAR_SET_UP_VISIBILITIES_H_
