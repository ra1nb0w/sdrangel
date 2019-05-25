#!/bin/sh

if [ "${TRAVIS_OS_NAME}" == "osx" ]; then
  JOBS=$(sysctl -n hw.ncpu)
elif [ "${TRAVIS_OS_NAME}" == "linux" ] || [ ${CI_LINUX} = true ]; then
  JOBS=$(nproc --all)
else
  JOBS=1
fi

if [ "x$DEBUILD" == "x1" ]; then
  debuild --preserve-envvar CMAKE_CUSTOM_OPTIONS -i -us -uc -b
else
  case "${CMAKE_CUSTOM_OPTIONS}" in
    *BUNDLE=ON*)
      mkdir build && cd build
      cmake .. ${CMAKE_CUSTOM_OPTIONS}
      make -j${JOBS} package
    ;;
    *)
      mkdir build && cd build
      cmake .. ${CMAKE_CUSTOM_OPTIONS}
      make -j${JOBS}
    ;;
  esac
fi
