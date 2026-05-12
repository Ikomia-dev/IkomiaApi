LD_LIBRARY_PATH="/usr/local/lib:$(pwd)/../../IkomiaCore/Build/lib:$(pwd)/../../gmic/build:$(pwd)/../../qwt/build/lib"
export LD_LIBRARY_PATH
PYTHON=${PYTHON:-python}
# Generate stubs for C++ libs
$PYTHON gen_stubs.py -m ikomia.core.pycore -o $(pwd)/../
$PYTHON gen_stubs.py -m ikomia.dataio.pydataio -o $(pwd)/../
$PYTHON gen_stubs.py -m ikomia.dataprocess.pydataprocess -o $(pwd)/../
$PYTHON gen_stubs.py -m ikomia.utils.pyutils -o $(pwd)/../
