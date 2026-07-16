/**
 *
 *  @file build_plan.hpp
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
#ifndef CGRIDE_ENGINE_BUILD_PLAN_HPP
#define CGRIDE_ENGINE_BUILD_PLAN_HPP

#include <filesystem>
#include <string>
#include <vector>

#include <cgride/graph/graph.hpp>

namespace cgride::engine
{
  /**
   * @class BuildPlan
   * @brief Prepared build graph and engine metadata.
   *
   * BuildPlan is the result of planning. It contains the graph that the
   * executor can run, the selected target names, and the build directory used
   * by generated outputs.
   *
   * The plan does not execute tasks, access the cache, discover compilers, or
   * print diagnostics.
   */
  class BuildPlan
  {
  public:
    /**
     * @brief Construct an empty build plan.
     */
    BuildPlan() = default;

    /**
     * @brief Construct a build plan from a graph.
     *
     * @param graph Build graph.
     */
    explicit BuildPlan(cgride::graph::Graph graph);

    /**
     * @brief Construct a build plan from a graph and build directory.
     *
     * @param graph Build graph.
     * @param build_directory Build directory.
     */
    BuildPlan(
        cgride::graph::Graph graph,
        std::filesystem::path build_directory);

    /**
     * @brief Create a build plan from a graph.
     *
     * @param graph Build graph.
     * @return Build plan.
     */
    [[nodiscard]] static BuildPlan create(cgride::graph::Graph graph);

    /**
     * @brief Set the build graph.
     *
     * @param graph Build graph.
     * @return Reference to this plan.
     */
    BuildPlan &graph(cgride::graph::Graph graph);

    /**
     * @brief Set the build directory.
     *
     * @param path Build directory.
     * @return Reference to this plan.
     */
    BuildPlan &build_directory(std::filesystem::path path);

    /**
     * @brief Add a selected target name.
     *
     * @param name Target name.
     * @return Reference to this plan.
     */
    BuildPlan &target(std::string name);

    /**
     * @brief Add many selected target names.
     *
     * @param names Target names.
     * @return Reference to this plan.
     */
    BuildPlan &targets(std::vector<std::string> names);

    /**
     * @brief Clear selected target names.
     *
     * @return Reference to this plan.
     */
    BuildPlan &clear_targets() noexcept;

    /**
     * @brief Access the build graph.
     */
    [[nodiscard]] const cgride::graph::Graph &graph() const noexcept;

    /**
     * @brief Access the mutable build graph.
     */
    [[nodiscard]] cgride::graph::Graph &graph() noexcept;

    /**
     * @brief Access the build directory.
     */
    [[nodiscard]] const std::filesystem::path &build_directory() const noexcept;

    /**
     * @brief Access selected target names.
     */
    [[nodiscard]] const std::vector<std::string> &targets() const noexcept;

    /**
     * @brief Return true when at least one target is selected.
     */
    [[nodiscard]] bool has_targets() const noexcept;

    /**
     * @brief Return the number of selected targets.
     */
    [[nodiscard]] std::size_t target_count() const noexcept;

    /**
     * @brief Return true when the plan has a usable build directory.
     */
    [[nodiscard]] bool valid() const noexcept;

  private:
    cgride::graph::Graph graph_{};
    std::filesystem::path build_directory_{"build"};
    std::vector<std::string> targets_{};
  };

} // namespace cgride::engine

#endif // CGRIDE_ENGINE_BUILD_PLAN_HPP
