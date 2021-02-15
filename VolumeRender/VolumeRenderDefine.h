/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef VOLUMERENDERDEFINE_H
#define VOLUMERENDERDEFINE_H

//----------------------------------------------------------------------------------//
//- We place enum here instead of scopped enum in CVolumeRender because of         -//
//- multiple definitions problem if we include VolumeRender.h in several cpp unit. -//
//- This is due to CL/cl2.hpp and MinGW but we don't know the root cause...        -//
//----------------------------------------------------------------------------------//
enum class RenderError : short
{
    SUCCESS,
    INIT_FAILED
};

enum class RenderParamType : size_t
{
    DENSITY,
    BRIGHTNESS,
    OFFSET,
    SCALE,
    CHR_MIN,
    CHR_MAX,
    CHG_MIN,
    CHG_MAX,
    CHB_MIN,
    CHB_MAX,
    ISOVALUE_R,
    ISOVALUE_G,
    ISOVALUE_B,
    ISOVALUE_A
};

enum class RenderMode : size_t
{
    RAYCAST,
    MIP,
    MinIP,
    ISOSURFACE
};

enum class RenderColormap : size_t
{
    CLASSIC,
    SKIN,
    JET,
    GRAYCOLOR
};

#endif // VOLUMERENDERDEFINE_H
