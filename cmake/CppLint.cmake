file(
  GLOB_RECURSE
  LINT_FILES
  "${PROJECT_SOURCE_DIR}/toykv/*.h"
  "${PROJECT_SOURCE_DIR}/toykv/*.cpp"
  "${PROJECT_SOURCE_DIR}/example/*.h"
  "${PROJECT_SOURCE_DIR}/example/*.cpp")

# cpplint
find_program(
  CPPLINT_BIN
  NAMES cpplint cpplint.py
  HINTS "${PROJECT_SOURCE_DIR}/cmake")

if("${CPPLINT_BIN}" STREQUAL "CPPLINT_BIN-NOTFOUND")
  message(WARNING "ToyKV/main couldn't find cpplint.")
else()
  message(STATUS "ToyKV/main found cpplint at ${CPPLINT_BIN}")
endif()

# Balancing act: cpplint.py takes a non-trivial time to launch, so process 12
# files per invocation, while still ensuring parallelism
add_custom_target(
  check-lint
  echo
  '${LINT_FILES}'
  |
  xargs
  -n12
  -P8
  ${CPPLINT_BIN}
  --verbose=2
  --quiet
  --linelength=120
  --filter=-legal/copyright,-build/header_guard,-runtime/references # https://github.com/cpplint/cpplint/issues/148
)
