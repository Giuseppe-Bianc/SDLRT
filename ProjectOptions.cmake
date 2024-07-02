include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(SDLRT_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(SDLRT_setup_options)
  option(SDLRT_ENABLE_HARDENING "Enable hardening" ON)
  option(SDLRT_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    SDLRT_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    SDLRT_ENABLE_HARDENING
    OFF)

  SDLRT_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR SDLRT_PACKAGING_MAINTAINER_MODE)
    option(SDLRT_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(SDLRT_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(SDLRT_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(SDLRT_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(SDLRT_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(SDLRT_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(SDLRT_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(SDLRT_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(SDLRT_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(SDLRT_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(SDLRT_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(SDLRT_ENABLE_PCH "Enable precompiled headers" OFF)
    option(SDLRT_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(SDLRT_ENABLE_IPO "Enable IPO/LTO" ON)
    option(SDLRT_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(SDLRT_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(SDLRT_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(SDLRT_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(SDLRT_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(SDLRT_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(SDLRT_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(SDLRT_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(SDLRT_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(SDLRT_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(SDLRT_ENABLE_PCH "Enable precompiled headers" OFF)
    option(SDLRT_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      SDLRT_ENABLE_IPO
      SDLRT_WARNINGS_AS_ERRORS
      SDLRT_ENABLE_USER_LINKER
      SDLRT_ENABLE_SANITIZER_ADDRESS
      SDLRT_ENABLE_SANITIZER_LEAK
      SDLRT_ENABLE_SANITIZER_UNDEFINED
      SDLRT_ENABLE_SANITIZER_THREAD
      SDLRT_ENABLE_SANITIZER_MEMORY
      SDLRT_ENABLE_UNITY_BUILD
      SDLRT_ENABLE_CLANG_TIDY
      SDLRT_ENABLE_CPPCHECK
      SDLRT_ENABLE_COVERAGE
      SDLRT_ENABLE_PCH
      SDLRT_ENABLE_CACHE)
  endif()

  SDLRT_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (SDLRT_ENABLE_SANITIZER_ADDRESS OR SDLRT_ENABLE_SANITIZER_THREAD OR SDLRT_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(SDLRT_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(SDLRT_global_options)
  if(SDLRT_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    SDLRT_enable_ipo()
  endif()

  SDLRT_supports_sanitizers()

  if(SDLRT_ENABLE_HARDENING AND SDLRT_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR SDLRT_ENABLE_SANITIZER_UNDEFINED
       OR SDLRT_ENABLE_SANITIZER_ADDRESS
       OR SDLRT_ENABLE_SANITIZER_THREAD
       OR SDLRT_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${SDLRT_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${SDLRT_ENABLE_SANITIZER_UNDEFINED}")
    SDLRT_enable_hardening(SDLRT_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(SDLRT_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(SDLRT_warnings INTERFACE)
  add_library(SDLRT_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  SDLRT_set_project_warnings(
    SDLRT_warnings
    ${SDLRT_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(SDLRT_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    SDLRT_configure_linker(SDLRT_options)
  endif()

  include(cmake/Sanitizers.cmake)
  SDLRT_enable_sanitizers(
    SDLRT_options
    ${SDLRT_ENABLE_SANITIZER_ADDRESS}
    ${SDLRT_ENABLE_SANITIZER_LEAK}
    ${SDLRT_ENABLE_SANITIZER_UNDEFINED}
    ${SDLRT_ENABLE_SANITIZER_THREAD}
    ${SDLRT_ENABLE_SANITIZER_MEMORY})

  set_target_properties(SDLRT_options PROPERTIES UNITY_BUILD ${SDLRT_ENABLE_UNITY_BUILD})

  if(SDLRT_ENABLE_PCH)
    target_precompile_headers(
      SDLRT_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(SDLRT_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    SDLRT_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(SDLRT_ENABLE_CLANG_TIDY)
    SDLRT_enable_clang_tidy(SDLRT_options ${SDLRT_WARNINGS_AS_ERRORS})
  endif()

  if(SDLRT_ENABLE_CPPCHECK)
    SDLRT_enable_cppcheck(${SDLRT_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(SDLRT_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    SDLRT_enable_coverage(SDLRT_options)
  endif()

  if(SDLRT_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(SDLRT_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(SDLRT_ENABLE_HARDENING AND NOT SDLRT_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR SDLRT_ENABLE_SANITIZER_UNDEFINED
       OR SDLRT_ENABLE_SANITIZER_ADDRESS
       OR SDLRT_ENABLE_SANITIZER_THREAD
       OR SDLRT_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    SDLRT_enable_hardening(SDLRT_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
