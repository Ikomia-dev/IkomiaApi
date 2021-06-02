helpFunction()
{
   echo ""
   echo "Usage: $0 [-m clean|build] [-r remote] [-b branch name] [-py python version tag] [-p platform name]"
   exit 1 # Exit script after printing help
}

while getopts "m:r:b:u:" opt
do
   case "$opt" in
      m ) method="$OPTARG" ;;
      r ) remote="$OPTARG" ;;
      b ) branch="$OPTARG" ;;
      py ) pytag="$OPTARG" ;;
      p ) platform="$OPTARG" ;;
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

  cd ../..
  python_lib_dir="ikomia/lib"
  if [ ! -d $python_lib_dir ]
  then
    mkdir $python_lib_dir
  fi

  cp -R cpp/Build/Lib/. $python_lib_dir

  python setup.py bdist_wheel --python-tag="$pytag" --plat-name="$platform"

  echo "Generating wheel successfully"
fi




