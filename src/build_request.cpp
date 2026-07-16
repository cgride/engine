/**
 *
 *  @file build_request.cpp
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
#include <cgride/engine/build_request.hpp>

#include <utility>

namespace cgride::engine
{
  BuildRequest::BuildRequest(
      cgride::project::Project project,
      cgride::toolchains::Toolchain toolchain)
      : project_(std::move(project)),
        toolchain_(std::move(toolchain))
  {
  }

  BuildRequest::BuildRequest(
      cgride::project::Project project,
      cgride::toolchains::Toolchain toolchain,
      BuildOptions options)
      : project_(std::move(project)),
        toolchain_(std::move(toolchain)),
        options_(std::move(options))
  {
  }

  BuildRequest BuildRequest::create(
      cgride::project::Project project,
      cgride::toolchains::Toolchain toolchain,
      BuildOptions options)
  {
    return BuildRequest(
        std::move(project),
        std::move(toolchain),
        std::move(options));
  }

  BuildRequest &BuildRequest::project(cgride::project::Project project)
  {
    project_ = std::move(project);
    return *this;
  }

  BuildRequest &BuildRequest::toolchain(cgride::toolchains::Toolchain toolchain)
  {
    toolchain_ = std::move(toolchain);
    return *this;
  }

  BuildRequest &BuildRequest::options(BuildOptions options)
  {
    options_ = std::move(options);
    return *this;
  }

  BuildRequest &BuildRequest::clear_project() noexcept
  {
    project_.reset();
    return *this;
  }

  BuildRequest &BuildRequest::clear_toolchain() noexcept
  {
    toolchain_.reset();
    return *this;
  }

  const std::optional<cgride::project::Project> &BuildRequest::project() const noexcept
  {
    return project_;
  }

  const std::optional<cgride::toolchains::Toolchain> &BuildRequest::toolchain() const noexcept
  {
    return toolchain_;
  }

  const BuildOptions &BuildRequest::options() const noexcept
  {
    return options_;
  }

  bool BuildRequest::has_project() const noexcept
  {
    return project_.has_value();
  }

  bool BuildRequest::has_toolchain() const noexcept
  {
    return toolchain_.has_value();
  }

  bool BuildRequest::valid() const noexcept
  {
    return has_project() &&
           has_toolchain() &&
           options_.valid();
  }

} // namespace cgride::engine
