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

    BuildResult result = BuildResult::succeeded(
        elapsed_since(started_at));

    result.task_results(std::move(executed.value()));

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
