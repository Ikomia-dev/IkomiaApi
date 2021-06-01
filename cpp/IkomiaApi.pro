TEMPLATE = subdirs

SUBDIRS += \
    Core \
    DataIO \
    PyDataIO \
    Utils \
    DataManagement \
    DataPlot \
    DataProcess \
    PyCore \
    PyUtils \
    PyDataProcess \
    VolumeRender \
    UnitTests

Core.depends = Utils
DataIO.depends = Utils Core
DataManagement.depends = Utils Core DataIO
DataPlot.depends = Utils
DataProcess.depends = Utils Core
VolumeRender.depends = Utils Core
PyUtils.depends = Utils
PyCore.depends = Utils Core
PyDataProcess.depends = Utils Core DataProcess
PyDataIO.depends = Utils Core DataIO
UnitTests.depends = Utils Core DataIO DataPlot DataProcess VolumeRender

equals(TEMPLATE, subdirs): prepareRecursiveTarget(vera++)
QMAKE_EXTRA_TARGETS += vera++
