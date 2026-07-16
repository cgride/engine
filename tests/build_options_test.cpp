/**
 *
 *  @file build_options_test.cpp
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
#include <string_view>

#include <cgride/engine/build_options.hpp>

int main()
{
  {
    assert(cgride::engine::to_string(cgride::engine::BuildMode::Debug) == std::string_view("debug"));
    assert(cgride::engine::to_string(cgride::engine::BuildMode::Release) == std::string_view("release"));
  }

  {
    cgride::engine::BuildOptions options;

    assert(options.build_directory() == std::filesystem::path("build"));
    assert(!options.has_target());
    assert(!options.target().has_value());
    assert(options.mode() == cgride::engine::BuildMode::Debug);
    assert(!options.is_release());
    assert(options.jobs() == 1);
    assert(!options.rebuild());
    assert(options.use_cache());
    assert(options.verify_cache_outputs());
    assert(!options.hash_cache_inputs());
    assert(!options.dry_run());
    assert(!options.verbose());
    assert(options.valid());
  }

  {
    auto options = cgride::engine::BuildOptions::defaults();

    assert(options.build_directory() == std::filesystem::path("build"));
    assert(options.mode() == cgride::engine::BuildMode::Debug);
    assert(options.jobs() == 1);
    assert(options.use_cache());
    assert(options.valid());
  }

  {
    auto options = cgride::engine::BuildOptions::debug();

    assert(options.mode() == cgride::engine::BuildMode::Debug);
    assert(!options.is_release());
    assert(options.valid());
  }

  {
    auto options = cgride::engine::BuildOptions::release();

    assert(options.mode() == cgride::engine::BuildMode::Release);
    assert(options.is_release());
    assert(options.valid());
  }

  {
    cgride::engine::BuildOptions options;

    options
        .build_directory("build-debug")
        .target("app")
        .mode(cgride::engine::BuildMode::Release)
        .jobs(8)
        .rebuild(true)
        .use_cache(false)
        .verify_cache_outputs(false)
        .hash_cache_inputs(true)
        .dry_run(true)
        .verbose(true);

    assert(options.build_directory() == std::filesystem::path("build-debug"));
    assert(options.has_target());
    assert(options.target().has_value());
    assert(options.target().value() == "app");
    assert(options.mode() == cgride::engine::BuildMode::Release);
    assert(options.is_release());
    assert(options.jobs() == 8);
    assert(options.rebuild());
    assert(!options.use_cache());
    assert(!options.verify_cache_outputs());
    assert(options.hash_cache_inputs());
    assert(options.dry_run());
    assert(options.verbose());
    assert(options.valid());
  }

  {
    cgride::engine::BuildOptions options;

    options.target("core");

    assert(options.has_target());
    assert(options.target().value() == "core");

    options.clear_target();

    assert(!options.has_target());
    assert(!options.target().has_value());
  }

  {
    cgride::engine::BuildOptions options;

    options.target("core");

    assert(options.has_target());

    options.target("");

    assert(!options.has_target());
    assert(!options.target().has_value());
  }

  {
    cgride::engine::BuildOptions options;

    options.jobs(0);

    assert(options.jobs() == 1);
    assert(options.valid());

    options.jobs(4);

    assert(options.jobs() == 4);
    assert(options.valid());
  }

  {
    cgride::engine::BuildOptions options;

    options.build_directory({});

    assert(options.build_directory().empty());
    assert(!options.valid());

    options.build_directory("build");

    assert(options.valid());
  }

  {
    cgride::engine::BuildOptions options;

    options
        .mode(cgride::engine::BuildMode::Debug)
        .rebuild(false)
        .use_cache(true)
        .verify_cache_outputs(true)
        .hash_cache_inputs(false)
        .dry_run(false)
        .verbose(false);

    assert(options.mode() == cgride::engine::BuildMode::Debug);
    assert(!options.rebuild());
    assert(options.use_cache());
    assert(options.verify_cache_outputs());
    assert(!options.hash_cache_inputs());
    assert(!options.dry_run());
    assert(!options.verbose());
    assert(options.valid());
  }

  return 0;
}
