@echo off

SET GENERATOR="Visual Studio 11 Win64"
SET PACKAGE_NAME=GPGOAP
SET PACKAGE_VERSION=1.0
SET BUILD_MODES=Debug Release
SET PLATFORMS=win64
SET BUILD=STATIC

mkdir build_2012_64
cd build_2012_64

for %%k in (%BUILD_MODES%) do @(
    del CMakeCache.txt
    cmake -G%GENERATOR% .. -DCMAKE_INSTALL_PREFIX=../%PLATFORMS%/%%k
    cmake --build . --target INSTALL --config %%k
)

cd ..
