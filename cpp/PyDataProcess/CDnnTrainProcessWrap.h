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

#ifndef CDNNTRAINPROCESSWRAP_H
#define CDNNTRAINPROCESSWRAP_H

#include "PyDataProcessGlobal.h"
#include "Core/CDnnTrainProcess.h"


class CDnnTrainProcessWrap : public CDnnTrainProcess, public wrapper<CDnnTrainProcess>
{
    public:

        CDnnTrainProcessWrap();
        CDnnTrainProcessWrap(const std::string& name, const std::shared_ptr<CDnnTrainProcessParam>& pParam);
        CDnnTrainProcessWrap(const CDnnTrainProcess& process);

        ~CDnnTrainProcessWrap() = default;

        virtual size_t  getProgressSteps() override;
        size_t          default_getProgressSteps();

        virtual size_t  getProgressSteps(size_t unitEltCount) override;
        size_t          default_getProgressSteps(size_t unitEltCount);

        virtual void    setActive(bool bActive) override;
        void            default_setActive(bool bActive);

        virtual void    beginTaskRun() override;
        void            default_beginTaskRun();

        virtual void    endTaskRun() override;
        void            default_endTaskRun();

        virtual void    run() override;
        void            default_run();

        virtual void    stop() override;
        void            default_stop();

        void            emitAddSubProgressSteps(int count);
        void            emitStepProgress();
        void            emitOutputChanged();
};

#endif // CDNNTRAINPROCESSWRAP_H
