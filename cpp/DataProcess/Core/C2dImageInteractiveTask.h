/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef C2DIMAGEINTERACTIVETASK_H
#define C2DIMAGEINTERACTIVETASK_H

#include "C2dImageTask.h"
#include "Graphics/CGraphicsLayer.h"

/**
 * @ingroup groupDataProcess
 * @brief The C2dImageInteractiveTask class is derived from C2dImageTask and adds user interactions capability to a 2D image process task.
 * @details This class implements a user interaction mechanism through the use of dedicated graphics layer.
 * When a C2dImageInteractiveTask instance is active, the system automatically activates this internal graphics layer on which the user can interact by drawing items (points, lines, polygons...).
 * Every changes on this layer are then notified to this class, and actions can be implemented accordingly.
 * This class is used for example to handle interactive segmentation with color picker.
 */
class DATAPROCESSSHARED_EXPORT C2dImageInteractiveTask : public C2dImageTask
{
    public:

        using Blobs = std::vector<std::vector<cv::Point>>;

        /**
         * @brief Default constructor.
         * Same inputs and outputs as C2dImageTask.
         */
        C2dImageInteractiveTask();
        /**
         * @brief Constructs C2dImageInteractiveTask with the given name.
         * Same inputs and outputs as C2dImageTask.
         * @param name: name of the task.
         */
        C2dImageInteractiveTask(const std::string& name);
        /**
         * @brief Copy constructor.
         */
        C2dImageInteractiveTask(const C2dImageInteractiveTask& process);

        virtual ~C2dImageInteractiveTask() = default;

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
         * @brief See CWorkflowTask for more details.
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

        //C2dImageInteractiveTask object keeps ownership of CGraphicsLayer pointer
        std::unique_ptr<CGraphicsLayer> m_layerPtr;
        CMat                            m_interactionMask;
        std::vector<cv::Vec4i>          m_hierarchy;
        Blobs                           m_blobs;
    /** @endcond */
};

#endif // C2DIMAGEINTERACTIVETASK_H
