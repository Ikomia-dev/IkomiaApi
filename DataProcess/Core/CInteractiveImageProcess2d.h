#ifndef CINTERACTIVEIMAGEPROCESS2D_H
#define CINTERACTIVEIMAGEPROCESS2D_H

#include "CImageProcess2d.h"
#include "Graphics/CGraphicsLayer.h"

/**
 * @ingroup groupDataProcess
 * @brief The CInteractiveImageProcess2d class is derived from CImageProcess2d and adds user interactions capability to a 2D image process task.
 * @details This class implements a user interaction mechanism through the use of dedicated graphics layer.
 * When a CInteractiveImageProcess2d instance is active, the system automatically activates this internal graphics layer on which the user can interact by drawing items (points, lines, polygons...).
 * Every changes on this layer are then notified to this class, and actions can be implemented accordingly.
 * This class is used for example to handle interactive segmentation with color picker.
 */
class DATAPROCESSSHARED_EXPORT CInteractiveImageProcess2d : public CImageProcess2d
{
    public:

        using Blobs = std::vector<std::vector<cv::Point>>;

        /**
         * @brief Default constructor.
         * Same inputs and outputs as CImageProcess2d.
         */
        CInteractiveImageProcess2d();
        /**
         * @brief Constructs CInteractiveImageProcess2d with the given name.
         * Same inputs and outputs as CImageProcess2D.
         * @param name: name of the task.
         */
        CInteractiveImageProcess2d(const std::string& name);
        /**
         * @brief Copy constructor.
         */
        CInteractiveImageProcess2d(const CInteractiveImageProcess2d& process);

        virtual ~CInteractiveImageProcess2d() = default;

        /**
         * @brief Makes the task and its graphics interaction layer active or inactive.
         * @param bActive: True to activate, False to desactivate.
         */
        virtual void    setActive(bool bActive) override;

        /**
         * @brief Get the binary mask generated from the graphics interaction layer.
         * @return Binary mask, CMat for C++, Numpy array for Python.
         */
        CMat            getInteractionMask() const;
        /**
         * @brief Gets the list of connected components extracted from the binary interaction mask.
         * @return vector of vector of 2D points.
         */
        Blobs           getBlobs() const;
        /**
         * @brief Gets the last graphics item added to the interaction layer by the user.
         * @return CGraphicsItem pointer.
         */
        QGraphicsItem*  getLastItem() const;
        /**
         * @brief Gets the graphics interaction layer.
         * @return CGraphicsLayer pointer.
         */
        CGraphicsLayer* getInteractionLayer() const;

        /**
         * @brief See CProtocolTask for more details.
         * @return Always True
         */
        bool            isGraphicsChangedListening() const override;

        /**
         * @brief Notifies that the graphics interaction layer has changed.
         */
        virtual void    graphicsChanged() override;
        /**
         * @brief Notifies that the workflow input has changed. The graphics interaction layer is updated.
         * Don't forger to call this method if you override it in a derived class.
         * @param bNewSequence: indicates if it is a new sequence (not a new frame of the same video for example).
         */
        virtual void    globalInputChanged(bool bNewSequence) override;

        /**
         * @brief Generates a binary mask from the graphics interaction layer.
         * @param width: width of the mask (should be the width of the source image).
         * @param height: height of the mask (should be the height of the source image).
         */
        void            createInteractionMask(int width, int height);

        /**
         * @brief Generates the list of connected components from the binary mask.
         */
        void            computeBlobs();

        /**
         * @brief Clear all items in the interaction layer.
         */
        void            clearInteractionLayer();

    /** @cond INTERNAL */
    protected:

        //CInteractiveImageProcess2d object keeps ownership of CGraphicsLayer pointer
        std::unique_ptr<CGraphicsLayer> m_layerPtr;
        CMat                            m_interactionMask;
        std::vector<cv::Vec4i>          m_hierarchy;
        Blobs                           m_blobs;
    /** @endcond */
};

#endif // CINTERACTIVEIMAGEPROCESS2D_H
