@echo off

SET GENERATOR="Visual Studio 11"
SET PACKAGE_NAME=GPGOAP
SET PACKAGE_VERSION=1.0
SET BUILD_MODES=Debug Release
SET PLATFORMS=win32
SET BUILD=STATIC

mkdir build_2012_32
cd build_2012_32

for %%k in (%BUILD_MODES%) do @(
    del CMakeCache.txt
    cmake -G%GENERATOR% .. -DCMAKE_INSTALL_PREFIX=../%PLATFORMS%/%%k
    cmake --build . --target INSTALL --config %%k
)

cd ..
