/**
 *
 *  @file planner.cpp
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
#include <cgride/engine/planner.hpp>
#include <utility>
#include <cgride/core/error.hpp>
#include <cgride/graph/graph.hpp>

namespace cgride::engine
{
  namespace
  {
    using cgride::core::Error;
    using cgride::core::ErrorCode;

    [[nodiscard]] cgride::core::Result<void> validate_request(
        const BuildRequest &request)
    {
      if (!request.has_project())
      {
        return Error(
            ErrorCode::InvalidArgument,
            "Cannot plan build without a project.");
      }

      if (!request.has_toolchain())
      {
        return Error(
            ErrorCode::InvalidArgument,
            "Cannot plan build without a toolchain.");
      }

      if (!request.options().valid())
      {
        return Error(
            ErrorCode::InvalidArgument,
            "Cannot plan build with invalid build options.");
      }

      return cgride::core::Result<void>::ok();
    }

  } // namespace

  cgride::core::Result<BuildPlan> Planner::plan(
      const BuildRequest &request) const
  {
    auto validated = validate_request(request);

    if (!validated)
    {
      return validated.error();
    }

    cgride::graph::Graph graph;

    BuildPlan plan(
        std::move(graph),
        request.options().build_directory());

    if (request.options().has_target())
    {
      plan.target(request.options().target().value());
    }

    return plan;
  }

  cgride::core::Result<BuildPlan> plan_build(
      const BuildRequest &request)
  {
    Planner planner;

    return planner.plan(request);
  }

} // namespace cgride::engine
