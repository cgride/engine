/**
 *
 *  @file build_engine_test.cpp
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
#include <filesystem>

#include <cgride/core/error.hpp>
#include <cgride/engine/build_engine.hpp>
#include <cgride/graph/graph.hpp>

namespace
{
  [[nodiscard]] cgride::project::Project make_project()
  {
    return cgride::project::Project{};
  }

  [[nodiscard]] cgride::toolchains::Toolchain make_toolchain()
  {
    return cgride::toolchains::Toolchain{};
  }

  [[nodiscard]] cgride::engine::BuildRequest make_request()
  {
    return cgride::engine::BuildRequest(
        make_project(),
        make_toolchain());
  }

} // namespace

int main()
{
  {
    cgride::engine::BuildEngine engine;

    const auto &planner = engine.planner();

    static_cast<void>(planner);
  }

  {
    cgride::engine::BuildEngine engine;

    engine.planner(cgride::engine::Planner{});

    auto planned = engine.plan(make_request());

    assert(planned);

    auto plan = std::move(planned.value());

    assert(plan.valid());
    assert(plan.build_directory() == std::filesystem::path("build"));
  }

  {
    cgride::engine::BuildEngine engine;

    cgride::engine::BuildRequest request;

    auto planned = engine.plan(request);

    assert(!planned);
    assert(planned.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(planned.error().message() == "Cannot plan build without a project.");
  }

  {
    cgride::engine::BuildEngine engine;

    cgride::engine::BuildOptions options;

    options
        .build_directory("build-debug")
        .target("app")
        .jobs(2);

    cgride::engine::BuildRequest request(
        make_project(),
        make_toolchain(),
        options);

    auto planned = engine.plan(request);

    assert(planned);

    auto plan = std::move(planned.value());

    assert(plan.valid());
    assert(plan.build_directory() == std::filesystem::path("build-debug"));
    assert(plan.has_targets());
    assert(plan.targets()[0] == "app");
  }

  {
    cgride::engine::BuildEngine engine;

    cgride::engine::BuildPlan plan;
    plan.build_directory({});

    auto result = engine.execute(
        plan,
        cgride::engine::BuildOptions::defaults());

    assert(result.failed());
    assert(result.finished());
    assert(result.has_error());
    assert(result.error().value().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().value().message() == "Cannot execute an invalid build plan.");
  }

  {
    cgride::engine::BuildEngine engine;

    cgride::engine::BuildPlan plan(
        cgride::graph::Graph{});

    cgride::engine::BuildOptions options;
    options.build_directory({});

    auto result = engine.execute(
        plan,
        options);

    assert(result.failed());
    assert(result.finished());
    assert(result.has_error());
    assert(result.error().value().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().value().message() == "Cannot execute build plan with invalid build options.");
  }

  {
    cgride::engine::BuildEngine engine;

    cgride::engine::BuildPlan plan(
        cgride::graph::Graph{});

    auto result = engine.execute(
        plan,
        cgride::engine::BuildOptions::defaults());

    assert(result.failed());
    assert(result.finished());
    assert(result.has_error());
    assert(result.task_results().empty());
  }

  {
    cgride::engine::BuildEngine engine;

    cgride::engine::BuildRequest request;

    auto result = engine.build(request);

    assert(result.failed());
    assert(result.finished());
    assert(result.has_error());
    assert(result.error().value().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().value().message() == "Cannot plan build without a project.");
  }

  {
    cgride::engine::BuildEngine engine;

    auto result = engine.build(make_request());

    /*
     * The first planner implementation creates an empty graph. The executor
     * rejects that graph, so the build fails cleanly instead of pretending a
     * real build happened.
     */
    assert(result.failed());
    assert(result.finished());
    assert(result.has_error());
    assert(result.task_results().empty());
  }

  {
    auto result = cgride::engine::build(make_request());

    assert(result.failed());
    assert(result.finished());
    assert(result.has_error());
  }

  {
    cgride::engine::BuildEngine engine;

    cgride::engine::BuildOptions options;

    options
        .dry_run(true)
        .jobs(3);

    cgride::engine::BuildRequest request(
        make_project(),
        make_toolchain(),
        options);

    auto result = engine.build(request);

    assert(result.failed());
    assert(result.finished());
    assert(result.has_error());
  }

  return 0;
}
