cmake_minimum_required(VERSION 3.16.0)
project (Primordial VERSION 0.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS off)
#set (CMAKE_GENERATOR_PLATFORM x64)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

option(PRIMORDIAL_ALLOW_INTRINSICS "Compiler intrinsics may be used" off)
option(PRIMORIAL_ALLOW_UINT128 "Compiler __UINT128 type may be used" off)
option (BUILD_TESTS "Build unit tests" off)

if(CT_SPECIALIZE_NUMERIC_LIMITS)
	add_compile_definitions(-D PRIMORDIAL_ALLOW_INTRINSICS -D PRIMORIAL_ALLOW_UINT128)
endif()

add_library(primordial)
target_compile_features(primordial PUBLIC cxx_std_20)
target_sources(primordial
               PRIVATE
			   src/primordial.cpp
			   src/nq_io.cpp
			   src/unit_system_io.cpp
			   src/supersub.cpp)

target_include_directories (primordial 
                            PUBLIC 
							$<INSTALL_INTERFACE:include>
							$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>							
							PRIVATE
							$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)
							
set_property(TARGET 
             primordial
			 PROPERTY 
			 PUBLIC_HEADER
			 include/ct/details/utility.hpp
			 include/ct/quot.hpp
			 include/ct/prime.hpp
			 include/ct/dims.hpp
			 include/ct/nq.hpp
			 include/ct/unit.hpp
			 include/ct/concepts.hpp
)

if (${BUILD_TESTS})
  enable_testing()
  add_subdirectory(test)
endif()
