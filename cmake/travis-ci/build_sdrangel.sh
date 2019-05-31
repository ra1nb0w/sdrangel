#!/bin/sh

if [ "${TRAVIS_OS_NAME}" == "osx" ]; then
  JOBS=$(sysctl -n hw.ncpu)
elif [ "${TRAVIS_OS_NAME}" == "linux" ]; then
  JOBS=$(nproc --all)
else
  JOBS=1
fi


if [ "${TRAVIS_OS_NAME}" == "linux" ]; then
  debuild -i -us -uc -b
else
  mkdir build && cd build
  cmake .. "${CMAKE_CUSTOM_OPTIONS}"

  case "${CMAKE_CUSTOM_OPTIONS}" in
    *BUNDLE=ON*)
      make -j${JOBS} package
    ;;
    *)
      make -j${JOBS}
    ;;
  esac
fi
