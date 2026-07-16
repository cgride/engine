/**
 *
 *  @file build_options.cpp
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
#include <cgride/engine/build_options.hpp>

#include <utility>

namespace cgride::engine
{
  std::string_view to_string(BuildMode mode) noexcept
  {
    switch (mode)
    {
    case BuildMode::Debug:
      return "debug";

    case BuildMode::Release:
      return "release";
    }

    return "debug";
  }

  BuildOptions BuildOptions::defaults()
  {
    return BuildOptions{};
  }

  BuildOptions BuildOptions::debug()
  {
    BuildOptions options;

    options.mode(BuildMode::Debug);

    return options;
  }

  BuildOptions BuildOptions::release()
  {
    BuildOptions options;

    options.mode(BuildMode::Release);

    return options;
  }

  BuildOptions &BuildOptions::build_directory(std::filesystem::path path)
  {
    build_directory_ = std::move(path);
    return *this;
  }

  BuildOptions &BuildOptions::target(std::string name)
  {
    if (name.empty())
    {
      target_.reset();
    }
    else
    {
      target_ = std::move(name);
    }

    return *this;
  }

  BuildOptions &BuildOptions::clear_target() noexcept
  {
    target_.reset();
    return *this;
  }

  BuildOptions &BuildOptions::mode(BuildMode mode) noexcept
  {
    mode_ = mode;
    return *this;
  }

  BuildOptions &BuildOptions::jobs(std::size_t value) noexcept
  {
    jobs_ = value == 0 ? 1 : value;
    return *this;
  }

  BuildOptions &BuildOptions::rebuild(bool value) noexcept
  {
    rebuild_ = value;
    return *this;
  }

  BuildOptions &BuildOptions::use_cache(bool value) noexcept
  {
    use_cache_ = value;
    return *this;
  }

  BuildOptions &BuildOptions::verify_cache_outputs(bool value) noexcept
  {
    verify_cache_outputs_ = value;
    return *this;
  }

  BuildOptions &BuildOptions::hash_cache_inputs(bool value) noexcept
  {
    hash_cache_inputs_ = value;
    return *this;
  }

  BuildOptions &BuildOptions::dry_run(bool value) noexcept
  {
    dry_run_ = value;
    return *this;
  }

  BuildOptions &BuildOptions::verbose(bool value) noexcept
  {
    verbose_ = value;
    return *this;
  }

  BuildOptions &BuildOptions::on_event(EventHandler handler)
  {
    event_handler_ = std::move(handler);
    return *this;
  }

  BuildOptions &BuildOptions::clear_event_handler() noexcept
  {
    event_handler_ = {};
    return *this;
  }

  const BuildOptions::EventHandler &BuildOptions::event_handler() const noexcept
  {
    return event_handler_;
  }

  bool BuildOptions::has_event_handler() const noexcept
  {
    return static_cast<bool>(event_handler_);
  }

  const std::filesystem::path &BuildOptions::build_directory() const noexcept
  {
    return build_directory_;
  }

  const std::optional<std::string> &BuildOptions::target() const noexcept
  {
    return target_;
  }

  bool BuildOptions::has_target() const noexcept
  {
    return target_.has_value() && !target_.value().empty();
  }

  BuildMode BuildOptions::mode() const noexcept
  {
    return mode_;
  }

  bool BuildOptions::is_release() const noexcept
  {
    return mode_ == BuildMode::Release;
  }

  std::size_t BuildOptions::jobs() const noexcept
  {
    return jobs_;
  }

  bool BuildOptions::rebuild() const noexcept
  {
    return rebuild_;
  }

  bool BuildOptions::use_cache() const noexcept
  {
    return use_cache_;
  }

  bool BuildOptions::verify_cache_outputs() const noexcept
  {
    return verify_cache_outputs_;
  }

  bool BuildOptions::hash_cache_inputs() const noexcept
  {
    return hash_cache_inputs_;
  }

  bool BuildOptions::dry_run() const noexcept
  {
    return dry_run_;
  }

  bool BuildOptions::verbose() const noexcept
  {
    return verbose_;
  }

  bool BuildOptions::valid() const noexcept
  {
    return !build_directory_.empty() && jobs_ > 0;
  }

} // namespace cgride::engine
