function(cgride_engine_add_test name source)
  add_executable(${name}
    ${source}
  )

  target_link_libraries(${name}
    PRIVATE
      cgride::engine
  )

  target_compile_features(${name}
    PRIVATE
      cxx_std_23
  )

  set_target_properties(${name}
    PROPERTIES
      CXX_EXTENSIONS OFF
  )

  add_test(
    NAME ${name}
    COMMAND ${name}
  )
endfunction()

cgride_engine_add_test(cgride_engine_build_options_test build_options_test.cpp)
cgride_engine_add_test(cgride_engine_build_request_test build_request_test.cpp)
cgride_engine_add_test(cgride_engine_build_result_test build_result_test.cpp)
cgride_engine_add_test(cgride_engine_build_plan_test build_plan_test.cpp)
cgride_engine_add_test(cgride_engine_planner_test planner_test.cpp)
cgride_engine_add_test(cgride_engine_build_engine_test build_engine_test.cpp)
