helpFunction()
{
   echo ""
   echo "Usage: $0 [-m clean|build] [-r remote] [-b branch name] [-v python version tag] [-p platform name] [-w]"
   exit 1 # Exit script after printing help
}

build_wheel=0

while getopts "m:r:b:v:p:w" opt
do
   case "$opt" in
      m ) method="$OPTARG" ;;
      r ) remote="$OPTARG" ;;
      b ) branch="$OPTARG" ;;
      v ) pytag="$OPTARG" ;;
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
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/ikomia/lib linuxdeployqt $PWD/ikomia/lib/pyutils.so -bundle-non-qt-libs
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/ikomia/lib linuxdeployqt $PWD/ikomia/lib/pycore.so -bundle-non-qt-libs
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/ikomia/lib linuxdeployqt $PWD/ikomia/lib/pydataio.so -bundle-non-qt-libs
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/ikomia/lib linuxdeployqt $PWD/ikomia/lib/pydataprocess.so -bundle-non-qt-libs
    echo "----- Bundle dependencies done -----"

    echo "----- Generating Python wheel -----"
    python setup.py bdist_wheel --python-tag="$pytag" --plat-name="$platform"
    echo "----- Python wheel done -----"
  fi
fi




