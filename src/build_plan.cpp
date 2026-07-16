/**
 *
 *  @file build_plan.cpp
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
#include <cgride/engine/build_plan.hpp>

#include <utility>

namespace cgride::engine
{
  BuildPlan::BuildPlan(cgride::graph::Graph graph)
      : graph_(std::move(graph))
  {
  }

  BuildPlan::BuildPlan(
      cgride::graph::Graph graph,
      std::filesystem::path build_directory)
      : graph_(std::move(graph)),
        build_directory_(std::move(build_directory))
  {
  }

  BuildPlan BuildPlan::create(cgride::graph::Graph graph)
  {
    return BuildPlan(std::move(graph));
  }

  BuildPlan &BuildPlan::graph(cgride::graph::Graph graph)
  {
    graph_ = std::move(graph);
    return *this;
  }

  BuildPlan &BuildPlan::build_directory(std::filesystem::path path)
  {
    build_directory_ = std::move(path);
    return *this;
  }

  BuildPlan &BuildPlan::target(std::string name)
  {
    if (!name.empty())
    {
      targets_.push_back(std::move(name));
    }

    return *this;
  }

  BuildPlan &BuildPlan::targets(std::vector<std::string> names)
  {
    for (auto &name : names)
    {
      if (!name.empty())
      {
        targets_.push_back(std::move(name));
      }
    }

    return *this;
  }

  BuildPlan &BuildPlan::clear_targets() noexcept
  {
    targets_.clear();
    return *this;
  }

  const cgride::graph::Graph &BuildPlan::graph() const noexcept
  {
    return graph_;
  }

  cgride::graph::Graph &BuildPlan::graph() noexcept
  {
    return graph_;
  }

  const std::filesystem::path &BuildPlan::build_directory() const noexcept
  {
    return build_directory_;
  }

  const std::vector<std::string> &BuildPlan::targets() const noexcept
  {
    return targets_;
  }

  bool BuildPlan::has_targets() const noexcept
  {
    return !targets_.empty();
  }

  std::size_t BuildPlan::target_count() const noexcept
  {
    return targets_.size();
  }

  bool BuildPlan::valid() const noexcept
  {
    return !build_directory_.empty();
  }

} // namespace cgride::engine
