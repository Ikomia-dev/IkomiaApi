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

#ifndef CCONVERTIO_H
#define CCONVERTIO_H

#include "Workflow/CWorkflowTaskIO.h"
#include "IO/CVideoIO.h"

class CConvertIO
{
    public:

        CConvertIO();

        static ImageIOPtr    convertToImageIO(const WorkflowTaskIOPtr& ioFrom);
        static VideoIOPtr    convertToVideoIO(const WorkflowTaskIOPtr& ioFrom);
        static VideoIOPtr    convertToStreamIO(const WorkflowTaskIOPtr& ioFrom);
        static ImageIOPtr    convertToVolumeIO(const WorkflowTaskIOPtr& ioFrom);

    private:

        static IODataType   getTargetImageType(IODataType srcType);
        static IODataType   getTargetVideoType(IODataType srcType);
        static IODataType   getTargetStreamType(IODataType srcType);
        static IODataType   getTargetVolumeType(IODataType srcType);
};

#endif // CCONVERTIO_H
