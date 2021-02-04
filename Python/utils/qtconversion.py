#!/usr/bin/env python
# -*- coding: utf-8 -*-

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
