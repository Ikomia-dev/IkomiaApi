#ifndef CGRAPHICSPROCESSINPUT_H
#define CGRAPHICSPROCESSINPUT_H

#include "DataProcessGlobal.hpp"
#include "Protocol/CProtocolTaskIO.h"
#include "Graphics/CGraphicsItem.hpp"

/** @cond INTERNAL */

class CGraphicsLayer;
class QGraphicsItem;
class CGraphicsProcessOutput;

class DATAPROCESSSHARED_EXPORT CGraphicsProcessInput : public CProtocolTaskIO
{
    public:

        enum GraphicsSource { GRAPHICS_LAYER, GRAPHICS_OUTPUT };

        using GraphicsProcessInputPtr = std::shared_ptr<CGraphicsProcessInput>;

        CGraphicsProcessInput();
        CGraphicsProcessInput(CGraphicsLayer* pLayer);
        CGraphicsProcessInput(const CGraphicsProcessInput& in);
        CGraphicsProcessInput(CGraphicsProcessInput&& in);
        CGraphicsProcessInput(const CGraphicsProcessOutput &out);

        virtual ~CGraphicsProcessInput();

        CGraphicsProcessInput&      operator=(const CGraphicsProcessInput& in);
        CGraphicsProcessInput&      operator=(CGraphicsProcessInput&& in);
        CGraphicsProcessInput&      operator=(const CGraphicsProcessOutput& out);

        void                        setLayer(CGraphicsLayer* pLayer);
        void                        setItems(const std::vector<ProxyGraphicsItemPtr>& items);

        const CGraphicsLayer*               getLayer() const;
        std::vector<ProxyGraphicsItemPtr>   getItems() const;
        QRectF                              getBoundingRect() const;

        bool                        isDataAvailable() const override;

        void                        clearData() override;

        void                        copy(const std::shared_ptr<CProtocolTaskIO> &ioPtr) override;

        GraphicsProcessInputPtr     clone() const;

    private:

        virtual ProtocolTaskIOPtr   cloneImp() const override;

    private:

        //Must be seen as weak pointer
        const CGraphicsLayer*               m_pLayer = nullptr;
        std::vector<ProxyGraphicsItemPtr>   m_items;
        GraphicsSource                      m_source = GraphicsSource::GRAPHICS_LAYER;
};

using GraphicsProcessInputPtr = std::shared_ptr<CGraphicsProcessInput>;

class DATAPROCESSSHARED_EXPORT CGraphicsProcessInputFactory: public CProtocolTaskIOFactory
{
    public:

        CGraphicsProcessInputFactory()
        {
            m_name = "CGraphicsProcessInput";
        }

        virtual ProtocolTaskIOPtr   create(IODataType dataType)
        {
            Q_UNUSED(dataType);
            return std::make_shared<CGraphicsProcessInput>();
        }
};

/** @endcond */

#endif // CGRAPHICSPROCESSINPUT_H
