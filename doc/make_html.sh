LD_LIBRARY_PATH="/usr/local/lib:$(pwd)/../../IkomiaCore/Build/lib"
export LD_LIBRARY_PATH
sphinx-autobuild source build/html
