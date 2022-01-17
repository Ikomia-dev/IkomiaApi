#ifndef COBJDETECTFILTER_H
#define COBJDETECTFILTER_H

#include "Workflow/CWorkflowTask.h"
#include "Core/CTaskFactory.hpp"
#include "Workflow/CWorkflowTaskWidget.h"
#include "Core/CWidgetFactory.hpp"

//---------------------------------//
//----- CObjDetectFilterParam -----//
//---------------------------------//
class CObjDetectFilterParam: public CWorkflowTaskParam
{
    public:

        CObjDetectFilterParam();
        ~CObjDetectFilterParam() = default;

        void        setParamMap(const UMapString& paramMap) override;

        UMapString  getParamMap() const override;

    public:

        float       m_confidence = 0.5;
        std::string m_categories = "all";
};

//----------------------------//
//----- CObjDetectFilter -----//
//----------------------------//
class CObjDetectFilter: public CWorkflowTask
{
    public:

        CObjDetectFilter();
        CObjDetectFilter(const std::string name, const std::shared_ptr<CObjDetectFilterParam>& pParam);

        size_t  getProgressSteps() override;

        void    run() override;
};

//-----------------------------------//
//----- CObjDetectFilterFactory -----//
//-----------------------------------//
class CObjDetectFilterFactory : public CTaskFactory
{
    public:

        CObjDetectFilterFactory();
        ~CObjDetectFilterFactory() = default;

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override;
        virtual WorkflowTaskPtr create() override;
};

//----------------------------------//
//----- CWidgetObjDetectFilter -----//
//----------------------------------//
class CWidgetObjDetectFilter : public CWorkflowTaskWidget
{
    public:

        CWidgetObjDetectFilter(QWidget *parent = Q_NULLPTR);
        CWidgetObjDetectFilter(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR);

    protected:

        void    init();

        void    onApply() override;

    private:

        std::shared_ptr<CObjDetectFilterParam>  m_pParam = nullptr;
        QDoubleSpinBox*                         m_pSpinConfidence = nullptr;
        QLineEdit*                              m_pEditCategories = nullptr;
};

//-----------------------------------------//
//----- CWidgetObjDetectFilterFactory -----//
//-----------------------------------------//
class CWidgetObjDetectFilterFactory : public CWidgetFactory
{
    public:

        CWidgetObjDetectFilterFactory();
        ~CWidgetObjDetectFilterFactory() = default;

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam);
};

#endif // COBJDETECTFILTER_H
