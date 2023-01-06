#!bin/bash

LINT_DIR="toykv"
find . -name \*.h -or -name \*.cpp | grep -vE "^./build/" | xargs -n12 -P8 \
cpplint --quiet --counting=detailed --linelength=120 \
  --filter=-legal/copyright,-build/header_guard,-runtime/references,-build/c++11
  # --recursive ${LINT_DIR}/
