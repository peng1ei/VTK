cmake_minimum_required(VERSION 3.13)
project(vtk-study)

set(CMAKE_CXX_STANDARD 11)

# Find includes in corresponding build directories
# 自动添加CMAKE_CURRENT_BINARY_DIR和CMAKE_CURRENT_SOURCE_DIR
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 将输出重定向到bin子目录
# 根据编译的不同的平台，将最终结果输出到不同版本里
# cmake -DCMAKE_BUILD_TYPE=Debug/Release
if (CMAKE_BUILD_TYPE MATCHES "Debug"
        OR CMAKE_BUILD_TYPE MATCHES "None")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin/debug)
    #message(STATUS "CMAKE_BUILD_TYPE is Debug")
elseif (CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin/release)
    #message(STATUS "CMAKE_BUILD_TYPE is Release")
elseif (CMAKE_BUILD_TYPE MATCHES "RelWitchDebInfo")
    #message(STATUS "CMAKE_BUILD_TYPE is RelWitchDebInfo")
elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
    #message(STATUS "CMAKE_BUILD_TYPE is MinSizeRel")
else ()
    #message(STATUS "unknown CMAKE_BUILD_TYPE = " ${CMAKE_BUILD_TYPE})
endif ()

# 配置vtk
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#===========================================================================
# temp
#===========================================================================

set(TEMP_DIR ./temp)
include_directories(${TEMP_DIR})

# eg_rendering_cylinder
add_executable(temp_main ${TEMP_DIR}/main.cpp)
target_link_libraries(temp_main ${VTK_LIBRARIES})

add_executable(temp_vtk_02 ${TEMP_DIR}/Study_VTK_Together02.cpp)
target_link_libraries(temp_vtk_02 ${VTK_LIBRARIES})

#===========================================================================
# vtk_dev_book_cn
#===========================================================================

set(VTKDEVBOOKCN_DIR ./vtk_dev_book_cn)
include_directories(${VTKDEVBOOKCN_DIR})

# ch01
add_executable(bc_ch01 ${VTKDEVBOOKCN_DIR}/ch01/1.3_TestVTKInstall.cpp)
target_link_libraries(bc_ch01 ${VTK_LIBRARIES})

#===========================================================================
# examples
#===========================================================================

set(EXAMPLES_DIR ./examples)
include_directories(${EXAMPLES_DIR})

# eg_rendering_cylinder
add_executable(eg_rendering_cylinder ${EXAMPLES_DIR}/rendering/Cylinder.cpp)
target_link_libraries(eg_rendering_cylinder ${VTK_LIBRARIES})

#===========================================================================
# VTKExamples
#===========================================================================

set(VTK_EXAMPLES_DIR ./VTKExamples)
include_directories(${VTK_EXAMPLES_DIR})

# eg_rendering_cylinder
add_executable(ve_interaction_ImageRegion ${VTK_EXAMPLES_DIR}/Interaction/ImageRegion.cpp)
target_link_libraries(ve_interaction_ImageRegion ${VTK_LIBRARIES})

# eg_rendering_cylinder
add_executable(ve_interaction_WorldPointPicker ${VTK_EXAMPLES_DIR}/Interaction/WorldPointPicker.cpp)
target_link_libraries(ve_interaction_WorldPointPicker ${VTK_LIBRARIES})

add_executable(ve_interaction_MouseEvents ${VTK_EXAMPLES_DIR}/Interaction/MouseEvents.cpp)
target_link_libraries(ve_interaction_MouseEvents ${VTK_LIBRARIES})

#----------------------------------------------------------------------------------

# eg_rendering_cylinder
add_executable(ve_visualization_MoveActor ${VTK_EXAMPLES_DIR}/Visualization/MoveActor.cpp)
target_link_libraries(ve_visualization_MoveActor ${VTK_LIBRARIES})

add_executable(ve_visualization_MoveCamera ${VTK_EXAMPLES_DIR}/Visualization/MoveCamera.cpp)
target_link_libraries(ve_visualization_MoveCamera ${VTK_LIBRARIES})

#--------------------------------------------------------------------------------------

add_executable(ve_utilities_Coordinate ${VTK_EXAMPLES_DIR}/Utilities/Coordinate.cpp)
target_link_libraries(ve_utilities_Coordinate ${VTK_LIBRARIES})