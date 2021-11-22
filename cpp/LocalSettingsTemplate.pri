#-------------------------------------------------------------------------------------------#
# Fichier de configuration Qt pour définir les variables utilisées par qmake pour le projet #
# Les variables définies dans ce fichier sont dépendantes de la plateforme de compilation   #
# Ce fichier est un template et doit être copié sous le nom LocalSettings.pri               #
#-------------------------------------------------------------------------------------------#

# Uncomment the following line to enable unit tests compilation
#CONFIG += IKOMIA_TESTS
# Uncomment to enable CPU mode
#CONFIG += ik_cpu
# LINUX deployment (CENTOS 7)
CONFIG += centos centos7

########################
# Dependencies version #
########################

# Python
# Arch
#unix:!macx:PYTHON_VERSION = 3.8
#unix:!macx:PYTHON_VERSION_EXT = 3.8
# Centos7
unix:!macx:PYTHON_VERSION = 3.7m
unix:!macx:PYTHON_VERSION_EXT = 3.7m
win32: PYTHON_VERSION = 3.8
win32: PYTHON_VERSION_EXT = 38
macx: PYTHON_VERSION = 3.7

# VTK
unix:!macx:VTK_VERSION = 8.1
win32: VTK_VERSION = 8.1
macx: VTK_VERSION = 8.1

# OpenCV
win32: OPENCV_VERSION = 412

# Boost
win32: BOOST_VERSION = 1_71
win32: BOOST_VC_VERSION = 142

# CUDA
win32: CUDA_VERSION = 10.1

# Microsoft Visual Studio
win32: MSVC_VERSION = 16
