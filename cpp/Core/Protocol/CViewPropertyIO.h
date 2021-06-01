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

#ifndef COUTPUTVIEWINFO_H
#define COUTPUTVIEWINFO_H

#include <QPointF>
#include <vector>
#include "Main/CoreGlobal.hpp"

class CORESHARED_EXPORT CViewPropertyIO
{
    public:

        enum class ViewMode { GUI_DRIVEN, INPUT_OUTPUT, INPUT_ONLY, OUTPUT_ONLY };

        CViewPropertyIO();

        void        setMaximized(bool bMaximized);
        void        setZoomFit(bool bFit);
        void        setZoomScale(float scale);
        void        setZoomPosition(int numScheduledScalings, const QPointF& targetPos, const QPointF& viewCenter);
        void        setViewMode(const ViewMode mode);

        float       getZoomScale() const;
        int         getZoomNumScheduledScalings() const;
        QPointF     getZoomTargetPos() const;
        QPointF     getViewCenter() const;
        ViewMode    getViewMode() const;

        bool        isChanged() const;
        bool        isMaximized() const;
        bool        isZoomFit() const;

        void        toggleMaximized();

    private:

        bool        m_bChanged = false;
        bool        m_bMaximized = false;
        ViewMode    m_viewMode = ViewMode::GUI_DRIVEN;

        //Zoom properties
        bool        m_bZoomFit = false;
        float       m_zoomScale = 1.0;
        int         m_zoomNumScheduledScalings = 0;
        QPointF     m_zoomTargetPos;
        QPointF     m_viewCenter;
};

using ViewPropertiesIO = std::vector<CViewPropertyIO>;

#endif // COUTPUTVIEWINFO_H
