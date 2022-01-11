#include "CObjDetectFilter.h"
#include "IO/CGraphicsInput.h"
#include "IO/CGraphicsOutput.h"
#include "IO/CBlobMeasureIO.h"
#include "UtilsTools.hpp"

//---------------------------------//
//----- CObjDetectFilterParam -----//
//---------------------------------//
CObjDetectFilterParam::CObjDetectFilterParam()
{
}

void CObjDetectFilterParam::setParamMap(const UMapString &paramMap)
{
    m_confidence = std::stof(paramMap.at("confidence"));
    m_categories = paramMap.at("categories");
}

UMapString CObjDetectFilterParam::getParamMap() const
{
    UMapString params;
    params.insert(std::make_pair("confidence", std::to_string(m_confidence)));
    params.insert(std::make_pair("categories", m_categories));
    return params;
}

//----------------------------//
//----- CObjDetectFilter -----//
//----------------------------//
CObjDetectFilter::CObjDetectFilter() : CWorkflowTask()
{
    addInput(std::make_shared<CGraphicsInput>());
    addInput(std::make_shared<CBlobMeasureIO>());
    addOutput(std::make_shared<CGraphicsOutput>());
    addOutput(std::make_shared<CBlobMeasureIO>());
}

CObjDetectFilter::CObjDetectFilter(const std::string name, const std::shared_ptr<CObjDetectFilterParam> &pParam)
    : CWorkflowTask(name)
{
    m_pParam = std::make_shared<CObjDetectFilterParam>(*pParam);
    addInput(std::make_shared<CGraphicsInput>());
    addInput(std::make_shared<CBlobMeasureIO>());
    addOutput(std::make_shared<CGraphicsOutput>());
    addOutput(std::make_shared<CBlobMeasureIO>());
}

size_t CObjDetectFilter::getProgressSteps()
{
    return 2;
}

void CObjDetectFilter::run()
{
    auto graphicsInPtr = std::dynamic_pointer_cast<CGraphicsInput>(getInput(0));
    auto measuresInPtr = std::dynamic_pointer_cast<CBlobMeasureIO>(getInput(1));
    auto paramPtr = std::dynamic_pointer_cast<CObjDetectFilterParam>(m_pParam);

    if(graphicsInPtr == nullptr || measuresInPtr == nullptr || paramPtr == nullptr)
        throw CException(CoreExCode::INVALID_PARAMETER, "Invalid inputs", __func__, __FILE__, __LINE__);

    auto graphicsOutPtr = std::dynamic_pointer_cast<CGraphicsOutput>(getOutput(0));
    auto measuresOutPtr = std::dynamic_pointer_cast<CBlobMeasureIO>(getOutput(1));
    graphicsOutPtr->clearData();
    measuresOutPtr->clearData();

    std::set<int> graphicsIds;
    std::set<std::string> categories;

    if(paramPtr->m_categories != "all")
    {
        std::vector<std::string> categs;
        Utils::String::tokenize(paramPtr->m_categories, categs, ",");
        categories.insert(categs.begin(), categs.end());
    }

    ObjectsMeasures measures = measuresInPtr->getMeasures();
    for(size_t i=0; i<measures.size(); ++i)
    {
        int confIndex = measuresInPtr->getBlobMeasureIndex(i, "Confidence");
        if(confIndex != -1)
        {
            double confValue = measures[i][confIndex].m_values[0];
            std::string category = measures[i][confIndex].m_label;

            if(confValue >= paramPtr->m_confidence && (categories.empty() || categories.find(category) != categories.end()))
            {
                graphicsIds.insert(measures[i][confIndex].m_graphicsId);
                measuresOutPtr->addObjectMeasures(measures[i]);
            }
        }
    }
    emit m_signalHandler->doProgress();

    auto items = graphicsInPtr->getItems();
    for(size_t i=0; i<items.size(); ++i)
    {
        if(graphicsIds.find(items[i]->getId()) != graphicsIds.end())
            graphicsOutPtr->addItem(items[i]);
    }
    emit m_signalHandler->doProgress();
}

//-----------------------------------//
//----- CObjDetectFilterFactory -----//
//-----------------------------------//
CObjDetectFilterFactory::CObjDetectFilterFactory()
{
    m_info.m_name = "ik_obj_detection_filter";
    m_info.m_description = QObject::tr("This process extract connected components from binary image and calculate selected measures for each.").toStdString();
    m_info.m_path = QObject::tr("Core/Utils").toStdString();
    m_info.m_iconPath = QObject::tr(":/Images/default-process.png").toStdString();
    m_info.m_keywords = "object,detection,filtering,measures,graphics";
}

WorkflowTaskPtr CObjDetectFilterFactory::create(const WorkflowTaskParamPtr &pParam)
{
    auto pDerivedParam = std::dynamic_pointer_cast<CObjDetectFilterParam>(pParam);
    if(pDerivedParam != nullptr)
        return std::make_shared<CObjDetectFilter>(m_info.m_name, pDerivedParam);
    else
        return create();
}

WorkflowTaskPtr CObjDetectFilterFactory::create()
{
    auto pDerivedParam = std::make_shared<CObjDetectFilterParam>();
    assert(pDerivedParam != nullptr);
    return std::make_shared<CObjDetectFilter>(m_info.m_name, pDerivedParam);
}

//----------------------------------//
//----- CWidgetObjDetectFilter -----//
//----------------------------------//
CWidgetObjDetectFilter::CWidgetObjDetectFilter(QWidget *parent): CWorkflowTaskWidget(parent)
{
    init();
}

CWidgetObjDetectFilter::CWidgetObjDetectFilter(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent)
    : CWorkflowTaskWidget(parent)
{
    m_pParam = std::dynamic_pointer_cast<CObjDetectFilterParam>(pParam);
    init();
}

void CWidgetObjDetectFilter::init()
{
    if(m_pParam == nullptr)
        m_pParam = std::make_shared<CObjDetectFilterParam>();

    m_pSpinConfidence = addDoubleSpin("Confidence", m_pParam->m_confidence, 0.0, 1.0, 0.1, 2);
    m_pEditCategories = addEdit("Categories", QString::fromStdString(m_pParam->m_categories));
}

void CWidgetObjDetectFilter::onApply()
{
    m_pParam->m_confidence = m_pSpinConfidence->value();
    m_pParam->m_categories = m_pEditCategories->text().toStdString();
    emit doApplyProcess(m_pParam);
}

//-----------------------------------------//
//----- CWidgetObjDetectFilterFactory -----//
//-----------------------------------------//
CWidgetObjDetectFilterFactory::CWidgetObjDetectFilterFactory()
{
    m_name = "ik_obj_detection_filter";
}

WorkflowTaskWidgetPtr CWidgetObjDetectFilterFactory::create(std::shared_ptr<CWorkflowTaskParam> pParam)
{
    return std::make_shared<CWidgetObjDetectFilter>(pParam);
}
