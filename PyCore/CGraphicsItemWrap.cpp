#include "CGraphicsItemWrap.h"

CGraphicsItemWrap::CGraphicsItemWrap() : CProxyGraphicsItem()
{
}

CGraphicsItemWrap::CGraphicsItemWrap(GraphicsItem type) : CProxyGraphicsItem(type)
{
}

bool CGraphicsItemWrap::isTextItem() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override isTextItemOver = this->get_override("isTextItem"))
            return isTextItemOver();
        else
            return CProxyGraphicsItem::isTextItem();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CGraphicsItemWrap::default_isTextItem() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CProxyGraphicsItem::isTextItem();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CGraphicsItemWrap::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    Q_UNUSED(image);
    Q_UNUSED(filler);
    Q_UNUSED(bForceFill);
    Q_UNUSED(bBinary);
}

std::shared_ptr<CProxyGraphicsItem> CGraphicsItemWrap::clone() const
{
    return nullptr;
}
