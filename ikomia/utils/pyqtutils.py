# Copyright (C) 2021 Ikomia SAS
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Module providing helper functions to design
your own task widget. It uses the PyQt5 framework.
"""

from typing import Optional
from PyQt5.QtWidgets import (  # pylint: disable=E0611
    QWidget, QLineEdit, QFileDialog, QPushButton, QHBoxLayout, QDialog, QLabel, QComboBox, QSpinBox, QDoubleSpinBox,
    QCheckBox, QRadioButton, QSlider
)
from PyQt5.QtCore import Qt, QObject, pyqtSignal  # pylint: disable=E0611


class BrowseFileWidget(QWidget):
    """
    Composite widget to handle file or folder selection.
    """
    file_changed = pyqtSignal()

    def __init__(self, path: str, file_filter: str = "", tooltip: str = "", mode=QFileDialog.ExistingFile,
                 parent: Optional[QObject] = None):
        QWidget.__init__(self, parent)
        self.path = path
        self.tooltip = tooltip
        self.filter = file_filter
        self.mode = mode
        self.qedit_file = None
        self.qbrowse_btn = None
        self.init_layout()
        self.init_connections()

    def init_layout(self):
        """
        Set composite widget horizontal layout.
        """
        self.qedit_file = QLineEdit(self.path)
        self.qbrowse_btn = QPushButton("...")
        self.qbrowse_btn.setToolTip(self.tooltip)

        qlayout = QHBoxLayout()
        qlayout.addWidget(self.qedit_file)
        qlayout.addWidget(self.qbrowse_btn)
        self.setLayout(qlayout)

    def init_connections(self):
        """
        Create connections.
        """
        self.qbrowse_btn.clicked.connect(self.on_browse)
        self.qedit_file.editingFinished.connect(self.on_path_changed)

    def on_browse(self):
        """
        Slot triggered by button click.
        """
        self.path = self.show_select_file_dlg()
        self.qedit_file.setText(self.path)
        self.file_changed.emit()

    def on_path_changed(self):
        """
        Slot triggered when new path is selected.
        """
        self.path = self.qedit_file.text()

    def show_select_file_dlg(self):
        """
        Show file selection dialog.

        Returns:
            str: selected file path
        """
        path = ""
        file_dialog = QFileDialog()
        file_dialog.setNameFilter(self.filter)
        file_dialog.setViewMode(QFileDialog.Detail)
        file_dialog.setFileMode(self.mode)

        if self.mode == QFileDialog.Directory:
            file_dialog.setOption(QFileDialog.ShowDirsOnly, True)

        if file_dialog.exec() == QDialog.Accepted:
            path = file_dialog.selectedFiles()[0]

        return path

    def set_path(self, path: str):
        """
        Set current path.

        Args:
            path (str): new path

        """
        self.path = path
        self.qedit_file.setText(path)

    def clear(self):
        """
        Clear current path.
        """
        self.path = ""
        self.qedit_file.clear()


def add_edit(grid_layout, row: int, label: str, value: str = "") -> QLineEdit:
    """
    Add an edit box and its label in the layout at the given row.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        row (int): row index in the grid layout
        label (str): label in front of the QLineEdit
        value (str): value of the QLineEdit

    Returns:
        QLineEdit: instance of the QLineEdit added to the layout
    """
    qlabel = QLabel(label)
    qedit = QLineEdit()
    qedit.setText(value)
    grid_layout.addWidget(qlabel, row, 0)
    grid_layout.addWidget(qedit, row, 1)
    return qedit


def append_edit(grid_layout, label: str, value: str = "") -> QLineEdit:
    """
    Append an edit box and its label in the layout.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        label (str): label in front of the QLineEdit
        value (str): value of the QLineEdit

    Returns:
        QLineEdit: instance of the QLineEdit added to the layout
    """
    last_pos = grid_layout.rowCount()
    return add_edit(grid_layout, last_pos, label, value=value)


def add_combo(grid_layout, row: int, label: str) -> QComboBox:
    """
    Add a combo box and its label in the layout at the given row.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        row (int): row index in the grid layout
        label (str): label in front of the QComboBox

    Returns:
        QComboBox: instance of the QComboBox added to the layout
    """
    qlabel = QLabel(label)
    qcombo = QComboBox()
    grid_layout.addWidget(qlabel, row, 0)
    grid_layout.addWidget(qcombo, row, 1)
    return qcombo


def append_combo(grid_layout, label: str) -> QComboBox:
    """
    Append a combo box and its label in the layout.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        label (str): label in front of the QComboBox

    Returns:
        QComboBox: instance of the QComboBox added to the layout
    """
    last_pos = grid_layout.rowCount()
    return add_combo(grid_layout, last_pos, label)


def add_spin(grid_layout, row: int, label: str, value: int,
             min: int = 0, max: int = 2147483647, step: int = 1) -> QSpinBox:
    """
    Add a composite widget (label + edit box + spin box)
    in the layout at the given row - **integer values**.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        row (int): row index in the grid layout
        label (str): label in front of the QSpinBox
        value (int): spin box value
        min (int): minimum value of the spin box
        max (int): maximum value of the spin box
        step (int): increment/decrement step of the spin box

    Returns:
        QSpinBox: instance of the QSpinBox added to the layout
    """
    qlabel = QLabel(label)
    qspin = QSpinBox()
    qspin.setRange(min, max)
    qspin.setSingleStep(step)
    qspin.setValue(value)

    grid_layout.addWidget(qlabel, row, 0)
    grid_layout.addWidget(qspin, row, 1)
    return qspin


def append_spin(grid_layout, label: str, value: int,
                min: int = 0, max: int = 2147483647, step: int = 1) -> QSpinBox:
    """
    Append a composite widget (label + edit box + spin box) in the layout -
    **integer values**.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        label (str): label in front of the QSpinBox
        value (int): spin box value
        min (int): minimum value of the spin box
        max (int): maximum value of the spin box
        step (int): increment/decrement step of the spin box

    Returns:
        QSpinBox: instance of the QSpinBox added to the layout
    """
    last_pos = grid_layout.rowCount()
    return add_spin(grid_layout, last_pos, label, value, min, max, step)


def add_double_spin(grid_layout, row: int, label: str,
                    value: float, min: float = 0.0, max: float = float('inf'),
                    step: float = 1, decimals: int = 4) -> QDoubleSpinBox:
    """
    Add a composite widget (label + edit box + spin box)
    in the layout at the given row - **float values**.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        row (int): row index in the grid layout
        label (str): label in front of the QSpinBox
        value (float): spin box value
        min (float): minimum value of the spin box
        max (float): maximum value of the spin box
        step (float): increment/decrement step of the spin box
        decimals (int): number of decimals

    Returns:
        QDoubleSpinBox: instance of the QDoubleSpinBox added to the layout
    """
    qlabel = QLabel(label)
    qspin = QDoubleSpinBox()
    qspin.setDecimals(decimals)
    qspin.setRange(min, max)
    qspin.setValue(value)
    qspin.setSingleStep(step)

    grid_layout.addWidget(qlabel, row, 0)
    grid_layout.addWidget(qspin, row, 1)
    return qspin


def append_double_spin(grid_layout, label: str, value: float, min: float = 0.0, max: float = float('inf'),
                       step: float = 1.0, decimals: int = 4) -> QDoubleSpinBox:
    """
    Append a composite widget (label + edit box + spin box) in the layout -
    **float values**.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        label (str): label in front of the QSpinBox
        value (float): spin box value
        min (float): minimum value of the spin box
        max (float): maximum value of the spin box
        step (float): increment/decrement step of the spin box
        decimals (int): number of decimals

    Returns:
        QDoubleSpinBox: instance of the QDoubleSpinBox added to the layout
    """
    last_pos = grid_layout.rowCount()
    return add_double_spin(grid_layout, last_pos, label, value, min, max, step, decimals)


def add_check(grid_layout, row: int, label: str, checked: bool) -> QCheckBox:
    """
    Add a checkbox and its label in the layout at the given row.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        row (int): row index in the grid layout
        label (str): label in front of the QCheckBox
        checked (bool): check status

    Returns:
        QCheckBox: instance of the QCheckBox added to the layout
    """
    qcheck = QCheckBox(label)
    qcheck.setChecked(checked)
    grid_layout.addWidget(qcheck, row, 0)
    return qcheck


def append_check(grid_layout, label: str, checked: bool) -> QCheckBox:
    """
    Append a checkbox and its label in the layout.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        label (str): label in front of the QCheckBox
        checked (bool): check status

    Returns:
        QCheckBox: instance of the QCheckBox added to the layout
    """
    last_pos = grid_layout.rowCount()
    return add_check(grid_layout, last_pos, label, checked)


def add_radio(grid_layout, row: int, label: str, checked: bool) -> QRadioButton:
    """
    Add a radio button and its label in the layout at the given row.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        row (int): row index in the grid layout
        label (str): label in front of the QRadioButton
        checked (bool): check status

    Returns:
        QRadioButton: instance of the QRadioButton added to the layout
    """
    qradio = QRadioButton(label)
    qradio.setChecked(checked)
    grid_layout.addWidget(qradio, row, 0)
    return qradio


def append_radio(grid_layout, label: str, checked: bool) -> QRadioButton:
    """
    Append a radio button and its label in the layout.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        label (str): label in front of the QRadioButton
        checked (bool): check status

    Returns:
        QRadioButton: instance of the QRadioButton added to the layout
    """
    last_pos = grid_layout.rowCount()
    return add_radio(grid_layout, last_pos, label, checked)


def add_slider(grid_layout, row: int, label: str, value: int,
               min: int = 0, max: int = 2147483647, step: int = 1) -> QSlider:
    """
    Add a slider and its label in the layout at the given row.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        row (int): row index in the grid layout
        label (str): label in front of the QSpinBox
        value (int): slider position
        min (int): minimum slider position
        max (int): maximum slider position
        step (int): increment/decrement step of the slider

    Returns:
        QSlider: instance of the QSlider added to the layout
    """
    qlabel = QLabel(label)
    qslider = QSlider(Qt.Horizontal)
    qslider.setRange(min, max)
    qslider.setSingleStep(step)
    qslider.setValue(value)

    grid_layout.addWidget(qlabel, row, 0)
    grid_layout.addWidget(qslider, row, 1)
    return qslider


def append_slider(grid_layout, label: str, value: int,
                  min: int = 0, max: int = 2147483647, step: int = 1) -> QSlider:
    """
    Append a slider and its label in the layout.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        label (str): label in front of the QSpinBox
        value (int): slider position
        min (int): minimum slider position
        max (int): maximum slider position
        step (int): increment/decrement step of the slider

    Returns:
        QSlider: instance of the QSlider added to the layout
    """
    last_pos = grid_layout.rowCount()
    return add_slider(grid_layout, last_pos, label, value, min, max, step)


def add_browse_file(grid_layout, row: int = 0, label: str = "", path: str = "",
                    file_filter: str = "", tooltip: str = "", mode=QFileDialog.ExistingFile) -> BrowseFileWidget:
    """
    Add a composite widget (label + edit box + browse button)
    in the layout at the given row - **file or folder**.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        row (int): row index in the grid layout
        label (str): label in front of the edit box
        path (str): initial path value
        file_filter (str): list of extension-wildcard patterns, ex: '*.jpg, *.png'
        tooltip (str): tooltip message over the browse button
        mode: mode for the QFileDialog: QFileDialog::AnyFile, QFileDialog::ExistingFile (default),
            QFileDialog::Directory	or QFileDialog::ExistingFiles

    Returns:
        BrowseFileWidget: instance of the BrowseFileWidget added to the layout
    """
    qlabel = QLabel(label)
    qbrowse_widget = BrowseFileWidget(path=path, file_filter=file_filter, tooltip=tooltip, mode=mode)
    grid_layout.addWidget(qlabel, row, 0)
    grid_layout.addWidget(qbrowse_widget, row, 1)
    return qbrowse_widget


def append_browse_file(grid_layout, label: str = "", path: str = "",
                       file_filter: str = "", tooltip: str = "", mode=QFileDialog.ExistingFile) -> BrowseFileWidget:
    """
    Append a composite widget (label + edit box + browse button) in the layout -
    **file or folder**.

    Args:
        grid_layout (QGridLayout): Qt grid layout
        label (str): label in front of the edit box
        path (str): initial path value
        file_filter (str): list of extension-wildcard patterns, ex: '*.jpg, *.png'
        tooltip (str): tooltip message over the browse button
        mode: mode for the QFileDialog: QFileDialog::AnyFile, QFileDialog::ExistingFile (default),
            QFileDialog::Directory	or QFileDialog::ExistingFiles

    Returns:
        BrowseFileWidget: instance of the BrowseFileWidget added to the layout
    """
    last_pos = grid_layout.rowCount()
    return add_browse_file(grid_layout, row=last_pos, label=label, path=path,
                           file_filter=file_filter, tooltip=tooltip, mode=mode)
