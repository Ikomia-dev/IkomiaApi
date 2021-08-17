helpFunction()
{
   echo ""
   echo "Usage: $0 [-m clean|build] [-r remote] [-b branch name] [-v python version suffix] [-t python version tag] [-p platform name] [-w]"
   exit 1 # Exit script after printing help
}

build_wheel=0

while getopts "m:r:b:v:t:p:w" opt
do
   case "$opt" in
      m ) method="$OPTARG" ;;
      r ) remote="$OPTARG" ;;
      b ) branch="$OPTARG" ;;
      v ) pyversion="$OPTARG" ;;
      t ) pytag="$OPTARG" ;;
      p ) platform="$OPTARG" ;;
      w ) build_wheel=1 ;;
   esac
done

if [ -z "$method" ]
then
   method="build"
fi

if [ -z "$remote" ]
then
   remote="origin"
fi

if [ -z "$branch" ]
then
   branch="main"
fi

if [ -z "$pyversion" ]
then
   pyversion="3"
fi

if [ -z "$pytag" ]
then
   pytag="cp38"
fi

if [ -z "$platform" ]
then
   platform="manylinux2014"
fi

if [ $method = "clean" ]
then
  rm -rf build
  rm -rf dist
  rm -rf ikomia.egg-info
  echo "Cleaning wheel successfully"
else
  echo "----- Build Ikomia C++ libs -----"
  build_dir="cpp/Build"
  if [ ! -d $build_dir ]
  then
    mkdir $build_dir
  fi

  git pull $remote $branch
  cd $build_dir
  qmake ../IkomiaApi.pro
  make -j12
  make install
  echo "----- Build done-----"

  if [ $build_wheel = 1 ]
  then
    cd ../..
    python_lib_dir="ikomia/lib"
    if [ ! -d $python_lib_dir ]
    then
      mkdir $python_lib_dir
    fi

    echo "----- Copy C++ libs to Python package -----"
    cp cpp/Build/Lib/libikUtils* $python_lib_dir
    cp cpp/Build/Lib/libikCore* $python_lib_dir
    cp cpp/Build/Lib/libikDataIO* $python_lib_dir
    cp cpp/Build/Lib/libikDataProcess* $python_lib_dir
    echo "----- Copy done -----"

    echo "----- Bundle dependencies -----"
    cd $python_lib_dir
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD linuxdeployqt libikUtils.so -bundle-non-qt-libs
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD linuxdeployqt libikCore.so -bundle-non-qt-libs
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD linuxdeployqt libikDataIO.so -bundle-non-qt-libs
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD linuxdeployqt libikDataProcess.so -bundle-non-qt-libs
    cp -f /work/shared/local/lib/python3.7/site-packages/cv2/python-3.7/cv2.cpython-37m-x86_64-linux-gnu.so .
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD linuxdeployqt cv2.cpython-37m-x86_64-linux-gnu.so -bundle-non-qt-libs
    mv lib/* .
    rm -rf lib
    mv cv2.cpython-37m-x86_64-linux-gnu.so ../opencv
    echo "----- Bundle dependencies done -----"

    echo "----- Update Ikomia libs RPATH -----"
    patchelf --set-rpath "\$ORIGIN" libikUtils.so
    patchelf --set-rpath "\$ORIGIN" libikCore.so
    patchelf --set-rpath "\$ORIGIN" libikDataIO.so
    patchelf --set-rpath "\$ORIGIN" libikDataProcess.so
    cd ..
    patchelf --set-rpath "\$ORIGIN/../lib" utils/pyutils.so
    patchelf --set-rpath "\$ORIGIN/../lib" core/pycore.so
    patchelf --set-rpath "\$ORIGIN/../lib" dataio/pydataio.so
    patchelf --set-rpath "\$ORIGIN/../lib" dataprocess/pydataprocess.so

    patchelf --set-rpath "\$ORIGIN/../lib" opencv/cv2.cpython-37m-x86_64-linux-gnu.so
    echo "----- Patch Python libs done -----"

    echo "----- Generating Python wheel -----"
    cd ..
    python$pyversion setup.py bdist_wheel --python-tag="$pytag" --plat-name="$platform"
    echo "----- Python wheel done -----"
  fi
fi




