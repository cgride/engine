/**
 *
 *  @file planner_test.cpp
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
#include <cgride/engine/planner.hpp>

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

} // namespace

int main()
{
  {
    cgride::engine::Planner planner;

    cgride::engine::BuildRequest request;

    auto result = planner.plan(request);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot plan build without a project.");
  }

  {
    cgride::engine::Planner planner;

    cgride::engine::BuildRequest request;
    request.project(make_project());

    auto result = planner.plan(request);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot plan build without a toolchain.");
  }

  {
    cgride::engine::Planner planner;

    cgride::engine::BuildOptions options;
    options.build_directory({});

    cgride::engine::BuildRequest request(
        make_project(),
        make_toolchain(),
        options);

    auto result = planner.plan(request);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot plan build with invalid build options.");
  }

  {
    cgride::engine::Planner planner;

    cgride::engine::BuildRequest request(
        make_project(),
        make_toolchain());

    auto result = planner.plan(request);

    assert(result);

    auto plan = std::move(result.value());

    assert(plan.valid());
    assert(plan.build_directory() == std::filesystem::path("build"));
    assert(!plan.has_targets());
    assert(plan.target_count() == 0);
  }

  {
    cgride::engine::Planner planner;

    cgride::engine::BuildOptions options;

    options
        .build_directory("build-release")
        .mode(cgride::engine::BuildMode::Release)
        .target("app")
        .jobs(4);

    cgride::engine::BuildRequest request(
        make_project(),
        make_toolchain(),
        options);

    auto result = planner.plan(request);

    assert(result);

    auto plan = std::move(result.value());

    assert(plan.valid());
    assert(plan.build_directory() == std::filesystem::path("build-release"));
    assert(plan.has_targets());
    assert(plan.target_count() == 1);
    assert(plan.targets()[0] == "app");
  }

  {
    cgride::engine::BuildOptions options;

    options
        .build_directory("out")
        .target("core");

    cgride::engine::BuildRequest request(
        make_project(),
        make_toolchain(),
        options);

    auto result = cgride::engine::plan_build(request);

    assert(result);

    auto plan = std::move(result.value());

    assert(plan.valid());
    assert(plan.build_directory() == std::filesystem::path("out"));
    assert(plan.has_targets());
    assert(plan.targets()[0] == "core");
  }

  {
    cgride::engine::Planner planner;

    cgride::engine::BuildRequest request(
        make_project(),
        make_toolchain(),
        cgride::engine::BuildOptions::debug());

    auto first = planner.plan(request);
    auto second = planner.plan(request);

    assert(first);
    assert(second);

    auto first_plan = std::move(first.value());
    auto second_plan = std::move(second.value());

    assert(first_plan.build_directory() == std::filesystem::path("build"));
    assert(second_plan.build_directory() == std::filesystem::path("build"));
  }

  return 0;
}
