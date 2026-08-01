# ML_Engine

A custom C++ / DirectX 11 game engine built from scratch, developed between **April 2025 and June 2026**. ML_Engine started as a small collection of graphics and math utilities and grew over its development into a full engine with its own rendering pipeline, animation system, physics, audio, event system, and a data-driven entity/component/service architecture for loading levels from JSON.

Rather than being built for a single game, ML_Engine was developed iteratively through a series of standalone tech demos and gameplay prototypes, each one exercising and expanding a different part of the engine. Those subprojects are documented below.

## Engine Features

- **Rendering** - DirectX 11 pipeline with custom HLSL effects: `StandardEffect` (lit/textured geometry), `ShadowEffect` (shadow mapping), `TerrainEffect` (heightmap terrain), `SimpleTextureEffect` (lightweight unlit rendering), and a post-processing `Infrared` effect (thermal-vision shader with distortion, blur, luminance-based heat gradient, and edge detection).
- **Animation** - Skeletal `Animator`/`Animation` playback for imported character rigs, plus a code-driven `AnimationBuilder` for authoring keyframed position/rotation/scale animations directly in C++ (used for cinematic camera and object paths).
- **Terrain** - Heightmap-based terrain generation and sampling (including height queries for camera/character grounding).
- **Particles** - A configurable `ParticleSystem` (spawn rate, lifetime, color/scale over lifetime, spawn angle/speed) driven by a dedicated particle render effect.
- **Physics** - A `PhysicsWorld` service integrated into the engine's game-object system, plus a standalone multithreaded ball-physics system (see *Ball Physics Multithreading* below) used to prototype parallelized simulation before integrating similar ideas into the engine.
- **Audio** - `SoundEffectManager` for loading and triggering sound effects, plus an `AudioSystem` service.
- **Events** - An engine-wide `EventManager` with custom event types and listeners, paired with a `TimeEventManager` for scheduling events to fire at specific timestamps (used to choreograph cutscenes).
- **GameWorld / ECS** - A `GameWorld` containing `GameObject`s built from composable `Component`s and `Service`s (e.g. `CameraComponent`, `FPSCameraComponent`, `CameraService`), with support for loading full levels from JSON templates.
- **Debug Tooling** - ImGui-based debug UI wired into nearly every system, letting values (lighting, materials, transforms, physics settings) be tuned live at runtime.

## Tech Stack

- C++ (C++17/20, including `std::execution` parallel algorithms, `std::thread`, `std::mutex`/`std::condition_variable`)
- DirectX 11 + HLSL for rendering and shaders
- Dear ImGui for debug tooling
- Windows API (win32 application/window layer)

## Subprojects

Each subproject below is a standalone `GameState` built on top of ML_Engine, used to develop and showcase a specific engine system.

| Subproject | Date | Focus |
|---|---|---|
| [Simple Solar System](#simple-solar-system) | June 2025 | Textured sphere rendering, orbital motion, render-target-based camera views |
| [Infrared](#infrared) | December 2025 | Terrain, shadow mapping, and a custom thermal-vision post-processing shader |
| [Invasion Animation](#invasion-animation) | March 2026 | Scripted cinematic combining animation, event-driven cutscene direction, particles, and audio |
| [Wall Jump](#wall-jump) | June 2026 | Data-driven level loading through the engine's GameWorld/ECS and JSON level templates |
| [Ball Physics Multithreading](#ball-physics-multithreading) | May 2026 | Multithreaded ball-vs-ball physics simulation with a dedicated collision-detection thread |

### Simple Solar System

A textured 3D solar system: the solar system and all its planets each rotating on its own axis while orbiting the sun at an individual speed. Includes a secondary render-target camera that can focus on any body in the system (selectable through the debug UI) to render a picture-in-picture close-up view, alongside a free-flying main camera for exploring the scene.

### Infrared

A terrain and lighting demo built around a custom thermal-vision post-processing effect. Characters are lit with shadow-mapped directional lighting over a heightmap terrain, then the whole scene is rendered to a texture and run through a hand-written HLSL shader that applies UV distortion, a box blur, luminance-based heat-color grading (cold blue → hot yellow-white), bloom-style glow on bright areas, and Sobel edge detection, composited with a HUD overlay texture.

### Invasion Animation

The engine's most ambitious showcase: a fully scripted ~40-second cinematic combining skeletal character animation, keyframed camera and prop animation (via `AnimationBuilder`), a custom `TimeEventManager` that fires timestamped events to drive the scene, particle effects (blood and explosions), synchronized sound cues, and a toggleable infrared view, all coordinated through the engine's event system rather than hardcoded per-frame logic.

### Wall Jump

A gameplay prototype demonstrating the engine's data-driven side: the entire level (geometry, entities, wall-jump mechanics) is defined in a JSON level template and loaded through the engine's `GameWorld`/component/service architecture, with the ability to hot-reload the level from the debug UI.

### Ball Physics Multithreading

A standalone physics stress-test: dozens of balls simulate gravity, floor bouncing, and ball-to-ball collision with restitution and friction. Per-ball integration runs in parallel via `std::execution::par_unseq`, while collision *detection* runs on its own dedicated background thread, synchronized with the main thread each frame through a mutex/condition-variable handshake so detection and gameplay update never block each other for longer than necessary.
