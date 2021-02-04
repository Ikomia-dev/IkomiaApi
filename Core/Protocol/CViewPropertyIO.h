#ifndef COUTPUTVIEWINFO_H
#define COUTPUTVIEWINFO_H

#include <QPointF>
#include <vector>
#include "Main/CoreGlobal.hpp"

class CORESHARED_EXPORT CViewPropertyIO
{
    public:

        CViewPropertyIO();

        void    setMaximized(bool bMaximized);
        void    setZoomFit(bool bFit);
        void    setZoomScale(float scale);
        void    setZoomPosition(int numScheduledScalings, const QPointF& targetPos, const QPointF& viewCenter);

        float   getZoomScale() const;
        int     getZoomNumScheduledScalings() const;
        QPointF getZoomTargetPos() const;
        QPointF getViewCenter() const;

        bool    isChanged() const;
        bool    isMaximized() const;
        bool    isZoomFit() const;

        void    toggleMaximized();

    private:

        bool    m_bChanged = false;
        bool    m_bMaximized = false;

        //Zoom properties
        bool    m_bZoomFit = false;
        float   m_zoomScale = 1.0;
        int     m_zoomNumScheduledScalings = 0;
        QPointF m_zoomTargetPos;
        QPointF m_viewCenter;
};

using ViewPropertiesIO = std::vector<CViewPropertyIO>;

#endif // COUTPUTVIEWINFO_H
