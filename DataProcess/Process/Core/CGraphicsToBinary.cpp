#include "CGraphicsToBinary.h"

//----------------------------------//
//----- CGraphicsToBinaryParam -----//
//----------------------------------//
CGraphicsToBinaryParam::CGraphicsToBinaryParam() : CProtocolTaskParam()
{
}

void CGraphicsToBinaryParam::setParamMap(const UMapString &paramMap)
{
    m_bFitToContent = std::stoi(paramMap.at("fitToContent"));
    m_width = std::stoi(paramMap.at("width"));
    m_height = std::stoi(paramMap.at("height"));
}

UMapString CGraphicsToBinaryParam::getParamMap() const
{
    UMapString map;
    map.insert(std::make_pair("fitToContent", std::to_string(m_bFitToContent)));
    map.insert(std::make_pair("width", std::to_string(m_width)));
    map.insert(std::make_pair("height", std::to_string(m_height)));
    return map;
}

//-----------------------------//
//----- CGraphicsToBinary -----//
//-----------------------------//
CGraphicsToBinary::CGraphicsToBinary() : CImageProcess2d()
{
    removeInput(0);
}

CGraphicsToBinary::CGraphicsToBinary(const std::string name, const std::shared_ptr<CGraphicsToBinaryParam> &pParam) : CImageProcess2d(name)
{
    removeInput(0);
    m_pParam = std::make_shared<CGraphicsToBinaryParam>(*pParam);
}

size_t CGraphicsToBinary::getProgressSteps()
{
    return 2;
}

void CGraphicsToBinary::run()
{
    beginTaskRun();

    auto pInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(0));
    auto pParam = std::dynamic_pointer_cast<CGraphicsToBinaryParam>(m_pParam);

    if(pInput == nullptr || pParam == nullptr)
        throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

    if(pInput->isDataAvailable() == false)
        throw CException(CoreExCode::INVALID_PARAMETER, "Empty graphics layer", __func__, __FILE__, __LINE__);

    CMat binary;
    try
    {
        QRectF boundRect = pInput->getBoundingRect();
        if(pParam->m_bFitToContent)
        {
            std::vector<ProxyGraphicsItemPtr> itemsTmp;
            auto items = pInput->getItems();

            for(size_t i=0; i<items.size(); ++i)
            {
                auto itemTmp = items[i]->clone();
                itemTmp->translate(-boundRect.left(), -boundRect.top());
                itemsTmp.push_back(itemTmp);
            }
            CGraphicsConversion conv(boundRect.width(), boundRect.height());
            binary = conv.graphicsToBinaryMask(itemsTmp);
        }
        else
        {
            if(boundRect.right() > pParam->m_width || boundRect.bottom() > pParam->m_height)
                throw CException(CoreExCode::INVALID_DIMENSION, "Graphics out of bounds", __func__, __FILE__, __LINE__);

            CGraphicsConversion conv(pParam->m_width, pParam->m_height);
            binary = conv.graphicsToBinaryMask(pInput->getItems());
        }
    }
    catch(std::exception& e)
    {
        throw CException(CoreExCode::INVALID_PARAMETER, e.what(), __func__, __FILE__, __LINE__);
    }
    emit m_signalHandler->doProgress();

    auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
    if(pOutput)
        pOutput->setImage(binary);

    endTaskRun();
    emit m_signalHandler->doProgress();
}
