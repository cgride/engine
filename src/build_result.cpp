/**
 *
 *  @file build_result.cpp
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
#include <cgride/engine/build_result.hpp>

#include <utility>

namespace cgride::engine
{
  std::string_view to_string(BuildStatus status) noexcept
  {
    switch (status)
    {
    case BuildStatus::NotStarted:
      return "not_started";

    case BuildStatus::Succeeded:
      return "succeeded";

    case BuildStatus::Failed:
      return "failed";

    case BuildStatus::Cancelled:
      return "cancelled";
    }

    return "not_started";
  }

  BuildResult BuildResult::succeeded(Duration duration)
  {
    BuildResult result;

    result
        .status(BuildStatus::Succeeded)
        .duration(duration);

    return result;
  }

  BuildResult BuildResult::failed(
      cgride::core::Error error,
      Duration duration)
  {
    BuildResult result;

    result
        .status(BuildStatus::Failed)
        .duration(duration)
        .error(std::move(error));

    return result;
  }

  BuildResult BuildResult::cancelled(
      cgride::core::Error error,
      Duration duration)
  {
    BuildResult result;

    result
        .status(BuildStatus::Cancelled)
        .duration(duration)
        .error(std::move(error));

    return result;
  }

  BuildResult &BuildResult::status(BuildStatus status) noexcept
  {
    status_ = status;
    return *this;
  }

  BuildResult &BuildResult::duration(Duration duration) noexcept
  {
    duration_ = duration;
    return *this;
  }

  BuildResult &BuildResult::error(cgride::core::Error error)
  {
    error_ = std::move(error);
    return *this;
  }

  BuildResult &BuildResult::clear_error() noexcept
  {
    error_.reset();
    return *this;
  }

  BuildResult &BuildResult::task_result(cgride::executor::TaskResult result)
  {
    task_results_.push_back(std::move(result));
    return *this;
  }

  BuildResult &BuildResult::task_results(
      std::vector<cgride::executor::TaskResult> results)
  {
    for (auto &result : results)
    {
      task_results_.push_back(std::move(result));
    }

    return *this;
  }

  BuildResult &BuildResult::diagnostic(cgride::core::Diagnostic diagnostic)
  {
    diagnostics_.push_back(std::move(diagnostic));
    return *this;
  }

  BuildResult &BuildResult::diagnostics(
      std::vector<cgride::core::Diagnostic> diagnostics)
  {
    for (auto &diagnostic : diagnostics)
    {
      diagnostics_.push_back(std::move(diagnostic));
    }

    return *this;
  }

  BuildStatus BuildResult::status() const noexcept
  {
    return status_;
  }

  BuildResult::Duration BuildResult::duration() const noexcept
  {
    return duration_;
  }

  const std::optional<cgride::core::Error> &BuildResult::error() const noexcept
  {
    return error_;
  }

  bool BuildResult::has_error() const noexcept
  {
    return error_.has_value();
  }

  const std::vector<cgride::executor::TaskResult> &BuildResult::task_results() const noexcept
  {
    return task_results_;
  }

  const std::vector<cgride::core::Diagnostic> &BuildResult::diagnostics() const noexcept
  {
    return diagnostics_;
  }

  bool BuildResult::success() const noexcept
  {
    return status_ == BuildStatus::Succeeded;
  }

  bool BuildResult::failed() const noexcept
  {
    return status_ == BuildStatus::Failed;
  }

  bool BuildResult::cancelled() const noexcept
  {
    return status_ == BuildStatus::Cancelled;
  }

  bool BuildResult::finished() const noexcept
  {
    return status_ == BuildStatus::Succeeded ||
           status_ == BuildStatus::Failed ||
           status_ == BuildStatus::Cancelled;
  }

} // namespace cgride::engine
