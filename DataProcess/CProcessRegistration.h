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

#ifndef CPROCESSREGISTRATION_H
#define CPROCESSREGISTRATION_H

#include "DataProcessGlobal.hpp"
#include "Core/CProcessFactory.hpp"
#include "Core/CWidgetFactory.hpp"

class DATAPROCESSSHARED_EXPORT CProcessRegistration
{
    public:

        CProcessRegistration();

        const CProcessAbstractFactory&  getProcessFactory() const;
        const CWidgetAbstractFactory&   getWidgetFactory() const;

        void                            registerProcess(const std::shared_ptr<CProcessFactory>& pProcessFactory,
                                                        const std::shared_ptr<CWidgetFactory> &pWidgetFactory);

        void                            unregisterProcess(const std::string& name);

        void                            reset();

    private:

        void                            registerCore();
        void                            registerOpenCV();
        void                            registerGmic();

        // Opencv
        void                            registerCvCore();
        void                            registerCvDnn();
        void                            registerCvFeatures2d();
        void                            registerCvImgproc();
        void                            registerCvPhoto();
        void                            registerCvTracking();
        void                            registerCvVideo();
        void                            registerCvBgsegm();
        void                            registerCvXimgproc();
        void                            registerCvXphoto();
        void                            registerCvOptflow();
        void                            registerCvBioinspired();
        void                            registerCvSaliency();
        void                            registerCvSuperres();
        void                            registerCvObjdetect();
        void                            registerCvText();

    private:

        CProcessAbstractFactory         m_processFactory;
        CWidgetAbstractFactory          m_widgetFactory;
};

#endif // CPROCESSREGISTRATION_H
