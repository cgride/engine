/**
 *
 *  @file build_engine.hpp
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
#ifndef CGRIDE_ENGINE_BUILD_ENGINE_HPP
#define CGRIDE_ENGINE_BUILD_ENGINE_HPP

#include <cgride/core/result.hpp>
#include <cgride/engine/build_plan.hpp>
#include <cgride/engine/build_request.hpp>
#include <cgride/engine/build_result.hpp>
#include <cgride/engine/planner.hpp>

namespace cgride::engine
{
  /**
   * @class BuildEngine
   * @brief High-level build orchestration layer.
   *
   * BuildEngine connects planning and execution. It validates a BuildRequest
   * through the Planner, obtains a BuildPlan, then sends the plan graph to the
   * executor.
   *
   * The engine does not print to the terminal and it does not terminate the
   * process. All failures are returned through BuildResult or Result.
   */
  class BuildEngine
  {
  public:
    /**
     * @brief Construct a build engine with the default planner.
     */
    BuildEngine() = default;

    /**
     * @brief Construct a build engine with an explicit planner.
     *
     * @param planner Planner instance.
     */
    explicit BuildEngine(Planner planner);

    /**
     * @brief Access the planner.
     */
    [[nodiscard]] const Planner &planner() const noexcept;

    /**
     * @brief Set the planner.
     *
     * @param planner Planner instance.
     * @return Reference to this engine.
     */
    BuildEngine &planner(Planner planner);

    /**
     * @brief Prepare a build plan from a request.
     *
     * @param request Build request.
     * @return Build plan or validation error.
     */
    [[nodiscard]] cgride::core::Result<BuildPlan> plan(
        const BuildRequest &request) const;

    /**
     * @brief Execute an already prepared build plan.
     *
     * @param plan Build plan.
     * @param options Build options.
     * @return Build result.
     */
    [[nodiscard]] BuildResult execute(
        const BuildPlan &plan,
        const BuildOptions &options) const;

    /**
     * @brief Plan and execute a build request.
     *
     * @param request Build request.
     * @return Build result.
     */
    [[nodiscard]] BuildResult build(
        const BuildRequest &request) const;

  private:
    Planner planner_{};
  };

  /**
   * @brief Convenience function for building a request.
   *
   * @param request Build request.
   * @return Build result.
   */
  [[nodiscard]] BuildResult build(
      const BuildRequest &request);

} // namespace cgride::engine

#endif // CGRIDE_ENGINE_BUILD_ENGINE_HPP
