#ifndef CGRAPHICSITEMWRAP_H
#define CGRAPHICSITEMWRAP_H

#include "PyCoreGlobal.h"
#include "Graphics/CGraphicsItem.hpp"

class CGraphicsItemWrap : public CProxyGraphicsItem, public wrapper<CProxyGraphicsItem>
{
    public:

        CGraphicsItemWrap();
        CGraphicsItemWrap(GraphicsItem type);
        virtual ~CGraphicsItemWrap() = default;

        bool            isTextItem() const;
        bool            default_isTextItem() const;

        void            insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

        std::shared_ptr<CProxyGraphicsItem> clone() const override;
};

#endif // CGRAPHICSITEMWRAP_H
