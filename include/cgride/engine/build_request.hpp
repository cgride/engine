/**
 *
 *  @file build_request.hpp
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
#ifndef CGRIDE_ENGINE_BUILD_REQUEST_HPP
#define CGRIDE_ENGINE_BUILD_REQUEST_HPP

#include <optional>
#include <utility>

#include <cgride/engine/build_options.hpp>
#include <cgride/project/project.hpp>
#include <cgride/toolchains/toolchain.hpp>

namespace cgride::engine
{
  /**
   * @class BuildRequest
   * @brief Complete input required to prepare a build.
   *
   * BuildRequest connects a project model, a toolchain description and user
   * build options. It is still only an input object. It does not create a build
   * graph, execute tasks, access the cache, or print diagnostics.
   */
  class BuildRequest
  {
  public:
    /**
     * @brief Construct an empty build request.
     */
    BuildRequest() = default;

    /**
     * @brief Construct a build request with a project and toolchain.
     *
     * @param project Project model.
     * @param toolchain Toolchain description.
     */
    BuildRequest(
        cgride::project::Project project,
        cgride::toolchains::Toolchain toolchain);

    /**
     * @brief Construct a build request with a project, toolchain and options.
     *
     * @param project Project model.
     * @param toolchain Toolchain description.
     * @param options Build options.
     */
    BuildRequest(
        cgride::project::Project project,
        cgride::toolchains::Toolchain toolchain,
        BuildOptions options);

    /**
     * @brief Create a build request.
     *
     * @param project Project model.
     * @param toolchain Toolchain description.
     * @param options Build options.
     * @return Build request.
     */
    [[nodiscard]] static BuildRequest create(
        cgride::project::Project project,
        cgride::toolchains::Toolchain toolchain,
        BuildOptions options = BuildOptions::defaults());

    /**
     * @brief Set the project model.
     *
     * @param project Project model.
     * @return Reference to this request.
     */
    BuildRequest &project(cgride::project::Project project);

    /**
     * @brief Set the toolchain description.
     *
     * @param toolchain Toolchain description.
     * @return Reference to this request.
     */
    BuildRequest &toolchain(cgride::toolchains::Toolchain toolchain);

    /**
     * @brief Set build options.
     *
     * @param options Build options.
     * @return Reference to this request.
     */
    BuildRequest &options(BuildOptions options);

    /**
     * @brief Clear the project model.
     *
     * @return Reference to this request.
     */
    BuildRequest &clear_project() noexcept;

    /**
     * @brief Clear the toolchain description.
     *
     * @return Reference to this request.
     */
    BuildRequest &clear_toolchain() noexcept;

    /**
     * @brief Access the optional project model.
     */
    [[nodiscard]] const std::optional<cgride::project::Project> &project() const noexcept;

    /**
     * @brief Access the optional toolchain description.
     */
    [[nodiscard]] const std::optional<cgride::toolchains::Toolchain> &toolchain() const noexcept;

    /**
     * @brief Access build options.
     */
    [[nodiscard]] const BuildOptions &options() const noexcept;

    /**
     * @brief Return true when a project model is present.
     */
    [[nodiscard]] bool has_project() const noexcept;

    /**
     * @brief Return true when a toolchain description is present.
     */
    [[nodiscard]] bool has_toolchain() const noexcept;

    /**
     * @brief Return true when the request has all required input.
     */
    [[nodiscard]] bool valid() const noexcept;

  private:
    std::optional<cgride::project::Project> project_{};
    std::optional<cgride::toolchains::Toolchain> toolchain_{};
    BuildOptions options_{};
  };

} // namespace cgride::engine

#endif // CGRIDE_ENGINE_BUILD_REQUEST_HPP
