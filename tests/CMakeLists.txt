
set(MPIEXEC_COMMAND${MPIEXEC_EXECUTABLE})

macro(add_gtensor_test name)
  add_executable(${name})
  target_cxx_sources(${name} PRIVATE ${name}.cxx)
  target_link_libraries(${name} gtensor GTest::Main)
  if (USE_GTEST_DISCOVER_TESTS)
    gtest_discover_tests(${name} EXEC_WRAPPER ${MPIEXEC_COMMAND})
  else()
    gtest_add_tests(TARGET ${name} EXEC_WRAPPER ${MPIEXEC_COMMAND})
  endif()
endmacro()

if (GTENSOR_CUDA)
  add_gtensor_test(test_thrust_ext)
endif()

add_gtensor_test(test_expression)
add_gtensor_test(test_helper)
add_gtensor_test(test_gtensor)
add_gtensor_test(test_gtensor_view)
add_gtensor_test(test_view)
add_gtensor_test(test_wip)
