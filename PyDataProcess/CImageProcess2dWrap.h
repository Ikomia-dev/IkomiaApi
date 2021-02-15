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

#ifndef CIMAGEPROCESS2DWRAP_H
#define CIMAGEPROCESS2DWRAP_H

#include "PyDataProcessGlobal.h"
#include "Core/CImageProcess2d.h"

//----------------------------------------------//
//-----------  CImageProcess2dWrap  ------------//
//- Wrapping class to handle virtual functions -//
//----------------------------------------------//
class CImageProcess2dWrap : public CImageProcess2d, public wrapper<CImageProcess2d>
{
    public:

        CImageProcess2dWrap();
        CImageProcess2dWrap(bool bGraphicsInput);
        CImageProcess2dWrap(const std::string& name);
        CImageProcess2dWrap(const std::string& name, bool bGraphicsInput);
        CImageProcess2dWrap(const CImageProcess2d& process);

        virtual size_t  getProgressSteps() override;
        size_t          default_getProgressSteps();

        virtual size_t  getProgressSteps(size_t unitEltCount) override;
        size_t          default_getProgressSteps(size_t unitEltCount);

        virtual void    setActive(bool bActive) override;
        void            default_setActive(bool bActive);

        void            setOutputColorMap(size_t index, size_t maskIndex, const std::vector<std::vector<uchar>>& colors={});

        virtual void    updateStaticOutputs() override;
        void            default_updateStaticOutputs();

        virtual void    beginTaskRun() override;
        void            default_beginTaskRun();

        virtual void    endTaskRun() override;
        void            default_endTaskRun();

        virtual void    run() override;
        void            default_run();

        virtual void    stop() override;
        void            default_stop();

        virtual void    graphicsChanged() override;
        void            default_graphicsChanged();

        virtual void    globalInputChanged(bool bNewSequence) override;
        void            default_globalInputChanged(bool bNewSequence);

        void            emitStepProgress();
        void            emitGraphicsContextChanged();
        void            emitOutputChanged();
};

#endif // CIMAGEPROCESS2DWRAP_H


