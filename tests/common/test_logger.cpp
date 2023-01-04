#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "toykv/common/logger.h"

TEST_CASE("test LOG_INFO") {
  LOG_INFO("this is a info, {}", "hello");
}

TEST_CASE("test LOG_WARN") {
  LOG_WARN("this is a warn, {}", "hello");
}

TEST_CASE("test LOG_DEBUG") {
  LOG_DEBUG("this is a debug, {}", "hello");
}

TEST_CASE("test LOG_ERROR") {
  LOG_ERROR("this is a error, {}", "hello");
}
