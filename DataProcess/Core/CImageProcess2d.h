#ifndef CIMAGEPROCESS2D_H
#define CIMAGEPROCESS2D_H

#include "DataProcessGlobal.hpp"
#include "opencv2/opencv.hpp"
#include "Protocol/CProtocolTask.h"
#include "Data/CMat.hpp"
#include "Core/CProcessFactory.hpp"
#include "IO/CImageProcessIO.h"
#include "IO/CGraphicsProcessInput.h"
#include "IO/CGraphicsProcessOutput.h"
#include "IO/CMeasureProcessIO.h"
#include "IO/CFeatureProcessIO.hpp"

/**
 * @ingroup groupDataProcess
 * @brief The CImageProcess2d class implements all basic features used in a process task dedicated to 2D image processing.
 * @details This class defines a process task with the following properties:
 *  - Two inputs: one image (CImageProcessIO) and one graphics layer (CGraphicsProcessInput).
 *  - One output: one image (CImageProcessIO).
 * It can be a good starting point to use it as a base class for all process task dedicated to 2D images.
 */
class DATAPROCESSSHARED_EXPORT CImageProcess2d : public CProtocolTask
{
    public:

        enum MaskMode { MERGE_SOURCE, MASK_ONLY };

        /**
        * @brief Default constructor.
        * - Inputs: CImageProcessIO + CGraphicsProcessInput.
        * - Outputs: CImageProcessIO.
        */
        CImageProcess2d();
        /**
         * @brief Constructs CImageProcess2d with or without graphics input.
         * @param bEnableGraphics: True, the task manage graphics input. False, the task does not have any graphics input.
         */
        CImageProcess2d(bool bEnableGraphics);
        /**
         * @brief Constructs CImageProcess2d task with the given name. Default inputs and outputs.
         * @param name: name of the process task. Must be unique.
         */
        CImageProcess2d(const std::string& name);
        /**
         * @brief Constructs CImageProcess2d task with the given name, with or without graphics input.
         * @param name: name of the process task. Must be unique.
         * @param bEnableGraphics: True, the task manage graphics input. False, the task does not have any graphics input.
         */
        CImageProcess2d(const std::string& name, bool bEnableGraphics);

        virtual ~CImageProcess2d();

        /**
         * @brief see CProtocolTask::setActive for more details.
         * @param bActive
         */
        virtual void    setActive(bool bActive) override;
        /**
         * @brief Binds a display color map to an image output. The color mask is generated from a given output mask (binary or labelled image).
         * @param index: zero-based index of the output that will be displayed with the color map. The output must be a CImageProcessIO or derived.
         * @param maskIndex: zero-based index of the output representing the mask used to generate the color overlay.
         * @param colors: list of colors for the color map. If empty, the system generates random colors.
         */
        void            setOutputColorMap(size_t index, size_t maskIndex, const std::vector<cv::Vec3b>& colors={});

        /**
         * @brief Determines output data type automatically from input data types.
         * Don't forget to call this method in overriden methods.
         * See CProtocolTask::updateStaticOutputs.
         */
        virtual void    updateStaticOutputs() override;

        /**
         * @brief See CProtocolTask::beginTaskRun.
         */
        virtual void    beginTaskRun() override;
        /**
         * @brief See CProtocolTask::endTaskRun.
         */
        virtual void    endTaskRun() override;

        /**
         * @brief See CProtocolTask::graphicsChanged.
         */
        virtual void    graphicsChanged() override;
        /**
         * @brief See CProtocolTask::globalInputChanged.
         */
        virtual void    globalInputChanged(bool bNewSequence) override;

        /**
         * @brief Generates a mask image (binary) from the given graphics input.
         * The new mask is appended to the internal mask list.
         * @param width: width of the mask (should be the width of the source image).
         * @param height: height of the mask (should be the height of the source image).
         * @param pGraphicsInput: graphics items becomes white blobs in the black background mask.
         */
        void            createGraphicsMask(size_t width, size_t height, const GraphicsProcessInputPtr& pGraphicsInput);

        /**
         * @brief Applies the mask generated from graphics input to the result image so that only masked areas seems to be processed.
         * @param src: source image of the process.
         * @param dst: result image.
         * @param maskIndex: zero-based index of the mask.
         */
        void            applyGraphicsMask(const CMat &src, CMat &dst, size_t maskIndex);
        /**
         * @brief Applies the mask generated from graphics input to the binary source image. Only white areas on both image and mask are kept in the result image.
         * @param src: source image, binary only.
         * @param dst: masked result image (binary).
         * @param maskIndex: zero-based index of the mask.
         */
        void            applyGraphicsMaskToBinary(const CMat &src, CMat &dst, size_t maskIndex);

        void            applyInputGraphicsMask(int graphicsIndex, int inputImgIndex, int outputImgIndex, MaskMode mode);

        /**
         * @brief Gets the binary mask generated from graphics input at position index.
         * @param index: zero-based index.
         * @return CMat in C++, Numpy array in Python.
         */
        CMat            getGraphicsMask(size_t index) const;

        /**
         * @brief Checks if a binary mask from graphics input is available at position index.
         * @param index: zero-based index.
         * @return True if a mask is available, False otherwise.
         */
        bool            isMaskAvailable(size_t index) const;

        /**
         * @brief Forwards input image at position inputIndex to output at position outputIndex.
         * @param inputIndex: zero-based index of the input.
         * @param outputIndex: zero-based index of the output.
         */
        void            forwardInputImage(int inputIndex=0, int outputIndex=0);

    private:

        void            copyColorMapToOutput();

        CMat            createInputGraphicsMask(int index, int width, int height);
        void            createOverlayMasks();

    /** @cond INTERNAL */
    protected:

        bool                m_bEnableGraphics = true;
        std::vector<CMat>   m_graphicsMasks;
        std::vector<CMat>   m_colorMaps;
        std::vector<size_t> m_colorMapMaskIndices;
    /** @endcond */
};

#endif // CIMAGEPROCESS2D_H
