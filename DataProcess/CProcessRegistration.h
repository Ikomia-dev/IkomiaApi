#ifndef CPROCESSREGISTRATION_H
#define CPROCESSREGISTRATION_H

#include "DataProcessGlobal.hpp"
#include "Core/CProcessFactory.hpp"
#include "Core/CWidgetFactory.hpp"

class DATAPROCESSSHARED_EXPORT CProcessRegistration
{
    public:

        CProcessRegistration();

        const CProcessAbstractFactory&  getProcessFactory() const;
        const CWidgetAbstractFactory&   getWidgetFactory() const;

        void                            registerProcess(const std::shared_ptr<CProcessFactory>& pProcessFactory,
                                                        const std::shared_ptr<CWidgetFactory> &pWidgetFactory);

        void                            unregisterProcess(const std::string& name);

        void                            reset();

    private:

        void                            registerCore();
        void                            registerOpenCV();
        void                            registerGmic();

        // Opencv
        void                            registerCvCore();
        void                            registerCvDnn();
        void                            registerCvFeatures2d();
        void                            registerCvImgproc();
        void                            registerCvPhoto();
        void                            registerCvTracking();
        void                            registerCvVideo();
        void                            registerCvBgsegm();
        void                            registerCvXimgproc();
        void                            registerCvXphoto();
        void                            registerCvOptflow();
        void                            registerCvBioinspired();
        void                            registerCvSaliency();
        void                            registerCvSuperres();
        void                            registerCvObjdetect();
        void                            registerCvText();

    private:

        CProcessAbstractFactory         m_processFactory;
        CWidgetAbstractFactory          m_widgetFactory;
};

#endif // CPROCESSREGISTRATION_H
