/**
 *
 *  @file build_result.hpp
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
#ifndef CGRIDE_ENGINE_BUILD_RESULT_HPP
#define CGRIDE_ENGINE_BUILD_RESULT_HPP

#include <chrono>
#include <optional>
#include <string_view>
#include <vector>

#include <cgride/core/diagnostic.hpp>
#include <cgride/core/error.hpp>
#include <cgride/executor/task_result.hpp>

namespace cgride::engine
{
  /**
   * @enum BuildStatus
   * @brief Final state of an engine build operation.
   */
  enum class BuildStatus
  {
    NotStarted,
    Succeeded,
    Failed,
    Cancelled
  };

  /**
   * @brief Convert a build status to a stable string.
   *
   * @param status Build status.
   * @return Stable string representation.
   */
  [[nodiscard]] std::string_view to_string(BuildStatus status) noexcept;

  /**
   * @class BuildResult
   * @brief Result returned by the build engine.
   *
   * BuildResult records the high-level build status, task execution results,
   * diagnostics, optional error information and total duration. It does not
   * print anything and it does not terminate the process.
   */
  class BuildResult
  {
  public:
    /**
     * @brief Duration type used by build results.
     */
    using Duration = std::chrono::milliseconds;

    /**
     * @brief Construct an empty build result.
     */
    BuildResult() = default;

    /**
     * @brief Create a successful build result.
     *
     * @param duration Build duration.
     * @return Build result.
     */
    [[nodiscard]] static BuildResult succeeded(Duration duration = Duration{0});

    /**
     * @brief Create a failed build result.
     *
     * @param error Build error.
     * @param duration Build duration.
     * @return Build result.
     */
    [[nodiscard]] static BuildResult failed(
        cgride::core::Error error,
        Duration duration = Duration{0});

    /**
     * @brief Create a cancelled build result.
     *
     * @param error Cancellation error.
     * @param duration Build duration.
     * @return Build result.
     */
    [[nodiscard]] static BuildResult cancelled(
        cgride::core::Error error,
        Duration duration = Duration{0});

    /**
     * @brief Set the build status.
     *
     * @param status Build status.
     * @return Reference to this result.
     */
    BuildResult &status(BuildStatus status) noexcept;

    /**
     * @brief Set the build duration.
     *
     * @param duration Build duration.
     * @return Reference to this result.
     */
    BuildResult &duration(Duration duration) noexcept;

    /**
     * @brief Set the build error.
     *
     * @param error Build error.
     * @return Reference to this result.
     */
    BuildResult &error(cgride::core::Error error);

    /**
     * @brief Clear the build error.
     *
     * @return Reference to this result.
     */
    BuildResult &clear_error() noexcept;

    /**
     * @brief Add one task result.
     *
     * @param result Task result.
     * @return Reference to this result.
     */
    BuildResult &task_result(cgride::executor::TaskResult result);

    /**
     * @brief Add many task results.
     *
     * @param results Task results.
     * @return Reference to this result.
     */
    BuildResult &task_results(std::vector<cgride::executor::TaskResult> results);

    /**
     * @brief Add one diagnostic.
     *
     * @param diagnostic Diagnostic.
     * @return Reference to this result.
     */
    BuildResult &diagnostic(cgride::core::Diagnostic diagnostic);

    /**
     * @brief Add many diagnostics.
     *
     * @param diagnostics Diagnostics.
     * @return Reference to this result.
     */
    BuildResult &diagnostics(std::vector<cgride::core::Diagnostic> diagnostics);

    /**
     * @brief Access the build status.
     */
    [[nodiscard]] BuildStatus status() const noexcept;

    /**
     * @brief Access the build duration.
     */
    [[nodiscard]] Duration duration() const noexcept;

    /**
     * @brief Access the optional build error.
     */
    [[nodiscard]] const std::optional<cgride::core::Error> &error() const noexcept;

    /**
     * @brief Return true when the result has an error.
     */
    [[nodiscard]] bool has_error() const noexcept;

    /**
     * @brief Access task results.
     */
    [[nodiscard]] const std::vector<cgride::executor::TaskResult> &task_results() const noexcept;

    /**
     * @brief Access diagnostics.
     */
    [[nodiscard]] const std::vector<cgride::core::Diagnostic> &diagnostics() const noexcept;

    /**
     * @brief Return true when the build succeeded.
     */
    [[nodiscard]] bool success() const noexcept;

    /**
     * @brief Return true when the build failed.
     */
    [[nodiscard]] bool failed() const noexcept;

    /**
     * @brief Return true when the build was cancelled.
     */
    [[nodiscard]] bool cancelled() const noexcept;

    /**
     * @brief Return true when the build reached a final state.
     */
    [[nodiscard]] bool finished() const noexcept;

  private:
    BuildStatus status_{BuildStatus::NotStarted};
    Duration duration_{0};
    std::optional<cgride::core::Error> error_{};
    std::vector<cgride::executor::TaskResult> task_results_{};
    std::vector<cgride::core::Diagnostic> diagnostics_{};
  };

} // namespace cgride::engine

#endif // CGRIDE_ENGINE_BUILD_RESULT_HPP
