# Cgride Engine

`cgride-engine` is the orchestration layer of Cgride.

It connects the project model, toolchain description, build graph, executor, and cache primitives into one build flow. The engine does not print to the terminal, does not call `std::exit`, and does not own the CLI. It returns structured results so it can be embedded in a command-line tool, editor integration, server process, or another native application.

## Purpose

Cgride is built as a modular C++ build system. Each module owns a small part of the system:

- `cgride-core` provides shared primitives.
- `cgride-project` describes projects and targets.
- `cgride-graph` describes build tasks and dependencies.
- `cgride-toolchains` creates compiler, archiver, and linker commands.
- `cgride-executor` runs graph tasks.
- `cgride-cache` stores cache metadata.
- `cgride-engine` connects these modules into a build operation.

The engine is the first layer where these modules meet.

## Module boundary

This module depends on:

- `cgride::core`
- `cgride::project`
- `cgride::graph`
- `cgride::toolchains`
- `cgride::executor`
- `cgride::cache`

It must not depend on:

- `cgride::config`
- `cgride::cli`

This keeps the engine reusable outside the CLI.

## Main components

### BuildOptions

`BuildOptions` describes how a build should run.

It includes:

- build directory
- selected target
- debug or release mode
- job count
- rebuild mode
- cache usage
- dry-run mode
- verbose mode

Example:

```cpp
cgride::engine::BuildOptions options;

options
    .build_directory("build")
    .target("app")
    .mode(cgride::engine::BuildMode::Release)
    .jobs(8)
    .use_cache(true)
    .verbose(true);
```

### BuildRequest

`BuildRequest` is the complete input required by the engine.

It combines:

- a `cgride::project::Project`
- a `cgride::toolchains::Toolchain`
- `BuildOptions`

Example:

```cpp
cgride::engine::BuildRequest request(
    project,
    toolchain,
    options
);
```

The request does not build anything by itself. It only describes what the engine should prepare.

### BuildPlan

`BuildPlan` is produced by the planner.

It contains:

- the build graph
- selected target names
- the build directory

The plan does not execute commands and does not access the cache. It is a prepared build description.

### Planner

`Planner` converts a `BuildRequest` into a `BuildPlan`.

Current behavior:

- validates that the request has a project
- validates that the request has a toolchain
- validates build options
- creates an initial build plan
- forwards the selected target into the plan

Future behavior will generate compile, archive, and link tasks from the project model and toolchain.

### BuildResult

`BuildResult` is the final result returned by the engine.

It records:

- build status
- task results
- diagnostics
- optional error
- total duration

Statuses:

- `NotStarted`
- `Succeeded`
- `Failed`
- `Cancelled`

### BuildEngine

`BuildEngine` is the high-level orchestration object.

It can:

- plan a build
- execute a prepared plan
- plan and execute a request

Example:

```cpp
cgride::engine::BuildEngine engine;

auto result = engine.build(request);

if (!result.success())
{
  return 1;
}

return 0;
```

## Current behavior

The first version of the engine provides the build orchestration structure.

The planner currently creates an empty graph after validating the request. Because of that, a full build may fail cleanly when the executor rejects the empty graph. This is expected for the first engine foundation.

The next step is to expand the planner so it creates real graph tasks from project targets and toolchain commands.

## Example

```cpp
#include <cgride/engine/build_engine.hpp>

int main()
{
  cgride::project::Project project;
  cgride::toolchains::Toolchain toolchain;

  cgride::engine::BuildOptions options;

  options
      .build_directory("build")
      .target("app")
      .mode(cgride::engine::BuildMode::Debug)
      .jobs(4);

  cgride::engine::BuildRequest request(
      project,
      toolchain,
      options
  );

  cgride::engine::BuildEngine engine;

  auto result = engine.build(request);

  if (!result.success())
  {
    return 1;
  }

  return 0;
}
```

## Build

From the module directory, use the Vix workflow:

```bash
vix build
```

For a release build:

```bash
vix build --preset release
```

## Run tests

```bash
vix check --tests
```

Or run the test command directly:

```bash
vix tests
```

## Install

```bash
vix install
```

The install step exposes the `cgride::engine` integration target, public headers, and package metadata.

