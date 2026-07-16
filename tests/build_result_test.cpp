/**
 *
 *  @file build_result_test.cpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2026, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/cgride/engine
 *
 *  Use of this source code is governed by an MIT license
 *  that can be found in the LICENSE file.
 *
 *  Cgride
 *
 */
#include <cassert>
#include <chrono>
#include <string_view>
#include <utility>
#include <vector>

#include <cgride/core/diagnostic.hpp>
#include <cgride/core/error.hpp>
#include <cgride/engine/build_result.hpp>
#include <cgride/executor/task_result.hpp>

int main()
{
  {
    assert(cgride::engine::to_string(cgride::engine::BuildStatus::NotStarted) == std::string_view("not_started"));
    assert(cgride::engine::to_string(cgride::engine::BuildStatus::Succeeded) == std::string_view("succeeded"));
    assert(cgride::engine::to_string(cgride::engine::BuildStatus::Failed) == std::string_view("failed"));
    assert(cgride::engine::to_string(cgride::engine::BuildStatus::Cancelled) == std::string_view("cancelled"));
  }

  {
    cgride::engine::BuildResult result;

    assert(result.status() == cgride::engine::BuildStatus::NotStarted);
    assert(result.duration() == cgride::engine::BuildResult::Duration{0});
    assert(!result.has_error());
    assert(!result.error().has_value());
    assert(result.task_results().empty());
    assert(result.diagnostics().empty());
    assert(!result.success());
    assert(!result.failed());
    assert(!result.cancelled());
    assert(!result.finished());
  }

  {
    auto result = cgride::engine::BuildResult::succeeded(
        std::chrono::milliseconds{42});

    assert(result.status() == cgride::engine::BuildStatus::Succeeded);
    assert(result.duration() == std::chrono::milliseconds{42});
    assert(!result.has_error());
    assert(result.success());
    assert(!result.failed());
    assert(!result.cancelled());
    assert(result.finished());
  }

  {
    cgride::core::Error error(
        cgride::core::ErrorCode::InvalidArgument,
        "Invalid build request.");

    auto result = cgride::engine::BuildResult::failed(
        error,
        std::chrono::milliseconds{15});

    assert(result.status() == cgride::engine::BuildStatus::Failed);
    assert(result.duration() == std::chrono::milliseconds{15});
    assert(result.has_error());
    assert(result.error().value().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().value().message() == "Invalid build request.");
    assert(!result.success());
    assert(result.failed());
    assert(!result.cancelled());
    assert(result.finished());
  }

  {
    cgride::core::Error error(
        cgride::core::ErrorCode::Cancelled,
        "Build was cancelled.");

    auto result = cgride::engine::BuildResult::cancelled(
        error,
        std::chrono::milliseconds{8});

    assert(result.status() == cgride::engine::BuildStatus::Cancelled);
    assert(result.duration() == std::chrono::milliseconds{8});
    assert(result.has_error());
    assert(result.error().value().code() == cgride::core::ErrorCode::Cancelled);
    assert(result.error().value().message() == "Build was cancelled.");
    assert(!result.success());
    assert(!result.failed());
    assert(result.cancelled());
    assert(result.finished());
  }

  {
    cgride::engine::BuildResult result;

    result
        .status(cgride::engine::BuildStatus::Succeeded)
        .duration(std::chrono::milliseconds{100});

    assert(result.status() == cgride::engine::BuildStatus::Succeeded);
    assert(result.duration() == std::chrono::milliseconds{100});
    assert(result.success());
    assert(result.finished());
  }

  {
    cgride::engine::BuildResult result;

    result.error(cgride::core::Error(
        cgride::core::ErrorCode::InternalError,
        "Internal engine error."));

    assert(result.has_error());
    assert(result.error().value().code() == cgride::core::ErrorCode::InternalError);
    assert(result.error().value().message() == "Internal engine error.");

    result.clear_error();

    assert(!result.has_error());
    assert(!result.error().has_value());
  }

  {
    cgride::engine::BuildResult result;

    cgride::executor::TaskResult task_result;

    result.task_result(std::move(task_result));

    assert(result.task_results().size() == 1);
  }

  {
    cgride::engine::BuildResult result;

    std::vector<cgride::executor::TaskResult> task_results;
    task_results.emplace_back();
    task_results.emplace_back();

    result.task_results(std::move(task_results));

    assert(result.task_results().size() == 2);
  }

  {
    cgride::engine::BuildResult result;

    cgride::core::Diagnostic diagnostic;

    result.diagnostic(std::move(diagnostic));

    assert(result.diagnostics().size() == 1);
  }

  {
    cgride::engine::BuildResult result;

    std::vector<cgride::core::Diagnostic> diagnostics;
    diagnostics.emplace_back();
    diagnostics.emplace_back();

    result.diagnostics(std::move(diagnostics));

    assert(result.diagnostics().size() == 2);
  }

  {
    cgride::engine::BuildResult result;

    result.status(cgride::engine::BuildStatus::Failed);

    assert(!result.success());
    assert(result.failed());
    assert(!result.cancelled());
    assert(result.finished());

    result.status(cgride::engine::BuildStatus::Cancelled);

    assert(!result.success());
    assert(!result.failed());
    assert(result.cancelled());
    assert(result.finished());

    result.status(cgride::engine::BuildStatus::NotStarted);

    assert(!result.success());
    assert(!result.failed());
    assert(!result.cancelled());
    assert(!result.finished());
  }

  return 0;
}
