##############################################################################
# CMake
##############################################################################

cmake_minimum_required(VERSION 2.8.0)
project(udp_test_1)

##############################################################################
# Catkin
##############################################################################

# qt_build provides the qt cmake glue, roscpp the comms for a default talker
find_package(catkin REQUIRED COMPONENTS qt_build 
    roscpp
    std_msgs
    cv_bridge
    rospy
    message_runtime
    message_generation
    image_transport
    roslib
)
find_package(Qt5Network REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
include_directories(${catkin_INCLUDE_DIRS})
# Use this to define what the package will export (e.g. libs, headers).
# Since the default here is to produce only a binary, we don't worry about
# exporting anything.
catkin_package(
    INCLUDE_DIRS include LIBRARIES robot_udp
    CATKIN_DEPENDS
    image_transport
    roslib
    roscpp
    std_msgs
    rospy
    DEPENDS
)
set (OpenCV_INClUDE_DIRS /usr/local/include /usr/local/include/opencv2)
set(OpenCV_LIB_DIR /usr/local/lib)
set(Opencv_LIBS opencv_core opencv_highgui opencv_imgcodecs opencv_videoio opencv_imgproc opencv_features2d opencv_flann opencv_calib3d)

##############################################################################
# Qt Environment
##############################################################################

# this comes from qt_build's qt-ros.cmake which is automatically 
# included via the dependency call in package.xml
rosbuild_prepare_qt4(QtCore QtGui QtNetwork QtMultimedia) # Add the appropriate components to the component list here

##############################################################################
# Sections
##############################################################################

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resources/*.qrc)
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/udp_test_1/*.hpp)

QT4_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

##############################################################################
# Sources
##############################################################################

file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)

##############################################################################
# Binaries
##############################################################################

add_executable(udp_test_1 ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(udp_test_1 ${QT_LIBRARIES} ${catkin_LIBRARIES} ${Opencv_LIBS})
target_link_libraries(udp_test_1 ${QT_LIBRARIES} ${catkin_LIBRARIES})
install(TARGETS udp_test_1 RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

