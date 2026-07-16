/**
 *
 *  @file build_plan_test.cpp
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
#include <string>
#include <vector>

#include <cgride/engine/build_plan.hpp>
#include <cgride/graph/graph.hpp>

int main()
{
  {
    cgride::engine::BuildPlan plan;

    assert(plan.build_directory() == std::filesystem::path("build"));
    assert(plan.targets().empty());
    assert(!plan.has_targets());
    assert(plan.target_count() == 0);
    assert(plan.valid());
  }

  {
    cgride::engine::BuildPlan plan(
        cgride::graph::Graph{});

    assert(plan.build_directory() == std::filesystem::path("build"));
    assert(plan.targets().empty());
    assert(!plan.has_targets());
    assert(plan.target_count() == 0);
    assert(plan.valid());
  }

  {
    cgride::engine::BuildPlan plan(
        cgride::graph::Graph{},
        std::filesystem::path("build-debug"));

    assert(plan.build_directory() == std::filesystem::path("build-debug"));
    assert(plan.valid());
  }

  {
    auto plan = cgride::engine::BuildPlan::create(
        cgride::graph::Graph{});

    assert(plan.build_directory() == std::filesystem::path("build"));
    assert(plan.valid());
  }

  {
    cgride::engine::BuildPlan plan;

    plan.build_directory("build-release");

    assert(plan.build_directory() == std::filesystem::path("build-release"));
    assert(plan.valid());

    plan.build_directory({});

    assert(plan.build_directory().empty());
    assert(!plan.valid());
  }

  {
    cgride::engine::BuildPlan plan;

    plan.target("app");

    assert(plan.has_targets());
    assert(plan.target_count() == 1);
    assert(plan.targets().size() == 1);
    assert(plan.targets()[0] == "app");
  }

  {
    cgride::engine::BuildPlan plan;

    plan
        .target("")
        .target("core")
        .target("engine");

    assert(plan.has_targets());
    assert(plan.target_count() == 2);
    assert(plan.targets()[0] == "core");
    assert(plan.targets()[1] == "engine");
  }

  {
    cgride::engine::BuildPlan plan;

    plan.targets({
        "core",
        "",
        "engine",
        "cli",
    });

    assert(plan.has_targets());
    assert(plan.target_count() == 3);
    assert(plan.targets()[0] == "core");
    assert(plan.targets()[1] == "engine");
    assert(plan.targets()[2] == "cli");
  }

  {
    cgride::engine::BuildPlan plan;

    plan.targets({
        "core",
        "engine",
    });

    assert(plan.has_targets());
    assert(plan.target_count() == 2);

    plan.clear_targets();

    assert(!plan.has_targets());
    assert(plan.target_count() == 0);
    assert(plan.targets().empty());
  }

  {
    cgride::engine::BuildPlan plan;

    plan.graph(cgride::graph::Graph{});

    assert(plan.valid());
  }

  {
    cgride::engine::BuildPlan plan;

    const auto &const_graph = plan.graph();
    auto &mutable_graph = plan.graph();

    static_cast<void>(const_graph);
    static_cast<void>(mutable_graph);

    assert(plan.valid());
  }

  {
    cgride::engine::BuildPlan plan;

    plan
        .graph(cgride::graph::Graph{})
        .build_directory("out")
        .targets({
            "app",
            "tests",
        });

    assert(plan.build_directory() == std::filesystem::path("out"));
    assert(plan.has_targets());
    assert(plan.target_count() == 2);
    assert(plan.targets()[0] == "app");
    assert(plan.targets()[1] == "tests");
    assert(plan.valid());
  }

  return 0;
}
