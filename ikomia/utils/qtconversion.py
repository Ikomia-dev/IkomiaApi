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
Module dedicated to wrap widget instance from Qt-based Python frameworks to C++ Qt.
"""


def PyQtToQt(pyqt_widget):
    """
    Wraps PyQt widget instance to C++ Qt

    Parameters
    ----------
    pyqt_widget : instance of the PyQt object

    Returns
    -------
    C++ wrapper instance
    """
    import sip
    return sip.unwrapinstance(pyqt_widget)


def PySideToQt(pyside_widget):
    """
    Wraps PySide2 widget instance to C++ Qt

    Parameters
    ----------
    pyqt_widget : instance of the PyQt object

    Returns
    -------
    C++ wrapper instance
    """
    import shiboken2
    return shiboken2.getCppPointer(pyside_widget)[0]
