cmake_minimum_required (VERSION 2.6)
project (cglab1)

find_package(GLUT)
find_package(OpenGL)
find_package(glew)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

# glut
add_executable(glut glut/main.cpp)
target_link_libraries(glut ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )


# gasket1
add_executable(gasket1 gasket1/gasket.cpp)
target_link_libraries(gasket1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )


# gasket2
add_executable(gasket2 gasket2/gasket2.cpp)
target_link_libraries(gasket2 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )


# gasket3
add_executable(gasket3 gasket3/gasket3.cpp)
target_link_libraries(gasket3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

# gasket1-shader
add_executable(gasket1s gasket1-shader/example1.cpp gasket1-shader/InitShader.cpp 
               gasket1-shader/Angel.h gasket1-shader/mat.h gasket1-shader/vec.h gasket1-shader/CheckError.h)
target_link_libraries(gasket1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )