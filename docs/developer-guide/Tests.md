# wx_BGI_Graphics — Test Suite

This document describes the automated test suite for wx_BGI_Graphics.  
All tests are registered with CTest and run automatically in CI.

See **[Building.md](.Building.md)** for build commands.  
See **[WxWidgets.md](../WxWidgets.md)** for the embedded canvas integration guide.

---

## Test Summary

The CTest target count depends on the enabled wxWidgets, OpenLB, FreePascal, and
test-seam options. The table lists the targets available with wxWidgets, FPC,
and test seams enabled; OpenLB adds an additional smoke test when enabled.

| # | Test Name | Category | Source File | Notes |
|---|-----------|----------|-------------|-------|
| 1 | `test_fonts` | C++ | `examples/cpp/test_fonts.cpp` | Font rendering and metrics |
| 2 | `bgi_api_coverage_cpp` | C++ | `examples/cpp/bgi_api_coverage.cpp` | Classic BGI API coverage |
| 3 | `wx_bgi_solids_test` | wxWidgets | `examples/wx/wx_bgi_solids_test.cpp` | 3-D solids in embedded canvas |
| 4 | `wx_bgi_3d_orbit_test` | wxWidgets | `examples/wx/wx_bgi_3d_orbit_test.cpp` | 3-D orbit animation in embedded canvas |
| 5 | `wx_bgi_canvas_coverage_test` | wxWidgets | `examples/wx/wx_bgi_canvas_coverage_test.cpp` | Full BGI API coverage in embedded canvas |
| 6 | `test_dds_external_attrs` | C++ / DDS | `examples/cpp/test_dds_external_attrs.cpp` | External object attributes |
| 7 | `test_dds_cam3d_persp` | C++ / DDS | `examples/cpp/test_dds_cam3d_persp.cpp` | 3-D perspective camera |
| 8 | `test_multi_scene` | C++ / DDS | `examples/cpp/test_multi_scene.cpp` | Scene lifecycle, camera assignment, and JSON round-trip |
| 9 | `test_solids` | C++ / DDS | `examples/cpp/test_solids.cpp` | 3-D solid primitives and draw modes |
| 10 | `test_dds_serialize` | C++ / DDS | `examples/cpp/test_dds_serialize.cpp` | DDS JSON and YAML serialization |
| 11 | `test_dds_deserialize` | C++ / DDS | `examples/cpp/test_dds_deserialize.cpp` | DDS JSON and YAML deserialization |
| 12 | `test_openlb_bridge_materialize_2d` | C++ / DDS | `examples/cpp/test_openlb_bridge_materialize_2d.cpp` | OpenLB bridge materialization |
| 13 | `test_dds_clear` | C++ / DDS | `examples/cpp/test_dds_clear.cpp` | DDS scene clear |
| 14 | `test_dds_layers` | C++ / DDS | `examples/cpp/test_dds_layers.cpp` | Layer lifecycle, assignment, visibility, persistence, and non-retained previews |
| 15 | `wxbgi_rubberband_demo` | C++ / DDS demo smoke | `examples/cpp/wxbgi_rubberband_demo.cpp` | Bounded run of immediate and transform previews |
| 16 | `test_dds_cam2d_yz` | C++ / DDS | `examples/cpp/test_dds_cam2d_yz.cpp` | 2-D camera in YZ plane |
| 17 | `test_dds_csg` | C++ / DDS | `examples/cpp/test_dds_csg.cpp` | Retained transforms, set operations, and CSG regressions |
| 18 | `test_field_vis` | C++ / field vis | `examples/cpp/test_field_vis.cpp` | Scalar grid, vector glyphs, and legend helpers |
| 19 | `bgi_api_coverage_python` | Python | `examples/python/bgi_api_coverage.py` | ctypes BGI API coverage |
| 20 | `test_input_hooks` | C++ | `examples/cpp/test_input_hooks.cpp` | Keyboard hooks (requires `WXBGI_ENABLE_TEST_SEAMS`) |
| 21 | `test_input_bypass` | C++ | `examples/cpp/test_input_bypass.cpp` | Input bypass and scroll hook (requires test seams) |
| 22 | `bgi_api_coverage_pascal_build` | Pascal | `examples/demoFreePascal/demo_bgi_api_coverage.pas` | FPC compile |
| 23 | `bgi_api_coverage_pascal_run` | Pascal | same | GLFW and wx-standalone path |
| 24 | `bgi_canvas_coverage_pascal_build` | Pascal | `examples/demoFreePascal/demo_bgi_canvas_coverage.pas` | FPC compile |
| 25 | `bgi_canvas_coverage_pascal_run` | Pascal | same | wx-only path (no GLFW) |
| 26 | `test_input_hooks_pascal_build` | Pascal | `examples/demoFreePascal/test_input_hooks.pas` | FPC compile (requires test seams) |
| 27 | `test_input_hooks_pascal_run` | Pascal | same | Keyboard hooks |
| 28 | `test_input_bypass_pascal_build` | Pascal | `examples/demoFreePascal/test_input_bypass.pas` | FPC compile (requires test seams) |
| 29 | `test_input_bypass_pascal_run` | Pascal | same | Input bypass and scroll hooks |

Pascal tests run only when a matching-architecture FreePascal compiler (`fpc`) is
found at CMake configure time. Input-seam tests require
`WXBGI_ENABLE_TEST_SEAMS=ON`.

---

## Test Categories

### C++ and wxWidgets Tests

These tests link directly against `wx_bgi_graphics` (the DLL/shared library) and exercise the full API surface through a real graphics window.

| Test | What it checks |
|------|----------------|
| `test_fonts` | Font rendering and metrics |
| `bgi_api_coverage_cpp` | All classic BGI drawing functions: `line`, `circle`, `arc`, `bar`, `fillpoly`, `outtextxy`, `getimage`/`putimage`, page switching, palette, viewport |
| `wx_bgi_solids_test`, `wx_bgi_3d_orbit_test`, `wx_bgi_canvas_coverage_test` | Embedded wxWidgets canvas rendering and API coverage |
| `test_dds_external_attrs` | DDS object external attributes |
| `test_dds_cam3d_persp` | 3-D perspective camera projection |
| `test_multi_scene` | Multi-scene lifecycle, camera assignment, and serialization |
| `test_solids` | 3-D solid primitives and draw modes |
| `test_dds_serialize` | `wxbgi_dds_save_json` / `wxbgi_dds_save_yaml` round-trip correctness |
| `test_dds_deserialize` | `wxbgi_dds_load_json` / `wxbgi_dds_load_yaml` round-trip correctness |
| `test_openlb_bridge_materialize_2d` | OpenLB bridge materialization |
| `test_dds_clear` | `wxbgi_dds_clear` removes all scene objects |
| `test_dds_layers` | Layer creation/deletion/clear, object layer assignment, visibility, JSON/YAML persistence, and non-retained rubber-band draws/transforms |
| `wxbgi_rubberband_demo` | Bounded launch smoke test for immediate rubber-band drawing and transient transform previews |
| `test_dds_cam2d_yz` | 2-D orthographic camera in the YZ plane; coordinate projection |
| `test_dds_csg` | `wxbgi_dds_translate`, `wxbgi_dds_rotate_*`, `wxbgi_dds_scale_*`, `wxbgi_dds_skew`, `wxbgi_dds_union`, `wxbgi_dds_intersection`, `wxbgi_dds_difference`, retained child references, JSON tokens for Transform / SetUnion / SetIntersection / SetDifference, draw-mode propagation to retained set-op nodes, a 2D ordered-difference case, and exact 3D union/intersection/difference regressions including scaled retained operands |
| `test_field_vis` | Scalar-grid, vector-grid, and legend helpers used for solver visualisation |
| `test_input_hooks`, `test_input_bypass` | Keyboard/input-hook test seams (optional) |

### Python Test

```bash
python examples/python/bgi_api_coverage.py build/Debug/wx_bgi_graphics.dll    # Windows
python3 examples/python/bgi_api_coverage.py build/libwx_bgi_graphics.so       # Linux
python3 examples/python/bgi_api_coverage.py build/libwx_bgi_graphics.dylib    # macOS
```

Exercises the full BGI API surface via Python `ctypes` — the same functions as the C++ coverage test.

### FreePascal Tests

Each Pascal test has a **build** step (FPC compile) and a **run** step.  
The build step copies the up-to-date DLL into the output directory automatically.

| Tests | Source | What it checks |
|-------|--------|----------------|
| `bgi_api_coverage_pascal_*` | `demo_bgi_api_coverage.pas` | Full BGI API surface: GLFW `initgraph` / `closegraph` → `wxbgi_wx_frame_create`; all classic drawing functions + extension API |
| `bgi_canvas_coverage_pascal_*` | `demo_bgi_canvas_coverage.pas` | wx-only path (no GLFW): `wxbgi_wx_app_create` + `wxbgi_wx_frame_create`; colorful primitives phase + blue-screen phase + extension API |
| `test_input_hooks_pascal_*` | `test_input_hooks.pas` | Pascal keyboard hook API (requires test seams) |
| `test_input_bypass_pascal_*` | `test_input_bypass.pas` | Pascal input bypass / scroll hooks (requires test seams) |

> **Note:** `demo_bgi_canvas_coverage.pas` is the **wx-only** version — it does not call `initgraph` or `closegraph`. It uses a `YieldMs()` helper to keep the wx event loop alive between draw phases.

#### Linux / macOS: FreePascal FPU Exception Mask

On Linux and macOS, FreePascal enables strict x87 FPU exception masking by default.
GTK's SVG icon renderer (`librsvg` → `libxml2`) performs floating-point operations
(NaN / denormal arithmetic) that violate this strict mask and raise `SIGFPE`,
appearing as:

```
An unhandled exception occurred at $...:
EInvalidOp: Invalid floating point operation
```

All Pascal test programs that open a wx window already include the fix at the top
of their `begin` block:

```pascal
uses SysUtils, Math;

begin
  SetExceptionMask([exInvalidOp, exDenormalized, exZeroDivide,
                    exOverflow, exUnderflow, exPrecision]);
  // ... rest of program
```

**When writing new Pascal programs** on Linux / macOS that use `wxbgi_wx_app_create`
or any GTK-backed wx function, add `Math` to the `uses` clause and call
`SetExceptionMask` as the very first statement.

#### Linux / macOS: GLEW Initialisation Before `wxbgi_wx_app_main_loop`

`WxBgiCanvas::Render()` performs lazy GLEW initialisation the first time the
canvas paints.  In the C++ tests this paint occurs inside the wx event loop.
Pascal (and Python / C) programs that call GL-dependent extension functions such
as `wxbgi_write_pixels_rgba8` *before* `wxbgi_wx_app_main_loop` would previously
crash with `EAccessViolation` (null `glWindowPos2i` function pointer).

`wxbgi_wx_frame_create` now calls `wxApp::Yield()` followed by an explicit
`WxBgiCanvas::Render()` on Linux / macOS after `Show(true)`, so that the GL
context is fully initialised — including the DLL-local GLEW function pointer
table — before the function returns.  This makes GL extension calls safe
immediately after `wxbgi_wx_frame_create` on all platforms.

### wxWidgets Canvas Tests

These link against `wx_bgi_wx` (the static wx integration library) and use `WxBgiCanvas` embedded inside a `wxFrame`.  
They are the C++ equivalents of the standalone wx demos.

| Test | Duration | What it checks |
|------|----------|----------------|
| `wx_bgi_solids_test` | < 1 s | 3-D solids render without crash; depth buffer works |
| `wx_bgi_3d_orbit_test` | ~5 s | Camera orbit animation; solid occlusion across 360 ° |
| `wx_bgi_canvas_coverage_test` | ~4 s | Full BGI API coverage in embedded `WxBgiCanvas`; timer-based two-phase flow |

---

## Running the Tests

### Windows (MSVC, multi-config)

```powershell
# Debug
cmake -S . -B build
cmake --build build -j --config Debug
ctest --test-dir build -C Debug --output-on-failure

# Release
cmake -S . -B build
cmake --build build -j --config Release
ctest --test-dir build -C Release --output-on-failure

# Run a single test
ctest --test-dir build -C Debug -R bgi_api_coverage_cpp --output-on-failure

# Run a category by regex
ctest --test-dir build -C Debug -R pascal --output-on-failure
ctest --test-dir build -C Debug -R wx_bgi --output-on-failure
```

### Linux (single-config)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
ctest --test-dir build --output-on-failure

# Single test
ctest --test-dir build -R bgi_api_coverage_cpp --output-on-failure
```

### macOS (single-config, Apple Silicon)

First-time setup — install system GLFW and wxWidgets if not already done:

```bash
brew install glfw wxwidgets
```

Configure and build with the system-package flags:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug \
  -DWXBGI_SYSTEM_GLFW=ON -DWXBGI_SYSTEM_WX=ON \
  -DWXBGI_ENABLE_TEST_SEAMS=ON
cmake --build build -j
ctest --test-dir build --output-on-failure
```

> **FreePascal tests on macOS:** Pascal CTest targets require a working
> FPC Homebrew install with correct linker paths.  If FPC is installed to a
> non-standard Homebrew prefix the linker may report `ld: library 'c' not found`.
> C++, Python, and wxWidgets tests are independent of FreePascal availability.

### Single test

```bash
ctest --test-dir build -R bgi_api_coverage_cpp --output-on-failure
```

### Enabling Test Seams (keyboard injection)

```powershell
cmake -S . -B build -DWXBGI_ENABLE_TEST_SEAMS=ON
cmake --build build -j --config Debug
ctest --test-dir build -C Debug -R "input" --output-on-failure
```

---

## Test Seam Security Policy

### Keyboard Injection Test Seams (`WXBGI_ENABLE_TEST_SEAMS`)

These APIs allow CI to inject synthetic key events into the keyboard queue deterministically:

- `wxbgi_test_clear_key_queue()` — empties the keyboard queue
- `wxbgi_test_inject_key_code(code)` — enqueues a translated key code
- `wxbgi_test_inject_extended_scan(scan)` — enqueues a DOS-style extended scan code

**Security rules:**

- Compiled **only** when `-DWXBGI_ENABLE_TEST_SEAMS=ON` (default is `OFF`).
- Release and public distribution builds **must not** enable this option.
- CI test jobs that require keyboard injection set this flag explicitly.
- The guard `#ifdef WXBGI_ENABLE_TEST_SEAMS` is present in all usage sites.

```powershell
# CI / test-only configure — never use for release artifacts
cmake -S . -B build -DWXBGI_ENABLE_TEST_SEAMS=ON
```

### Multi-Scene Demo Headless Mode (`--test` flag)

`wxbgi_multi_scene_demo` supports an always-available `--test` mechanism:
passing it on the command line causes the demo to build both scene graphs, render
one frame with all three cameras, and exit cleanly.

- No compile-time option required.
- No synthetic input surface exposed.
- This interactive demo is not registered in CTest.

```powershell
# Manual headless run
.\build\Debug\wxbgi_multi_scene_demo.exe --test
```

### Camera Demo Headless Mode (`--test` flag)

`wxbgi_camera_demo_cpp` uses a separate, always-available mechanism: passing `--test` on the command line causes it to render exactly one frame and exit cleanly.

- No compile-time option required.
- No synthetic input surface exposed.
- This interactive demo is not registered in CTest.

```powershell
# Manual headless run
.\build\Debug\wxbgi_camera_demo_cpp.exe --test
```

---

## CI Integration

The repository workflow file is `.github/workflows/CI.yml`.

**Normal push / pull request:** matrix CI in Debug mode on Windows, Linux, and macOS.

**Tag push (`v1.2.3`):** Release build → tests → docs generation → packaging:

| Artifact | Description |
|----------|-------------|
| `wx_bgi_graphics-windows-x64.zip` | Windows x64 DLL + headers |
| `wx_bgi_graphics-linux-x64.tar.gz` | Linux x64 shared library + headers |
| `wx_bgi_graphics-macos-x64.tar.gz` | macOS x64 dylib + headers |
| `api-docs.zip` / `api-docs.tar.gz` | Doxygen HTML documentation |

Doxygen HTML docs are published to **GitHub Pages** on every release tag.

### Creating a Release

```bash
git tag v1.2.3
git push origin v1.2.3
```

### Enabling GitHub Pages (first time)

1. Repository **Settings → Pages → Source** → set to **GitHub Actions**.
2. **Settings → Environments → github-pages** → allow deployments from `v*` tags  
   (or remove the branch/tag restriction).
