// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "CBlobMeasure.h"
#include "Main/CoreTools.hpp"
#include "IO/CGraphicsProcessOutput.h"
#include "Graphics/CGraphicsLayer.h"
#include "Graphics/CGraphicsConversion.h"

//-----------------------------//
//----- CBlobMeasureParam -----//
//-----------------------------//
CBlobMeasureParam::CBlobMeasureParam() : CProtocolTaskParam()
{
}

void CBlobMeasureParam::setParamMap(const UMapString &paramMap)
{
    m_measures.clear();
    for(auto it=paramMap.begin(); it!=paramMap.end(); ++it)
    {
        CMeasure measure;
        measure.m_name = it->first;
        std::vector<std::string> tokens;
        Utils::String::tokenize(it->second, tokens, ",");

        if(tokens.size() > 0)
            measure.m_id = static_cast<CMeasure::Id>(std::stoi(tokens[0]));

        if(tokens.size() > 1)
            measure.m_formula = tokens[1];

        m_measures.push_back(measure);
    }
}

UMapString CBlobMeasureParam::getParamMap() const
{
    UMapString map;

    for(size_t i=0; i<m_measures.size(); ++i)
    {
        std::string paramValue = std::to_string(static_cast<size_t>(m_measures[i].m_id)) + "," + m_measures[i].m_formula;
        map.insert(std::make_pair(m_measures[i].m_name, paramValue));
    }
    return map;
}

//------------------------//
//----- CBlobMeasure -----//
//------------------------//
CBlobMeasure::CBlobMeasure() : CImageProcess2d()
{
    insertInput(std::make_shared<CImageProcessIO>(), 1);
    addOutput(std::make_shared<CImageProcessIO>());
    addOutput(std::make_shared<CMeasureProcessIO>());
    addOutput(std::make_shared<CGraphicsProcessOutput>());
}

CBlobMeasure::CBlobMeasure(const std::string name, const std::shared_ptr<CBlobMeasureParam> &pParam) : CImageProcess2d(name)
{
    m_pParam = std::make_shared<CBlobMeasureParam>(*pParam);
    insertInput(std::make_shared<CImageProcessIO>(), 1);
    addOutput(std::make_shared<CImageProcessIO>());
    addOutput(std::make_shared<CMeasureProcessIO>());
    addOutput(std::make_shared<CGraphicsProcessOutput>());
}

size_t CBlobMeasure::getProgressSteps()
{
    return 5;
}

void CBlobMeasure::run()
{
    beginTaskRun();

    if(getInputCount() == 0)
        throw CException(CoreExCode::INVALID_PARAMETER, "No input", __func__, __FILE__, __LINE__);

    auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
    auto pParam = std::dynamic_pointer_cast<CBlobMeasureParam>(m_pParam);

    if(pInput == nullptr || pParam == nullptr)
        throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

    if(pInput->isDataAvailable() == false)
        throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

    if(getOutputCount() < 3)
        throw CException(CoreExCode::INVALID_SIZE, "Wrong outputs count", __func__, __FILE__, __LINE__);

    emit m_signalHandler->doProgress();

    try
    {
        CMat img;
        CMat imgSrc = pInput->getImage();

        //Source image must be 1 single channel
        if(imgSrc.channels() > 1)
            cv::cvtColor(imgSrc, img, cv::COLOR_BGR2GRAY);
        else
            img = imgSrc;

        //Get measures output to fill it
        auto pOutputMeasure = std::dynamic_pointer_cast<CMeasureProcessIO>(getOutput(2));
        if(pOutputMeasure == nullptr)
            throw CException(CoreExCode::INVALID_SIZE, "Invalid measures output", __func__, __FILE__, __LINE__);

        pOutputMeasure->clearData();

        //Check graphics output and create layer
        auto pGraphicsOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(3));
        if(pGraphicsOutput == nullptr)
            throw CException(CoreExCode::INVALID_SIZE, "Invalid graphics output", __func__, __FILE__, __LINE__);

        pGraphicsOutput->setNewLayer(getName());
        emit m_signalHandler->doProgress();

        if( pInput->getDataType() == IODataType::IMAGE_LABEL ||
            pInput->getDataType() == IODataType::VIDEO_LABEL ||
            pInput->getDataType() == IODataType::LIVE_STREAM_LABEL)
        {
            computeFromLabel(img);
        }
        else
            compute(img);

        emit m_signalHandler->doProgress();
        forwardInputImage(0, 0);
        forwardInputImage(1, 1);
        emit m_signalHandler->doProgress();
    }
    catch(std::exception& e)
    {
        throw CException(CoreExCode::INVALID_PARAMETER, e.what(), __func__, __FILE__, __LINE__);
    }
    endTaskRun();
    emit m_signalHandler->doProgress();

}

void CBlobMeasure::updateStaticOutputs()
{
    bool bInputVideo = false;
    bool bInputStream = false;
    for(size_t i=0; i<getInputCount(); ++i)
    {
        assert(getInput(i));

        if( getInputDataType(i) == IODataType::VIDEO_BINARY ||
            getInputDataType(i) == IODataType::VIDEO_LABEL)
        {
            // Find input video data type
            bInputVideo = true;
            break;
        }
        else if(getInputDataType(i) == IODataType::LIVE_STREAM_BINARY ||
                getInputDataType(i) == IODataType::LIVE_STREAM_LABEL)
        {
            // Find input video data type
            bInputStream = true;
            break;
        }
    }

    // If input data type is a video or image sequence
    if(bInputVideo == true)
    {
        for(size_t i=0; i<getOutputCount(); ++i)
        {
            auto dataType = getOutputDataType(i);
            if(dataType == IODataType::IMAGE)
                setOutput(std::make_shared<CVideoProcessIO>(), i);
            else if(dataType == IODataType::IMAGE_BINARY)
                setOutput(std::make_shared<CVideoProcessIO>(IODataType::VIDEO_BINARY), i);
            else if(dataType == IODataType::IMAGE_LABEL)
                setOutput(std::make_shared<CVideoProcessIO>(IODataType::VIDEO_LABEL), i);
        }
    }
    else if(bInputStream == true)
    {
        for(size_t i=0; i<getOutputCount(); ++i)
        {
            auto dataType = getOutputDataType(i);
            if(dataType == IODataType::IMAGE)
                setOutput(std::make_shared<CVideoProcessIO>(IODataType::LIVE_STREAM), i);
            else if(dataType == IODataType::IMAGE_BINARY)
                setOutput(std::make_shared<CVideoProcessIO>(IODataType::LIVE_STREAM_BINARY), i);
            else if(dataType == IODataType::IMAGE_LABEL)
                setOutput(std::make_shared<CVideoProcessIO>(IODataType::LIVE_STREAM_LABEL), i);
        }
    }
    else
    {
        // No input video
        // No input stream
        // Restore correct IMAGE datatype
        for(size_t i=0; i<getOutputCount(); ++i)
        {
            if(getOutputDataType(i) == IODataType::VIDEO || getOutputDataType(i) == IODataType::LIVE_STREAM)
                setOutput(std::make_shared<CImageProcessIO>(), i);
            else if(getOutputDataType(i) == IODataType::VIDEO_BINARY || getOutputDataType(i) == IODataType::LIVE_STREAM_BINARY)
                setOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_BINARY), i);
            else if(getOutputDataType(i) == IODataType::VIDEO_LABEL || getOutputDataType(i) == IODataType::LIVE_STREAM_LABEL)
                setOutput(std::make_shared<CImageProcessIO>(IODataType::IMAGE_LABEL), i);
        }
    }
}

void CBlobMeasure::compute(CMat imgSrc)
{
    CMat tmp;
    if(imgSrc.depth() != CV_8U)
    {
        //Convert to 32 bits integer
        imgSrc.convertTo(tmp, CV_32SC1);
        assert(tmp.type() == CV_32SC1);
    }
    else
        tmp = imgSrc;

    //Find blobs
    m_blobs.clear();
    m_hierarchy.clear();
    cv::findContours(tmp, m_blobs, m_hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_TC89_L1);

    //Create associated graphics
    int lineSize = 1;
    CColor penColor = {255,0,0,255};
    CColor brushColor = {255,0,0,20};

    if(m_graphicsContextPtr)
    {
        auto pProperty = m_graphicsContextPtr->getPolygonPropertyPtr();
        penColor = pProperty->m_penColor;
        brushColor = pProperty->m_brushColor;
        lineSize = pProperty->m_lineSize;
    }
    computeGraphics(penColor, brushColor, lineSize);

    //Calculate associated measures
    std::string label = "";
    if(m_graphicsContextPtr)
    {
        auto pProperty = m_graphicsContextPtr->getPolygonPropertyPtr();
        label = pProperty->m_category;
    }
    computeMeasures(label);
}

void CBlobMeasure::computeFromLabel(CMat imgSrc)
{
    double minVal, maxVal;
    cv::minMaxLoc(imgSrc, &minVal, &maxVal);

    //Get graphics property : polygon
    std::string label = "";
    int lineSize = 1, labelIndex = 1;

    if(m_graphicsContextPtr)
    {
        auto pProperty = m_graphicsContextPtr->getPolygonPropertyPtr();
        lineSize = pProperty->m_lineSize;
        label = pProperty->m_category;
    }

    //For each label
    for(size_t i=1; i<=maxVal; ++i)
    {
        //Extract current label image as binary
        CMat imgTmp;
        imgTmp = (imgSrc == i);

        //Find blobs
        cv::findContours(imgTmp, m_blobs, m_hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_TC89_L1);
        if(m_blobs.size() > 0)
        {
            std::string currentLabel = label + std::to_string(labelIndex);
            labelIndex++;

            //Create associated graphics
            CColor penColor = Utils::Graphics::getRandomCColor();
            CColor brushColor = penColor;
            computeGraphics(penColor, brushColor, lineSize);

            //Calculate associated measures
            computeMeasures(currentLabel);

            m_blobs.clear();
            m_hierarchy.clear();
        }
    }
}

void CBlobMeasure::computeGraphics(const CColor& penColor, const CColor& brushColor, const int lineSize)
{
    auto pOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(3));
    assert(pOutput);
    CGraphicsConversion convert;
    auto graphics = convert.blobsToProxyGraphics(m_blobs, m_hierarchy, penColor, brushColor, lineSize);

    for(size_t i=0; i<graphics.size(); ++i)
        pOutput->addItem(graphics[i]);

    auto pInputImgOverlay = std::dynamic_pointer_cast<CImageProcessIO>(getInput(1));
    assert(pInputImgOverlay);

    if(pInputImgOverlay->isDataAvailable())
        pOutput->setImageIndex(1);
}

void CBlobMeasure::computeMeasures(const std::string& label)
{
    //Get graphics items list
    auto pGraphicsOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(3));
    assert(pGraphicsOutput);
    auto graphicsItems = pGraphicsOutput->getItems();

    //Get measures output to fill it
    auto pOutput = std::dynamic_pointer_cast<CMeasureProcessIO>(getOutput(2));
    assert(pOutput);

    if(m_blobs.size() > 0)
    {
        int holeId;
        size_t blobIndex = 0;
        size_t graphicsId = 0;

        for(int id=0; id >=0; id=m_hierarchy[id][0])
        {
            if(m_blobs[id].size() > 1)
            {
                if(blobIndex >= (size_t)graphicsItems.size())
                    throw CException(CoreExCode::INDEX_OVERFLOW, "Graphics index overflow", __func__, __FILE__, __LINE__);

                //Get graphics id
                graphicsId = graphicsItems[blobIndex]->getId();

                std::vector<std::vector<cv::Point>> holes;
                if((holeId = m_hierarchy[id][2]) != -1)
                {
                    // Polygon with holes -> retrieve potential holes
                    for(; holeId >=0; holeId=m_hierarchy[holeId][0])
                        holes.push_back(m_blobs[holeId]);
                }

                //Compute measures for current blob
                auto measures = computeBlobMeasures(m_blobs[id], holes, graphicsId, label);
                pOutput->addObjectMeasures(measures);
                blobIndex++;
            }
        }
    }
}

ObjectMeasures CBlobMeasure::computeBlobMeasures(const std::vector<cv::Point> &blob, const std::vector<std::vector<cv::Point>> &holes, size_t graphicsId, const std::string& label)
{
    std::vector<CObjectMeasure> measures;
    auto moments = cv::moments(blob, false);
    auto surface = cv::contourArea(blob);
    auto perimeter = cv::arcLength(blob, true);

    //Get measures to calculate
    auto pParam = std::dynamic_pointer_cast<CBlobMeasureParam>(m_pParam);
    for(size_t i=0; i<pParam->m_measures.size(); ++i)
    {
        CMeasure measure(pParam->m_measures[i].m_id);
        switch(pParam->m_measures[i].m_id)
        {
            case CMeasure::Id::SURFACE:
            {
                //Calculate holes surface
                double holesSurface = 0;
                for(size_t j=0; j<holes.size(); ++j)
                    holesSurface += cv::contourArea(holes[j]);

                measures.push_back(CObjectMeasure(measure, surface - holesSurface, graphicsId, label)); break;
            }

            case CMeasure::Id::PERIMETER:
                measures.push_back(CObjectMeasure(measure, perimeter, graphicsId, label)); break;

            case CMeasure::Id::CENTROID:
            {
                if(moments.m00 > 0)
                {
                    auto cx = moments.m10 / moments.m00;
                    auto cy = moments.m01 / moments.m00;
                    measures.push_back(CObjectMeasure(measure, {cx, cy}, graphicsId, label));
                }
                else
                {
                    //m00 is null -> average points to estimate centroid
                    double sumX=0, sumY=0;
                    size_t nbPts = blob.size();

                    for(size_t j=0; j<nbPts; ++j)
                    {
                        sumX += blob[j].x;
                        sumY += blob[j].y;
                    }
                    measures.push_back(CObjectMeasure(measure, {sumX/nbPts, sumY/nbPts}, graphicsId, label));
                }
                break;
            }

            case CMeasure::Id::BBOX:
            {
                auto bbox = cv::boundingRect(blob);
                measures.push_back(CObjectMeasure(measure, {(double)bbox.x, (double)bbox.y, (double)bbox.width, (double)bbox.height}, graphicsId, label));
                break;
            }

            case CMeasure::Id::ORIENTED_BBOX:
            {
                auto rotatedBbox = cv::minAreaRect(blob);
                measures.push_back(CObjectMeasure(measure,
                                                  {rotatedBbox.center.x, rotatedBbox.center.y, rotatedBbox.size.width, rotatedBbox.size.height, rotatedBbox.angle},
                                                  graphicsId, label));
                break;
            }

            case CMeasure::Id::EQUIVALENT_DIAMETER:
            {
                auto diameter = sqrt(4 * surface / _pi);
                measures.push_back(CObjectMeasure(measure, diameter, graphicsId, label));
                break;
            }

            case CMeasure::Id::ELONGATION:
            {
                if(moments.m20 == 0 && moments.m02 == 0)
                    measures.push_back(CObjectMeasure(measure, -1.0, graphicsId, label));
                else
                {
                    //R. Mukundan and K.R. Ramakrishnan. Moment Functions in Image Analysis –Theory and Applications. World Scientific, 1998.
                    auto elongation = sqrt((moments.m20 - moments.m02)*(moments.m20 - moments.m02) + (4*moments.m11*moments.m11)) / (moments.m20 + moments.m02);
                    measures.push_back(CObjectMeasure(measure, elongation, graphicsId, label));
                }
                break;
            }

            case CMeasure::Id::CIRCULARITY:
            {
                if(perimeter == 0)
                    measures.push_back(CObjectMeasure(measure, -1.0, graphicsId, label));
                else
                {
                    auto circularity = (4 * _pi * surface) / (perimeter*perimeter);
                    measures.push_back(CObjectMeasure(measure, circularity, graphicsId, label));
                }
                break;
            }

            case CMeasure::Id::SOLIDITY:
            {
                try
                {
                    std::vector<cv::Point> hull;
                    cv::convexHull(blob, hull);
                    auto hullSurface = cv::contourArea(hull);

                    if(hullSurface == 0)
                        measures.push_back(CObjectMeasure(measure, -1.0, graphicsId, label));
                    else
                    {
                        auto solidity = surface / hullSurface;
                        measures.push_back(CObjectMeasure(measure, solidity, graphicsId, label));
                    }
                }
                catch(std::exception& e)
                {
                    qDebug() << QString::fromStdString(e.what());
                }
                break;
            }

            case CMeasure::Id::CUSTOM:
                computeCustomMeasure(measures, pParam->m_measures[i].m_formula, graphicsId); break;
        }
    }
    return measures;
}

void CBlobMeasure::computeCustomMeasure(ObjectMeasures &blobMeasures, const std::string &formula, size_t graphicsId)
{
    Q_UNUSED(blobMeasures);
    Q_UNUSED(formula);
    Q_UNUSED(graphicsId);
}


