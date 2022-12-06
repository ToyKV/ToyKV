add_executable(example example.cpp)

target_include_directories(example PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(example PRIVATE ${DEPEND_LIST} ${LIBRARY_NAME})
target_compile_features(example PUBLIC cxx_std_17)
