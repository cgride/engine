/**
 *
 *  @file build_engine.cpp
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
#include <cgride/engine/build_engine.hpp>

#include <chrono>
#include <string>
#include <utility>

#include <cgride/core/error.hpp>
#include <cgride/executor/execution_options.hpp>
#include <cgride/executor/executor.hpp>

namespace cgride::engine
{
  namespace
  {
    using cgride::core::Error;
    using cgride::core::ErrorCode;

    using Clock = std::chrono::steady_clock;

    [[nodiscard]] BuildResult::Duration elapsed_since(
        Clock::time_point start) noexcept
    {
      return std::chrono::duration_cast<BuildResult::Duration>(
          Clock::now() - start);
    }

    [[nodiscard]] cgride::executor::ExecutionOptions make_execution_options(
        const BuildOptions &options)
    {
      cgride::executor::ExecutionOptions execution_options;

      execution_options
          .jobs(options.jobs())
          .dry_run(options.dry_run())
          .capture_output(true)
          .stop_on_failure(true);

      return execution_options;
    }

  } // namespace

  BuildEngine::BuildEngine(Planner planner)
      : planner_(std::move(planner))
  {
  }

  const Planner &BuildEngine::planner() const noexcept
  {
    return planner_;
  }

  BuildEngine &BuildEngine::planner(Planner planner)
  {
    planner_ = std::move(planner);
    return *this;
  }

  cgride::core::Result<BuildPlan> BuildEngine::plan(
      const BuildRequest &request) const
  {
    return planner_.plan(request);
  }

  BuildResult BuildEngine::execute(
      const BuildPlan &plan,
      const BuildOptions &options) const
  {
    const auto started_at = Clock::now();

    if (!plan.valid())
    {
      return BuildResult::failed(
          Error(
              ErrorCode::InvalidArgument,
              "Cannot execute an invalid build plan."),
          elapsed_since(started_at));
    }

    if (!options.valid())
    {
      return BuildResult::failed(
          Error(
              ErrorCode::InvalidArgument,
              "Cannot execute build plan with invalid build options."),
          elapsed_since(started_at));
    }

    cgride::executor::Executor executor;

    auto executed = executor.execute(
        plan.graph(),
        make_execution_options(options));

    if (!executed)
    {
      return BuildResult::failed(
          executed.error(),
          elapsed_since(started_at));
    }

    auto task_results = std::move(executed.value());

    for (const auto &task_result : task_results)
    {
      if (!task_result.failed())
      {
        continue;
      }

      if (task_result.process_result().has_value() &&
          task_result.process_result().value().error().has_value())
      {
        const auto &process = task_result.process_result().value();
        const auto &process_error = process.error().value();
        auto detail = process.standard_error().empty()
                          ? process.standard_output()
                          : process.standard_error();

        if (detail.empty())
        {
          const auto task_name = task_result.task_name().empty()
                                     ? std::string(task_result.task_id().value())
                                     : task_result.task_name();
          const auto exit_detail = process_error.detail().value_or(std::string{});
          detail = exit_detail.empty() ? task_name : task_name + " (" + exit_detail + ")";
        }

        auto result = BuildResult::failed(
            Error(
                process_error.code(),
                process_error.message(),
                detail),
            elapsed_since(started_at));
        result.task_results(std::move(task_results));
        return result;
      }

      if (task_result.error().has_value())
      {
        auto result = BuildResult::failed(
            task_result.error().value(),
            elapsed_since(started_at));
        result.task_results(std::move(task_results));
        return result;
      }

      auto result = BuildResult::failed(
          Error(
              ErrorCode::ProcessFailed,
              "Build task failed.",
              task_result.message().empty() ? std::string(task_result.task_id().value()) : task_result.message()),
          elapsed_since(started_at));
      result.task_results(std::move(task_results));
      return result;
    }

    BuildResult result = BuildResult::succeeded(
        elapsed_since(started_at));

    result.task_results(std::move(task_results));

    return result;
  }

  BuildResult BuildEngine::build(
      const BuildRequest &request) const
  {
    const auto started_at = Clock::now();

    auto planned = planner_.plan(request);

    if (!planned)
    {
      return BuildResult::failed(
          planned.error(),
          elapsed_since(started_at));
    }

    auto plan = std::move(planned.value());

    auto result = execute(
        plan,
        request.options());

    result.duration(elapsed_since(started_at));

    return result;
  }

  BuildResult build(
      const BuildRequest &request)
  {
    BuildEngine engine;

    return engine.build(request);
  }

} // namespace cgride::engine
