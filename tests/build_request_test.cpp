/**
 *
 *  @file build_request_test.cpp
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
#include <utility>

#include <cgride/engine/build_request.hpp>

namespace
{
  [[nodiscard]] cgride::project::Project make_project()
  {
    return cgride::project::Project{};
  }

  [[nodiscard]] cgride::toolchains::Toolchain make_toolchain()
  {
    return cgride::toolchains::Toolchain{};
  }

} // namespace

int main()
{
  {
    cgride::engine::BuildRequest request;

    assert(!request.has_project());
    assert(!request.has_toolchain());
    assert(!request.project().has_value());
    assert(!request.toolchain().has_value());
    assert(request.options().valid());
    assert(!request.valid());
  }

  {
    cgride::engine::BuildRequest request(
        make_project(),
        make_toolchain());

    assert(request.has_project());
    assert(request.has_toolchain());
    assert(request.project().has_value());
    assert(request.toolchain().has_value());
    assert(request.options().valid());
    assert(request.options().build_directory() == std::filesystem::path("build"));
    assert(request.valid());
  }

  {
    cgride::engine::BuildOptions options;

    options
        .build_directory("build-release")
        .mode(cgride::engine::BuildMode::Release)
        .jobs(4)
        .target("app");

    cgride::engine::BuildRequest request(
        make_project(),
        make_toolchain(),
        options);

    assert(request.has_project());
    assert(request.has_toolchain());
    assert(request.valid());

    assert(request.options().build_directory() == std::filesystem::path("build-release"));
    assert(request.options().mode() == cgride::engine::BuildMode::Release);
    assert(request.options().is_release());
    assert(request.options().jobs() == 4);
    assert(request.options().has_target());
    assert(request.options().target().value() == "app");
  }

  {
    cgride::engine::BuildOptions options;

    options
        .build_directory("build-debug")
        .jobs(2)
        .target("core");

    auto request = cgride::engine::BuildRequest::create(
        make_project(),
        make_toolchain(),
        options);

    assert(request.has_project());
    assert(request.has_toolchain());
    assert(request.valid());

    assert(request.options().build_directory() == std::filesystem::path("build-debug"));
    assert(request.options().jobs() == 2);
    assert(request.options().has_target());
    assert(request.options().target().value() == "core");
  }

  {
    cgride::engine::BuildRequest request;

    request.project(make_project());

    assert(request.has_project());
    assert(!request.has_toolchain());
    assert(!request.valid());

    request.toolchain(make_toolchain());

    assert(request.has_toolchain());
    assert(request.valid());
  }

  {
    cgride::engine::BuildRequest request(
        make_project(),
        make_toolchain());

    assert(request.valid());

    request.clear_project();

    assert(!request.has_project());
    assert(!request.project().has_value());
    assert(request.has_toolchain());
    assert(!request.valid());

    request.project(make_project());

    assert(request.valid());

    request.clear_toolchain();

    assert(request.has_project());
    assert(!request.has_toolchain());
    assert(!request.toolchain().has_value());
    assert(!request.valid());
  }

  {
    cgride::engine::BuildRequest request(
        make_project(),
        make_toolchain());

    cgride::engine::BuildOptions options;

    options.build_directory({});

    request.options(options);

    assert(request.has_project());
    assert(request.has_toolchain());
    assert(!request.options().valid());
    assert(!request.valid());

    options.build_directory("build");

    request.options(options);

    assert(request.options().valid());
    assert(request.valid());
  }

  {
    cgride::engine::BuildRequest request;

    request
        .project(make_project())
        .toolchain(make_toolchain())
        .options(cgride::engine::BuildOptions::release());

    assert(request.has_project());
    assert(request.has_toolchain());
    assert(request.options().is_release());
    assert(request.valid());
  }

  return 0;
}
