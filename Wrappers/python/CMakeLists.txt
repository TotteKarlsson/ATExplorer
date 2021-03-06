cmake_minimum_required(VERSION 2.8)
project(atexplorer)
find_package(SWIG REQUIRED)
include(UseSWIG)
find_package(PythonInterp)
find_package(PythonLibs)
get_filename_component(PYTHON_LIB_FOLDER ${PYTHON_LIBRARIES} DIRECTORY CACHE)
message("Python lib folder: "       ${PYTHON_LIB_FOLDER})
message("Python include folder: "   ${PYTHON_INCLUDE_DIRS})
message("Python libraries: "        ${PYTHON_LIBRARIES})
message("CMake Install folder: "    ${CMAKE_INSTALL_PREFIX})
message("ATExplorer Root: "         ${ATEXPLORER_ROOT})
message("Current folder: "          ${CMAKE_CURRENT_SOURCE_DIR})
set(PyModule "atexplorer")

include_directories(
    .
    ${BOOST_ROOT}
    ${BOOST_ROOT}/boost/tr1/tr1
    ${PYTHON_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ATEXPLORER_ROOT}
    ${ATEXPLORER_ROOT}/CPP/Source
    ${ATEXPLORER_ROOT}/CPP/Source/ATData
    ${TP_INCLUDES}
    ${TP_INCLUDES}/dsl
    ${TP_INCLUDES}/atexplorer
) 

link_directories(
${TP_LIBS}/lib
${CMAKE_BINARY_DIR}/lib
${PYTHON_LIB_FOLDER}
)

set(CMAKE_MODULE_LINKER_FLAGS "P:\\atExplorer\\Wrappers\\python\\${PyModule}.def")

set_source_files_properties(${PyModule}.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${PyModule}.i PROPERTIES SWIG_FLAGS "-threads")

SWIG_ADD_LIBRARY(${PyModule}
    TYPE MODULE 
    LANGUAGE python 
    SOURCES ${PyModule}.i
    )
    
TARGET_LINK_LIBRARIES (_${PyModule}
    atExplorerData
    atExplorerCore
    ${TP_LIBS}/dslFoundation.lib
    ${TP_LIBS}/dslPythonPlugins.lib
    ${TP_LIBS}/poco_foundation.lib
    ${PYTHON_LIB_FOLDER}/Python37_CG.lib
)

# INSTALL PYTHON BINDINGS
# Get the python site packages directory by invoking python
execute_process(COMMAND python -c "import site; print(site.getsitepackages()[0])" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)
message("PYTHON_SITE_PACKAGES = ${PYTHON_SITE_PACKAGES}")

install(
    TARGETS _${PyModule}
    DESTINATION ${PYTHON_SITE_PACKAGES})

install(
    FILES         ${PyModule}.i
    DESTINATION   include/atexplorer
)

install(
    FILES         ${CMAKE_CURRENT_BINARY_DIR}/${PyModule}.py 
                  ${CMAKE_BINARY_DIR}/lib/_${PyModule}.tds
    DESTINATION   ${PYTHON_SITE_PACKAGES}
)
