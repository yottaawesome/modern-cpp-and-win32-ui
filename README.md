# Modern C++ and Win32 UI

## Introduction

A sample intended to demonstrate working with various native Win32 GUI windows, controls and layouts with modern C++. Originally spun off from my `win32-experiments` repo after I found the techniques developed there to be useful for serious desktop app development. One partially-complete sample is currently added, but more development will be done over time, alongside better documentation of the techniques applied.

## Building

Visual Studio 2026 with the `Desktop development with C++` workload is required. Make sure `MSVC AddressSanitizer` is enabled in Visual Studio's individual components, as the debug builds are instrumented for memory safety. I use the preview MSVC build tools (installed via the `MSVC Build Tools for x64/x86 (Preview)` Visual Studio component), but this is just a personal preference of mine that isn't strictly necessary and can be disabled in the project settings if you wish to use the current mainline MSVC build tools.

## Concepts that will be covered

* Using various native Win32 controls;
* Top-level and child windows;
* Control subclassing;
* Custom window classes and windows;
* Strongly-typed message handling;
* Message reflection;
* Callbacks for UI interaction;
* Error handling.
