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

#ifndef CIMAGEIO_H
#define CIMAGEIO_H

#include "DataProcessGlobal.hpp"
#include "Data/CMat.hpp"
#include "Workflow/CWorkflowTaskIO.h"
#include "CGraphicsInput.h"
#include "CGraphicsOutput.h"

/**
 * @ingroup groupDataProcess
 * @brief The CImageIO class defines an input or output for a workflow task dedicated to image management.
 * @details This class is designed to handle image as input or output of a workflow task.
 * A CImageIO instance can be added as input or output to a CWorkflowTask or derived object.
 * It is the base class to define input or output of an image processing task.
 * Several image output type can be defined according to the nature of the implemented algorithm.
 * Image processing task can output binary, labelled or standard image.
 * The internal image data structure can be either 2D or 3D.
 * For Python, image data is a numpy array.
 */
class DATAPROCESSSHARED_EXPORT CImageIO : public CWorkflowTaskIO
{
    public:

        /**
         * @brief Default constructor
         */
        CImageIO();
        /**
         * @brief Constructs a CImageIO instance with the given image output type.
         * @param data: see ::IODataType for details.
         */
        CImageIO(IODataType data);
        /**
         * @brief Constructs a CImageIO instance with the given image output type and the name.
         * @param data: see ::IODataType for details.
         */
        CImageIO(IODataType data, const CMat& image);
        /**
         * @brief Constructs a CImageIO instance with the given image output type and the given image.
         * @param data: see ::IODataType for details.
         * @param image: CMat object for C++ and Numpy array for Python.
         */
        CImageIO(IODataType data, const CMat& image, const std::string& name);
        /**
         * @brief Constructs a CImageIO instance with the given image output type and the name.
         * @param data: see ::IODataType for details.
         * @param image: CMat object for C++ and Numpy array for Python.
         */
        CImageIO(IODataType data, const std::string& name);
        /**
         * @brief Constructs a CImageIO instance with the given image output type and the image path.
         * @param data: see ::IODataType for details.
         * @param image: CMat object for C++ and Numpy array for Python.
         */
        CImageIO(IODataType data, const std::string& name, const std::string& path);
        /**
         * @brief Copy constructor.
         */
        CImageIO(const CImageIO& io);
        /**
         * @brief Universal reference copy constructor.
         */
        CImageIO(const CImageIO&& io);

        virtual ~CImageIO() = default;

        /**
         * @brief Assignement operator.
         */
        CImageIO& operator=(const CImageIO& io);
        /**
         * @brief Universal reference assignement operator.
         */
        CImageIO& operator=(const CImageIO&& io);

        /**
         * @brief Sets the image.
         * @param image: CMat object for C++ and Numpy array for Python.
         */
        void            setImage(const CMat& image);
        /**
         * @brief Sets the channel count property for the input/output.
         * @details This property holds the required channel count as a static information.
         * Such information can be useful when one designs a workflow and wants to inform about specific image structure towards future connected tasks.
         * @param nb: channel count, can be 1 for monochrome or 3 for color image.
         */
        void            setChannelCount(size_t nb);
        /**
         * @brief Sets the index of the current image into a 3D volume.
         * @details Image data structure can be 2D or 3D. This methods sets the index of the current 2D plane from a 3D volume.
         * @param index: index of the 2D plane.
         */
        void            setCurrentImage(size_t index);
        /**
         * @brief Sets the associated overlay mask.
         * @details Ikomia software is able to display overlay mask on top of image.
         * This method sets this mask, it will be displayed automatically according to a predefined color map.
         * The color map must be defined in the task implementation.
         * @param mask: CMat object for C++ and Numpy array for Python. 8 bits - 1 channel.
         */
        void            setOverlayMask(const CMat& mask);

        /**
         * @brief Gets the static channel count information.
         * @return Channel count.
         */
        size_t          getChannelCount() const;
        /**
         * @brief Gets the image data.
         * @details Returned image can be either 2D or 3D.
         * @return CMat object for C++ and Numpy array for Python.
         */
        CMat            getData() const;
        /**
         * @brief Gets the image information.
         * @details Information like width,height,bit depth, channel count and basic statistics can be retrieved.
         * @return CDataInfoPtr object or derived.
         */
        virtual CDataInfoPtr    getDataInfo() override;
        /**
         * @brief Gets the 2D image data.
         * @details Returned image is 2D only. In case of 3D volume, the current image index is used to get the desired 2D plane (see setCurrentImage()).
         * @return CMat object for C++ and Numpy array for Python. 2D only.
         */
        virtual CMat    getImage();
        /**
         * @brief Gets the 2D image data with graphics items from the given input/output.
         * @details Returned image is 2D only. In case of 3D volume, the current image index is used to get the desired 2D plane (see setCurrentImage()).
         * @return CMat object for C++ and Numpy array for Python. 2D only.
         */
        CMat            getImageWithGraphics(const GraphicsInputPtr& graphics);
        CMat            getImageWithGraphics(const GraphicsOutputPtr& graphics);

        /**
         * @brief Gets the number of unit elements when the data is processed.
         * @details The number of unit elements (or atomic) is used to calculate the number of progress steps needed to perform a task.
         * In case of 2D image, we won't update progress bar every pixel, so the return value should be 1 (1 update per image).
         * In case of 3D image, update progress every 2D plane can make sense for task processing volume on 2D planes independently.
         * So user have to define the number of unit elements through the progress bar update perspective.
         * @return Number of unit element.
         */
        size_t          getUnitElementCount() const override;
        /**
         * @brief Gets the overlay mask.
         * @return CMat object for C++ and Numpy array for Python.
         */
        CMat            getOverlayMask() const;

        std::vector<DataFileFormat> getPossibleSaveFormats() const override;

        /**
         * @brief Checks whether the input/output have valid image or not.
         * @return True if image is not empty, False otherwise.
         */
        bool            isDataAvailable() const override;
        /**
         * @brief Checks whether the input/output have valid overlay mask or not.
         * @return True if overlay mask is not empty, False otherwise.
         */
        bool            isOverlayAvailable() const;
        /**
         * @brief Clears image and overlay mask so that they become empty.
         */
        void            clearData() override;

        /**
         * @brief Performs a copy of the static information from input/output to another input/output.
         * @details For this class, the channel count is the only static data.
         * @param ioPtr: input/output to copy static data from.
         */
        void            copyStaticData(const WorkflowTaskIOPtr& ioPtr) override;

        void            drawGraphics(const GraphicsInputPtr& graphics);
        void            drawGraphics(const GraphicsOutputPtr& graphics);

        void            save() override;
        void            save(const std::string &path) override;

        /**
         * @brief Performs a deep copy the this instance
         * @return CImageIO smart pointer (std::shared_ptr).
         */
        std::shared_ptr<CImageIO>            clone() const;

    private:

        virtual std::shared_ptr<CWorkflowTaskIO>    cloneImp() const override;

    public:

        size_t          m_channelCount = 0;

    protected:

        CMat            m_image;
        CMat            m_overlayMask;
        size_t          m_currentIndex = 0;
};

using ImageIOPtr = std::shared_ptr<CImageIO>;

class DATAPROCESSSHARED_EXPORT CImageIOFactory: public CWorkflowTaskIOFactory
{
    public:

        CImageIOFactory()
        {
            m_name = "CImageIO";
        }

        virtual WorkflowTaskIOPtr   create(IODataType dataType)
        {
            return std::make_shared<CImageIO>(dataType);
        }
};

#endif // CIMAGEIO_H
