/**
 *
 *  @file build_options.hpp
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
#ifndef CGRIDE_ENGINE_BUILD_OPTIONS_HPP
#define CGRIDE_ENGINE_BUILD_OPTIONS_HPP

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

namespace cgride::engine
{
  /**
   * @enum BuildMode
   * @brief High-level build mode requested by the caller.
   */
  enum class BuildMode
  {
    Debug,
    Release
  };

  /**
   * @brief Convert a build mode to a stable string.
   *
   * @param mode Build mode.
   * @return Stable string representation.
   */
  [[nodiscard]] std::string_view to_string(BuildMode mode) noexcept;

  /**
   * @class BuildOptions
   * @brief User-level build options consumed by the engine.
   *
   * BuildOptions describes how a build should be performed. It does not contain
   * a project, a toolchain, a graph or any executor state. Those are connected
   * later by BuildRequest, Planner and BuildEngine.
   */
  class BuildOptions
  {
  public:
    /**
     * @brief Construct default build options.
     */
    BuildOptions() = default;

    /**
     * @brief Create default debug build options.
     *
     * @return Build options.
     */
    [[nodiscard]] static BuildOptions defaults();

    /**
     * @brief Create debug build options.
     *
     * @return Build options.
     */
    [[nodiscard]] static BuildOptions debug();

    /**
     * @brief Create release build options.
     *
     * @return Build options.
     */
    [[nodiscard]] static BuildOptions release();

    /**
     * @brief Set the build directory.
     *
     * @param path Build directory.
     * @return Reference to these options.
     */
    BuildOptions &build_directory(std::filesystem::path path);

    /**
     * @brief Set the selected target name.
     *
     * @param name Target name.
     * @return Reference to these options.
     */
    BuildOptions &target(std::string name);

    /**
     * @brief Clear the selected target name.
     *
     * @return Reference to these options.
     */
    BuildOptions &clear_target() noexcept;

    /**
     * @brief Set the build mode.
     *
     * @param mode Build mode.
     * @return Reference to these options.
     */
    BuildOptions &mode(BuildMode mode) noexcept;

    /**
     * @brief Set the number of parallel jobs.
     *
     * Values smaller than one are normalized to one.
     *
     * @param value Job count.
     * @return Reference to these options.
     */
    BuildOptions &jobs(std::size_t value) noexcept;

    /**
     * @brief Enable or disable rebuild mode.
     *
     * @param value True to ignore existing build state.
     * @return Reference to these options.
     */
    BuildOptions &rebuild(bool value) noexcept;

    /**
     * @brief Enable or disable cache usage.
     *
     * @param value True to use the cache.
     * @return Reference to these options.
     */
    BuildOptions &use_cache(bool value) noexcept;

    /**
     * @brief Enable or disable output verification on cache hits.
     *
     * @param value True to verify cached outputs.
     * @return Reference to these options.
     */
    BuildOptions &verify_cache_outputs(bool value) noexcept;

    /**
     * @brief Enable or disable hashing of input file contents.
     *
     * @param value True to hash input contents.
     * @return Reference to these options.
     */
    BuildOptions &hash_cache_inputs(bool value) noexcept;

    /**
     * @brief Enable or disable dry-run mode.
     *
     * @param value True to plan without executing commands.
     * @return Reference to these options.
     */
    BuildOptions &dry_run(bool value) noexcept;

    /**
     * @brief Enable or disable verbose diagnostics.
     *
     * @param value True to emit more diagnostics.
     * @return Reference to these options.
     */
    BuildOptions &verbose(bool value) noexcept;

    /**
     * @brief Access the build directory.
     */
    [[nodiscard]] const std::filesystem::path &build_directory() const noexcept;

    /**
     * @brief Access the selected target name.
     */
    [[nodiscard]] const std::optional<std::string> &target() const noexcept;

    /**
     * @brief Return true when a target name is selected.
     */
    [[nodiscard]] bool has_target() const noexcept;

    /**
     * @brief Access the build mode.
     */
    [[nodiscard]] BuildMode mode() const noexcept;

    /**
     * @brief Return true when release mode is selected.
     */
    [[nodiscard]] bool is_release() const noexcept;

    /**
     * @brief Access the number of parallel jobs.
     */
    [[nodiscard]] std::size_t jobs() const noexcept;

    /**
     * @brief Return true when rebuild mode is enabled.
     */
    [[nodiscard]] bool rebuild() const noexcept;

    /**
     * @brief Return true when cache usage is enabled.
     */
    [[nodiscard]] bool use_cache() const noexcept;

    /**
     * @brief Return true when cached outputs should be verified.
     */
    [[nodiscard]] bool verify_cache_outputs() const noexcept;

    /**
     * @brief Return true when input file contents should be hashed.
     */
    [[nodiscard]] bool hash_cache_inputs() const noexcept;

    /**
     * @brief Return true when dry-run mode is enabled.
     */
    [[nodiscard]] bool dry_run() const noexcept;

    /**
     * @brief Return true when verbose diagnostics are enabled.
     */
    [[nodiscard]] bool verbose() const noexcept;

    /**
     * @brief Return true when options are usable.
     */
    [[nodiscard]] bool valid() const noexcept;

  private:
    std::filesystem::path build_directory_{"build"};
    std::optional<std::string> target_{};
    BuildMode mode_{BuildMode::Debug};
    std::size_t jobs_{1};
    bool rebuild_{false};
    bool use_cache_{true};
    bool verify_cache_outputs_{true};
    bool hash_cache_inputs_{false};
    bool dry_run_{false};
    bool verbose_{false};
  };

} // namespace cgride::engine

#endif // CGRIDE_ENGINE_BUILD_OPTIONS_HPP
