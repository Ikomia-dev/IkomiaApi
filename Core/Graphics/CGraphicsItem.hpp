#ifndef CGRAPHICSITEMINTERFACE_HPP
#define CGRAPHICSITEMINTERFACE_HPP

#include <QCursor>
#include "CGraphicsJSON.h"
#include "DesignPattern/CAbstractFactory.hpp"
#include "Data/CMat.hpp"
#include "Graphics/CGraphicsConversion.h"
#include "Graphics/CGraphicsContext.h"


/** @cond INTERNAL */

//-----------------------------------
//- Class CProxyGraphicsItem
//- Proxy class without Qt dependance
//- Useful for Python bindings
//-----------------------------------
class CProxyGraphicsItem
{
    public:

        CProxyGraphicsItem()
        {
            m_id = getNextId();
        }
        CProxyGraphicsItem(GraphicsItem type)
        {
            m_id = getNextId();
            m_type = type;
        }

        virtual ~CProxyGraphicsItem() = default;

        void                    setCategory(const std::string& category)
        {
            m_category = category;
        }

        size_t                  getId() const
        {
            return m_id;
        }
        size_t                  getType() const
        {
            return static_cast<size_t>(m_type);
        }
        std::string             getCategory() const
        {
            return m_category;
        }
        virtual QRectF          getBoundingRect() const
        {
            return QRectF();
        }

        virtual bool            isTextItem() const
        {
            return false;
        }
        bool                    isUsedGlobalContext() const
        {
            return m_bUseGlobalContext;
        }

        virtual void            insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const = 0;

        virtual void            translate(float dx, float dy)
        {
            Q_UNUSED(dx);
            Q_UNUSED(dy);
        }

        virtual std::shared_ptr<CProxyGraphicsItem> clone() const = 0;

        virtual void            toJson(QJsonObject& obj) const
        {
            obj["id"] = (qint64)m_id;
            obj["type"] = (int)m_type;
            obj["category"] = QString::fromStdString(m_category);
        }

    private:

        static size_t           getNextId()
        {
            // Use static function scope variable to correctly define lifespan of object.
            static size_t nextId = 0;
            return nextId++;
        }

    private:

        size_t          m_id = 0;
        GraphicsItem    m_type = GraphicsItem::LAYER;
        std::string     m_category = "Default";

    protected:

        bool            m_bUseGlobalContext = true;
};

using ProxyGraphicsItemPtr = std::shared_ptr<CProxyGraphicsItem>;

//------------------------------------------------
//- Class CGraphicsItem
//- Base class of all Qt-based graphics item
//- Enable to make Qt-based graphics item copyable
//------------------------------------------------
class CGraphicsItem
{
    public:

        CGraphicsItem()
        {
            m_id = getNextId();
        }
        CGraphicsItem(GraphicsItem type)
        {
            m_id = getNextId();
            m_type = type;
        }
        CGraphicsItem(size_t id, GraphicsItem type)
        {
            m_id = id;
            m_type = type;
        }

        virtual ~CGraphicsItem() = default;

        virtual void                    setPenColor(QColor color) = 0;
        virtual void                    setBrushColor(QColor color) = 0;
        void                            setCategory(const QString& category){ m_category = category; }
        void                            setCategory(const std::string& category){ m_category = QString::fromStdString(category); }

        size_t                          getId() const { return m_id; }
        GraphicsItem                    getItemType() const { return m_type; }
        QString                         getCategory() const { return m_category; }
        virtual QByteArray              getJsonData(CGraphicsJSON& jsonMgr) const = 0;

        virtual ProxyGraphicsItemPtr    createProxyGraphicsItem() const = 0;

        virtual void                    buildFromJsonData(CGraphicsJSON& jsonMgr, QByteArray data) = 0;

        virtual void                    insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const = 0;

        virtual QGraphicsItem*          clone() const = 0;

    private:

        static size_t                   getNextId()
        {
            // Use static function scope variable to correctly define lifespan of object.
            static size_t nextId = 0;
            return nextId++;
        }

    private:

        size_t          m_id = 0;
        GraphicsItem    m_type = GraphicsItem::LAYER;
        QString         m_category = "Default";
};

//-----------------------------------------------
//- Class CGraphicsFactory
//- Factory to instanciate Qt-based graphics item
//-----------------------------------------------
class CGraphicsFactory
{
    public:

        CGraphicsFactory(){}
        virtual ~CGraphicsFactory(){}

        size_t                  getType() const
        {
            return m_type;
        }

        virtual QGraphicsItem*  create(QGraphicsItem* pParent) = 0;

    protected:

        size_t m_type = static_cast<size_t>(GraphicsItem::POLYGON);
};

using GraphicsFactories = std::vector<std::shared_ptr<CGraphicsFactory>>;

class CGraphicsAbstractFactory: public CAbstractFactory<size_t, QGraphicsItem*, QGraphicsItem*>
{
    public:

        GraphicsFactories& getList()
        {
            return m_factories;
        }

    private:

        GraphicsFactories m_factories;
};

/** @endcond */

#endif // CGRAPHICSITEMINTERFACE_HPP
