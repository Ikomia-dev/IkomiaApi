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

#ifndef CPROTOCOLTASKWIDGET_HPP
#define CPROTOCOLTASKWIDGET_HPP

#include <memory>
#include <QtWidgets>
#include <QPushButton>
#include "CProtocolTaskParam.h"
#include "Main/CoreGlobal.hpp"
#include "Graphics/CGraphicsItem.hpp"
#include "Widgets/CColorPushButton.h"
#include "Widgets/CBrowseFileWidget.h"

/**
 * @ingroup groupCore
 * @brief The CProtocolTaskWidget class must be the base class for all widget class associated with a process task.
 * Each process comes with a Qt widget to manage algorithm parameters and user actions. The widget class implementation is mandatory.
 * This class offers some basic tools to build rapidly a functionnal user interface.
 * As it is derived from Qt QWidget, one can easily customize it to fit more complex needs.
 * For Python, use of bindings like PyQt5 or PySide2 are recommended.
 */
class CORESHARED_EXPORT CProtocolTaskWidget : public QWidget
{
    Q_OBJECT

    public:

        /**
        * @brief Default constructor
        */
        CProtocolTaskWidget(QWidget *parent = Q_NULLPTR);
        virtual ~CProtocolTaskWidget() = default;

        /**
         * @brief Makes the Apply button of the widget visible or not.
         * @param bHidden: True if button must be hidden, False otherwise.
         */
        void            setApplyBtnHidden(bool bHidden);

        virtual void    onApply();

    signals:

        /**
         * @brief Emit this signal to run the process.
         * A common use is to emit the signal in response of the clicked signal of the Apply button.
         * @param pParam: CProtocolTaskParam base shared pointer.
         */
        void            doApplyProcess(const ProtocolTaskParamPtr& pParam);
        /**
         * @brief Emit this signal to request specific action of the process task.
         * Use this mechanism to manage user interaction with the process task.
         * @param flags: action code (user defined).
         */
        void            doSendProcessAction(int flags);
        /**
         * @brief Emit this signal to change the current graphics tool.
         * @param tool: see ::GraphicsShape.
         */
        void            doSetGraphicsTool(GraphicsShape tool);
        /**
         * @brief Emit this signal to change the current graphics category.
         * @param category: category name.
         */
        void            doSetGraphicsCategory(const QString& category);

    protected:

        /**
         * @brief Initialization of the widget. Must be overriden.
         */
        virtual void    init() = 0;

        /**
         * @brief Tool method to add combo box into the internal grid layout.
         * @param row: zero based index to set where the combo box is placed.
         * @param title: label associated with the combo box.
         * @return Pointer to the newly created combo box.
         */
        QComboBox*      addCombo(int row, const QString& title);
        /**
         * @brief Tool method to add combo box at the end of the internal grid layout.
         * @param title: label associated with the combo box.
         * @return Pointer to the newly created combo box.
         */
        QComboBox*      addCombo(const QString& title);
        /**
         * @brief Tool method to add integer spin box into the internal grid layout.
         * @param row: zero based index to set where the spin box is placed.
         * @param title: label associated with the spin box.
         * @param value: initial value.
         * @param min: minimum possible value.
         * @param max: maximum possible value.
         * @param step: increment/decrement step.
         * @return Pointer to the newly created spin box.
         */
        QSpinBox*       addSpin(int row, const QString& title, int value, int min=0, int max=INT_MAX, int step=1);        
        /**
         * @brief Tool method to add integer spin box at the end of the internal grid layout.
         * @param title: label associated with the spin box.
         * @param value: initial value.
         * @param min: minimum possible value.
         * @param max: maximum possible value.
         * @param step: increment/decrement step.
         * @return Pointer to the newly created spin box.
         */
        QSpinBox*       addSpin(const QString& title, int value, int min=0, int max=INT_MAX, int step=1);
        /**
         * @brief Tool method to add floating point spin box into the internal grid layout.
         * @param row: zero based index to set where the spin box is placed.
         * @param title: label associated with the spin box.
         * @param value: initial value
         * @param min: minimum possible value.
         * @param max: maximum possible value.
         * @param step: increment/decrement step.
         * @param decimals: number of decimals.
         * @return Pointer to the newly created spin box.
         */
        QDoubleSpinBox* addDoubleSpin(int row, const QString& title, double value, double min=0.0, double max=DBL_MAX, double step=1.0, int decimals=4);
        /**
         * @brief Tool method to add floating point spin box at the end of the internal grid layout.
         * @param title: label associated with the spin box.
         * @param value: initial value
         * @param min: minimum possible value.
         * @param max: maximum possible value.
         * @param step: increment/decrement step.
         * @param decimals: number of decimals.
         * @return Pointer to the newly created spin box.
         */
        QDoubleSpinBox* addDoubleSpin(const QString& title, double value, double min=0.0, double max=DBL_MAX, double step=1.0, int decimals=4);
        /**
         * @brief Tool method to add check box into the internal grid layout.
         * @param row: zero based index to set where the check box is placed.
         * @param title: label associated with the check box.
         * @param isChecked: initial state.
         * @return Pointer to the newly created check box.
         */
        QCheckBox*      addCheck(int row, const QString& title, bool isChecked);
        /**
         * @brief Tool method to add check box at the end of the internal grid layout.
         * @param title: label associated with the check box.
         * @param isChecked: initial state.
         * @return Pointer to the newly created check box.
         */
        QCheckBox*      addCheck(const QString& title, bool isChecked);
        /**
         * @brief Tool method to add radio button into the internal grid layout.
         * @param row: zero based index to set where the radio button is placed.
         * @param title: label associated with the radio button.
         * @param isChecked: initial state.
         * @return Pointer to the newly created radio button.
         */
        QRadioButton*   addRadio(int row, const QString& title, bool isChecked);
        /**
         * @brief Tool method to add radio button at the end of the internal grid layout.
         * @param title: label associated with the radio button.
         * @param isChecked: initial state.
         * @return Pointer to the newly created radio button.
         */
        QRadioButton*   addRadio(const QString& title, bool isChecked);
        /**
         * @brief Tool method to add slider into the internal grid layout.
         * @param row: zero based index to set where the slider is placed.
         * @param title: label associated with the slider.
         * @param value: initial value.
         * @param min: minimum possible value.
         * @param max: maximum possible value.
         * @param step: increment/decrement step
         * @return Pointer to the newly created slider.
         */
        QSlider*        addSlider(int row, const QString& title, int value, int min=0, int max=INT_MAX, int step=1);
        /**
         * @brief Tool method to add slider at the end of the internal grid layout.
         * @param title: label associated with the slider.
         * @param value: initial value.
         * @param min: minimum possible value.
         * @param max: maximum possible value.
         * @param step: increment/decrement step
         * @return Pointer to the newly created slider.
         */
        QSlider*        addSlider(const QString& title, int value, int min=0, int max=INT_MAX, int step=1);
        /**
         * @brief Tool method to add color button into the internal grid layout.
         * @param row: zero based index to set where the button is placed.
         * @param title: label associated with the button.
         * @param color: initial color.
         * @return Pointer to the newly created button.
         */
        CColorPushButton*   addColorButton(int row, const QString& title, const QColor& color);
        /**
         * @brief Tool method to add color button at the end of the internal grid layout.
         * @param title: label associated with the button.
         * @param color: initial color.
         * @return Pointer to the newly created button.
         */
        CColorPushButton*   addColorButton(const QString& title, const QColor& color);
        /**
         * @brief Tool method to add browse file widget into the internal grid layout.
         * @param row: zero based index to set where the button is placed.
         * @param title: label associated with the widget.
         * @param path: initial file path.
         * @param tootip: message shown in the browse button tooltip.
         * @return Pointer to the newly created button.
         */
        CBrowseFileWidget*  addBrowseFile(int row, const QString& title, const QString& path, const QString& tooltip);
        /**
         * @brief Tool method to add browse file widget at the end of the internal grid layout.
         * @param row: zero based index to set where the button is placed.
         * @param title: label associated with the widget.
         * @param path: initial file path.
         * @param tootip: message shown in the browse button tooltip.
         * @return Pointer to the newly created button.
         */
        CBrowseFileWidget*  addBrowseFile(const QString& title, const QString& path, const QString& tooltip);
        /**
         * @brief Tool method to add browse folder widget into the internal grid layout.
         * @param row: zero based index to set where the button is placed.
         * @param title: label associated with the widget.
         * @param path: initial folder path.
         * @param tootip: message shown in the browse button tooltip.
         * @return Pointer to the newly created button.
         */
        CBrowseFileWidget*  addBrowseFolder(int row, const QString& title, const QString& path, const QString& tooltip);
        /**
         * @brief Tool method to add browse folder widget at the end of the internal grid layout.
         * @param row: zero based index to set where the button is placed.
         * @param title: label associated with the widget.
         * @param path: initial folder path.
         * @param tootip: message shown in the browse button tooltip.
         * @return Pointer to the newly created button.
         */
        CBrowseFileWidget*  addBrowseFolder(const QString& title, const QString& path, const QString& tooltip);

    protected:

        QPushButton*    m_pApplyBtn = nullptr;
        QGridLayout*    m_pLayout = nullptr;
        QScrollArea*    m_pScrollArea = nullptr;
};

using ProtocolTaskWidgetPtr = std::shared_ptr<CProtocolTaskWidget>;
using ProtocolTaskWidgetUPtr = std::unique_ptr<CProtocolTaskWidget>;

#endif // CPROTOCOLTASKWIDGET_HPP
