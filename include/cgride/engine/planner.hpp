/**
 *
 *  @file planner.hpp
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
#ifndef CGRIDE_ENGINE_PLANNER_HPP
#define CGRIDE_ENGINE_PLANNER_HPP

#include <cgride/core/result.hpp>
#include <cgride/engine/build_plan.hpp>
#include <cgride/engine/build_request.hpp>

namespace cgride::engine
{
  /**
   * @class Planner
   * @brief Converts a build request into a build plan.
   *
   * Planner is the first orchestration layer of the engine. It validates the
   * request-level input and prepares a BuildPlan. Later revisions will expand
   * this class to generate compile, archive and link tasks from the project
   * model and toolchain.
   *
   * The planner does not execute tasks, access the cache, print diagnostics or
   * terminate the process.
   */
  class Planner
  {
  public:
    /**
     * @brief Construct a planner.
     */
    Planner() = default;

    /**
     * @brief Create a build plan from a request.
     *
     * @param request Build request.
     * @return Build plan or validation error.
     */
    [[nodiscard]] cgride::core::Result<BuildPlan> plan(
        const BuildRequest &request) const;
  };

  /**
   * @brief Convenience function for planning a build.
   *
   * @param request Build request.
   * @return Build plan or validation error.
   */
  [[nodiscard]] cgride::core::Result<BuildPlan> plan_build(
      const BuildRequest &request);

} // namespace cgride::engine

#endif // CGRIDE_ENGINE_PLANNER_HPP
