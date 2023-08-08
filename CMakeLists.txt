cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(hep_apps)

set(CMAKE_OSX_ARCHITECTURES "arm64")
SET(CMAKE_OSX_DEPLOYMENT_TARGET 13.2)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

find_package(ROOT REQUIRED COMPONENTS Minuit Eve Geom)
include(${ROOT_USE_FILE})

SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 
SET(CMAKE_INSTALL_RPATH "/cvmfs/sft.cern.ch/lcg/releases/vdt/0.4.3-992df/x86_64-centos7-gcc8-opt/lib:/cvmfs/sft.cern.ch/lcg/releases/tbb/2019_U7-ba3eb/x86_64-centos7-gcc8-opt/lib:/cvmfs/sft.cern.ch/lcg/releases/Davix/0.7.3-d94fa/x86_64-centos7-gcc8-opt/lib64/:/cvmfs/sft.cern.ch/lcg/releases/png/1.6.37-9c2fe/x86_64-centos7-gcc8-opt/lib/:/cvmfs/sft.cern.ch/lcg/releases/GSL/2.5-32fc5/x86_64-centos7-gcc8-opt/lib/")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


set(LIBRARIES_DIR libs)
set(LIBRARIES 
    core
    extensions
    histogramming
)
foreach(LIBRARY ${LIBRARIES})
    add_subdirectory("${LIBRARIES_DIR}/${LIBRARY}")
    include_directories("${LIBRARIES_DIR}/${LIBRARY}/include")
endforeach(LIBRARY)

add_executable(core_test apps/core_test.cpp)
add_executable(extensions_test apps/extensions_test.cpp)
add_executable(top_trig_efficiency apps/top_trig_efficiency.cpp)
add_executable(histogramming_test apps/histogramming_test.cpp)

target_link_libraries(core_test core ${ROOT_LIBRARIES})
target_link_libraries(extensions_test core extensions ${ROOT_LIBRARIES})
target_link_libraries(histogramming_test core extensions histogramming ${ROOT_LIBRARIES})
target_link_libraries(top_trig_efficiency core extensions ${ROOT_LIBRARIES})

set_property(TARGET core_test PROPERTY CXX_STANDARD 17)
set_property(TARGET extensions_test PROPERTY CXX_STANDARD 17)
set_property(TARGET top_trig_efficiency PROPERTY CXX_STANDARD 17)
set_property(TARGET histogramming PROPERTY CXX_STANDARD 17)
