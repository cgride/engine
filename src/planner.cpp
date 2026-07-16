/**
 *
 *  @file planner.cpp
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
#include <cgride/engine/planner.hpp>

#include <algorithm>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <cgride/core/error.hpp>
#include <cgride/core/platform.hpp>
#include <cgride/graph/graph.hpp>
#include <cgride/project/requirement.hpp>
#include <cgride/project/source_set.hpp>
#include <cgride/project/target.hpp>
#include <cgride/project/target_kind.hpp>
#include <cgride/toolchains/archive_command.hpp>
#include <cgride/toolchains/artifact_kind.hpp>
#include <cgride/toolchains/compile_command.hpp>
#include <cgride/toolchains/link_command.hpp>

namespace cgride::engine
{
  namespace
  {
    using cgride::core::Error;
    using cgride::core::ErrorCode;

    struct PlannedTarget
    {
      std::vector<cgride::graph::TaskId> compile_tasks{};
      std::vector<std::filesystem::path> objects{};
      std::filesystem::path artifact{};
      std::optional<cgride::graph::TaskId> artifact_task{};
    };

    [[nodiscard]] cgride::core::Result<void> validate_request(
        const BuildRequest &request)
    {
      if (!request.has_project())
      {
        return Error(
            ErrorCode::InvalidArgument,
            "Cannot plan build without a project.");
      }

      if (!request.has_toolchain())
      {
        return Error(
            ErrorCode::InvalidArgument,
            "Cannot plan build without a toolchain.");
      }

      if (!request.options().valid())
      {
        return Error(
            ErrorCode::InvalidArgument,
            "Cannot plan build with invalid build options.");
      }

      return cgride::core::Result<void>::ok();
    }

    [[nodiscard]] std::string sanitize_path_for_id(std::filesystem::path path)
    {
      auto value = path.generic_string();

      std::replace_if(
          value.begin(),
          value.end(),
          [](char character) {
            return character == '/' || character == '\\' || character == ':' || character == ' ';
          },
          '_');

      if (value.empty())
      {
        return "source";
      }

      return value;
    }

    [[nodiscard]] std::filesystem::path object_path_for(
        const BuildOptions &options,
        const cgride::project::Target &target,
        const std::filesystem::path &source)
    {
      return options.build_directory() /
             "obj" /
             target.name() /
             (sanitize_path_for_id(source) + std::string(cgride::core::object_file_extension(cgride::core::host_platform())));
    }

    [[nodiscard]] std::filesystem::path static_library_path_for(
        const BuildOptions &options,
        const cgride::project::Target &target)
    {
      return options.build_directory() /
             "lib" /
             (std::string(cgride::core::static_library_prefix(cgride::core::host_platform())) +
              target.name() +
              std::string(cgride::core::static_library_extension(cgride::core::host_platform())));
    }

    [[nodiscard]] std::filesystem::path executable_path_for(
        const BuildOptions &options,
        const cgride::project::Target &target)
    {
      return options.build_directory() /
             "bin" /
             (target.name() + std::string(cgride::core::executable_extension(cgride::core::host_platform())));
    }

    void append_target_requirements(
        const cgride::project::Target &target,
        cgride::toolchains::CompileCommandOptions &compile_options)
    {
      for (const auto &requirement : target.requirements().entries())
      {
        if (!requirement.valid())
        {
          continue;
        }

        switch (requirement.kind())
        {
        case cgride::project::RequirementKind::IncludeDirectory:
          compile_options.include_directories.push_back(requirement.path());
          break;

        case cgride::project::RequirementKind::CompileDefinition:
          compile_options.definitions.push_back(requirement.value());
          break;

        case cgride::project::RequirementKind::CompileOption:
          compile_options.options.push_back(requirement.value());
          break;

        case cgride::project::RequirementKind::LinkOption:
        case cgride::project::RequirementKind::LinkLibrary:
          break;
        }
      }
    }

    void append_link_requirements(
        const cgride::project::Target &target,
        cgride::toolchains::LinkCommandOptions &link_options)
    {
      for (const auto &requirement : target.requirements().entries())
      {
        if (!requirement.valid())
        {
          continue;
        }

        switch (requirement.kind())
        {
        case cgride::project::RequirementKind::LinkOption:
          link_options.options.push_back(requirement.value());
          break;

        case cgride::project::RequirementKind::LinkLibrary:
          link_options.libraries.push_back(requirement.value());
          break;

        case cgride::project::RequirementKind::IncludeDirectory:
        case cgride::project::RequirementKind::CompileDefinition:
        case cgride::project::RequirementKind::CompileOption:
          break;
        }
      }
    }

    [[nodiscard]] std::vector<std::filesystem::path> explicit_sources_for(
        const cgride::project::Target &target)
    {
      std::vector<std::filesystem::path> sources;

      for (const auto &entry : target.source_set().entries())
      {
        if (!entry.valid())
        {
          continue;
        }

        if (entry.kind() == cgride::project::SourceKind::File)
        {
          sources.push_back(entry.path());
        }
      }

      return sources;
    }

    [[nodiscard]] cgride::core::Result<void> add_compile_tasks(
        cgride::graph::Graph &graph,
        const BuildRequest &request,
        const cgride::project::Target &target,
        PlannedTarget &planned)
    {
      const auto sources = explicit_sources_for(target);
      std::size_t index = 0;

      for (const auto &source : sources)
      {
        cgride::toolchains::CompileCommandOptions options;
        options.source = source;
        options.object = object_path_for(request.options(), target, source);
        options.debug = request.options().mode() == BuildMode::Debug;
        options.optimize = request.options().mode() == BuildMode::Release;

        append_target_requirements(target, options);

        auto command = cgride::toolchains::make_compile_command(
            request.toolchain().value(),
            options);

        if (!command)
        {
          return command.error();
        }

        const auto task_id = cgride::graph::TaskId(
            "compile:" + target.name() + ":" + std::to_string(index));

        graph.task(task_id, cgride::graph::TaskKind::Compile, "Compile " + source.string())
            .input(source)
            .output(options.object)
            .command(command.value());

        planned.compile_tasks.push_back(task_id);
        planned.objects.push_back(options.object);
        ++index;
      }

      return cgride::core::Result<void>::ok();
    }

    [[nodiscard]] cgride::core::Result<void> add_archive_task(
        cgride::graph::Graph &graph,
        const BuildRequest &request,
        const cgride::project::Target &target,
        PlannedTarget &planned)
    {
      if (target.kind() != cgride::project::TargetKind::StaticLibrary)
      {
        return cgride::core::Result<void>::ok();
      }

      cgride::toolchains::ArchiveCommandOptions options;
      options.objects = planned.objects;
      options.output = static_library_path_for(request.options(), target);

      auto command = cgride::toolchains::make_archive_command(
          request.toolchain().value(),
          options);

      if (!command)
      {
        return command.error();
      }

      const auto task_id = cgride::graph::TaskId("archive:" + target.name());
      auto &task = graph.task(task_id, cgride::graph::TaskKind::Archive, "Archive " + target.name());

      task.inputs(options.objects)
          .output(options.output)
          .depends_on(planned.compile_tasks)
          .command(command.value());

      planned.artifact = options.output;
      planned.artifact_task = task_id;

      return cgride::core::Result<void>::ok();
    }

    [[nodiscard]] cgride::core::Result<void> add_link_task(
        cgride::graph::Graph &graph,
        const BuildRequest &request,
        const cgride::project::Target &target,
        PlannedTarget &planned,
        const std::unordered_map<std::string, PlannedTarget> &planned_targets)
    {
      if (target.kind() != cgride::project::TargetKind::Executable)
      {
        return cgride::core::Result<void>::ok();
      }

      cgride::toolchains::LinkCommandOptions options;
      options.artifact_kind = cgride::toolchains::ArtifactKind::Executable;
      options.objects = planned.objects;
      options.output = executable_path_for(request.options(), target);

      std::vector<cgride::graph::TaskId> dependencies = planned.compile_tasks;

      for (const auto &link : target.target_links())
      {
        if (!link.valid())
        {
          continue;
        }

        const auto found = planned_targets.find(link.target_name());

        if (found == planned_targets.end())
        {
          options.libraries.push_back(link.target_name());
          continue;
        }

        if (!found->second.artifact.empty())
        {
          options.libraries.push_back(found->second.artifact.string());
        }

        if (found->second.artifact_task.has_value())
        {
          dependencies.push_back(found->second.artifact_task.value());
        }
      }

      append_link_requirements(target, options);

      auto command = cgride::toolchains::make_link_command(
          request.toolchain().value(),
          options);

      if (!command)
      {
        return command.error();
      }

      const auto task_id = cgride::graph::TaskId("link:" + target.name());
      auto &task = graph.task(task_id, cgride::graph::TaskKind::Link, "Link " + target.name());

      task.inputs(options.objects)
          .output(options.output)
          .depends_on(std::move(dependencies))
          .command(command.value());

      planned.artifact = options.output;
      planned.artifact_task = task_id;

      return cgride::core::Result<void>::ok();
    }

  } // namespace

  cgride::core::Result<BuildPlan> Planner::plan(
      const BuildRequest &request) const
  {
    auto validated = validate_request(request);

    if (!validated)
    {
      return validated.error();
    }

    cgride::graph::Graph graph(request.project().value().name());
    std::unordered_map<std::string, PlannedTarget> planned_targets;

    for (const auto &target_ptr : request.project().value().targets())
    {
      const auto &target = *target_ptr;
      PlannedTarget planned;

      auto compiled = add_compile_tasks(graph, request, target, planned);

      if (!compiled)
      {
        return compiled.error();
      }

      auto archived = add_archive_task(graph, request, target, planned);

      if (!archived)
      {
        return archived.error();
      }

      planned_targets.emplace(target.name(), std::move(planned));
    }

    for (const auto &target_ptr : request.project().value().targets())
    {
      const auto &target = *target_ptr;
      auto &planned = planned_targets.at(target.name());

      auto linked = add_link_task(graph, request, target, planned, planned_targets);

      if (!linked)
      {
        return linked.error();
      }
    }


    if (request.options().has_target())
    {
      const auto *selected_target = request.project().value().find_target(request.options().target().value());

      if (selected_target == nullptr)
      {
        return Error(
            ErrorCode::NotFound,
            "Requested build target was not found.",
            request.options().target().value());
      }
    }

    BuildPlan plan(
        std::move(graph),
        request.options().build_directory());

    if (request.options().has_target())
    {
      plan.target(request.options().target().value());
    }

    return plan;
  }

  cgride::core::Result<BuildPlan> plan_build(
      const BuildRequest &request)
  {
    Planner planner;

    return planner.plan(request);
  }

} // namespace cgride::engine
