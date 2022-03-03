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


from PyQt5.QtWidgets import *
from PyQt5.QtCore import Qt, QObject, pyqtSignal


class BrowseFileWidget(QWidget):
    """
    Composite widget to handle file or folder selection.
    """
    file_changed = pyqtSignal()

    def __init__(self, path, filter="", tooltip="", mode=QFileDialog.ExistingFile, parent=None):
        QWidget.__init__(self, parent)
        self.path = path
        self.tooltip = tooltip
        self.filter = filter
        self.mode = mode
        self.qedit_file = None
        self.qbrowse_btn = None
        self.init_layout()
        self.init_connections()

    def init_layout(self):
        self.qedit_file = QLineEdit(self.path)
        self.qbrowse_btn = QPushButton("...")
        self.qbrowse_btn.setToolTip(self.tooltip)

        qlayout = QHBoxLayout()
        qlayout.addWidget(self.qedit_file)
        qlayout.addWidget(self.qbrowse_btn)
        self.setLayout(qlayout)

    def init_connections(self):
        self.qbrowse_btn.clicked.connect(self.on_browse)
        self.qedit_file.editingFinished.connect(self.on_path_changed)

    def on_browse(self):
        self.path = self.show_select_file_dlg()
        self.qedit_file.setText(self.path)
        self.file_changed.emit()

    def on_path_changed(self):
        self.path = self.qedit_file.text()

    def show_select_file_dlg(self):
        path = ""
        file_dialog = QFileDialog()
        file_dialog.setNameFilter(self.filter)
        file_dialog.setViewMode(QFileDialog.Detail)
        file_dialog.setFileMode(self.mode)

        if self.mode == QFileDialog.Directory:
            file_dialog.setOption(QFileDialog.ShowDirsOnly, True)

        if file_dialog.exec() == QDialog.Accepted:
            path = file_dialog.selectedFiles()[0]

        return path;

    def set_path(self, path):
        self.path = path
        self.qedit_file.setText(path)

    def clear(self):
        self.path = ""
        self.qedit_file.clear()


def add_edit(grid_layout, row, label, value=""):
    """
    Add an edit box and its label in the layout at the given row.
    """
    qlabel = QLabel(label)
    qedit = QLineEdit()
    qedit.setText(value)
    grid_layout.addWidget(qlabel, row, 0)
    grid_layout.addWidget(qedit, row, 1)
    return qedit


def append_edit(grid_layout, label, value=""):
    """
    Append an edit box and its label in the layout.
    """
    last_pos = grid_layout.rowCount()
    return add_edit(grid_layout, last_pos, label, value=value)


def add_combo(grid_layout, row, label):
    """
    Add a combo box and its label in the layout at the given row.
    """
    qlabel = QLabel(label)
    qcombo = QComboBox()
    grid_layout.addWidget(qlabel, row, 0)
    grid_layout.addWidget(qcombo, row, 1)
    return qcombo


def append_combo(grid_layout, label):
    """
    Append a combo box and its label in the layout.
    """
    last_pos = grid_layout.rowCount()
    return add_combo(grid_layout, last_pos, label)


def add_spin(grid_layout, row, label, value, min=0, max=2147483647, step=1):
    """
    Add a composite widget (label + edit box + spin box)
    in the layout at the given row - **integer values**.
    """
    qlabel = QLabel(label)
    qspin = QSpinBox()
    qspin.setRange(min, max)
    qspin.setSingleStep(step)
    qspin.setValue(value)

    grid_layout.addWidget(qlabel, row, 0)
    grid_layout.addWidget(qspin, row, 1)
    return qspin


def append_spin(grid_layout, label, value, min=0, max=2147483647, step=1):
    """
    Append a composite widget (label + edit box + spin box) in the layout -
    **integer values**.
    """
    last_pos = grid_layout.rowCount()
    return add_spin(grid_layout, last_pos, label, value, min, max, step)


def add_double_spin(grid_layout, row, label, value, min=0, max=float('inf'), step=1, decimals=4):
    """
    Add a composite widget (label + edit box + spin box)
    in the layout at the given row - **float values**.
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


def append_double_spin(grid_layout, label, value, min=0, max=float('inf'), step=1, decimals=4):
    """
    Append a composite widget (label + edit box + spin box) in the layout -
    **float values**.
    """
    last_pos = grid_layout.rowCount()
    return add_double_spin(grid_layout, last_pos, label, value, min, max, step, decimals)


def add_check(grid_layout, row, label, checked):
    """
    Add a check box and its label in the layout at the given row.
    """
    qcheck = QCheckBox(label)
    qcheck.setChecked(checked)
    grid_layout.addWidget(qcheck, row, 0)
    return qcheck


def append_check(grid_layout, label, checked):
    """
    Append a check box and its label in the layout.
    """
    last_pos = grid_layout.rowCount()
    return add_check(grid_layout, last_pos, label, checked)


def add_radio(grid_layout, row, label, checked):
    """
    Add a radio button and its label in the layout at the given row.
    """
    qradio = QRadioButton(label)
    qradio.setChecked(checked)
    grid_layout.addWidget(qradio, row, 0)
    return qradio


def append_radio(grid_layout, label, checked):
    """
    Append a radio button and its label in the layout.
    """
    last_pos = grid_layout.rowCount()
    return add_radio(grid_layout, last_pos, label, checked)


def add_slider(grid_layout, row, label, value, min=0, max=2147483647, step=1):
    """
    Add a slider and its label in the layout at the given row.
    """
    qlabel = QLabel(label)
    qslider = QSlider(Qt.Horizontal)
    qslider.setRange(min, max)
    qslider.setSingleStep(step)
    qslider.setValue(value)

    grid_layout.addWidget(qlabel, row, 0)
    grid_layout.addWidget(qslider, row, 1)
    return qslider


def append_slider(grid_layout, label, value, min=0, max=2147483647, step=1):
    """
    Append a slider and its label in the layout.
    """
    last_pos = grid_layout.rowCount()
    return add_slider(grid_layout, last_pos, label, value, min, max, step)


def add_browse_file(grid_layout, row=0, label="", path="", filter="", tooltip="", mode=QFileDialog.ExistingFile):
    """
    Add a composite widget (label + edit box + browse button)
    in the layout at the given row - **file or folder**.
    """
    qlabel = QLabel(label)
    qbrowse_widget = BrowseFileWidget(path=path, filter=filter, tooltip=tooltip, mode=mode)
    grid_layout.addWidget(qlabel, row, 0)
    grid_layout.addWidget(qbrowse_widget, row, 1)
    return qbrowse_widget


def append_browse_file(grid_layout, label="", path="", filter="", tooltip="", mode=QFileDialog.ExistingFile):
    """
    Append a composite widget (label + edit box + browse button) in the layout -
    **file or folder**.
    """
    last_pos = grid_layout.rowCount()
    return add_browse_file(grid_layout, row=last_pos, label=label, path=path, filter=filter, tooltip=tooltip, mode=mode)
