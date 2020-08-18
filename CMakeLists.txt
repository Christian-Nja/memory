cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(memory VERSION 1.0)

# option to make memory_db library optional
option(USE_MEMORY_DB "Use library to interface with db" ON)

# header configuration file to pass version
configure_file(memoryConfig.h.in memoryConfig.h)

# add the memory_db library (building subdirectories)
add_subdirectory(memory_db)
list(APPEND EXTRA_LIBS memory_db)

# add the executable
add_executable(memory memory.c)

# linking subdirectories
target_link_libraries(memory PUBLIC ${EXTRA_LIBS})

# add the binary tree to the list of paths to search for include files
# to have access to memoryConfig.h
# add also path for library headers
target_include_directories(memory PUBLIC
			   "${PROJECT_BINARY_DIR}"
			  )


### install rules

# change default installation directory
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "/home/linux/opt/memory" CACHE PATH "..." FORCE)
endif()

install(TARGETS memory DESTINATION /usr/bin)
install(FILES "${PROJECT_BINARY_DIR}/memoryConfig.h"
	DESTINATION include
	)
				
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${memory_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${memory_VERSION_MINOR}")
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Christian Colonna") 
# application destination
set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/memory") 
include(CPack)
