# HarmonyEngine

**Harmony Engine** is currently a *3D and 2D game engine* in development with hopes to one day be a standalone game engine that can be used to *develop 3D or 2D games*.

![Image of Harmony Engine v0.3.0 (Jul 25, 2021)](screenshots/harmony-engine-window.png)
<p align="center"><i>Image of Harmony Engine SNAPSHOT v0.6.0 (Aug 27, 2021)</i></p>

<br />

Currently *Harmony Engine* is begin developed on MacOS however, with a few quick adjustments it should be able to run on Windows 10 as well. *(Windows support coming soon)!!!*

## Quick Links

1. [Highlight Features and Goals](#highlight-features)
1. [Changelog](CHANGELOG.md)
1. [Instructions for Cloning and Building Project](#cloning-and-building-project)
1. [Instructions for Generating Changelog](#generating-the-changelog)
1. [How to Contribute](#how-to-contribute)

---

## Highlight Features
* 3D Rendering
* 2D Rendering
* Editor based workflow
* Lua Scripting
* Drag and Drop Assets Functionality
* Serialization System

## Short-Term Goals
* Editor Mouse Picking
* Integration with Operating System *(eg. file recognition, etc.)*
* Editor Undo/Redo System
* Engine Runtime (halfway done)

## Long-Term Goals
* Compile *Harmony Engine/Editor* to a Window Executable File.
* Audio Support
* Ability to work with external applications. *(eg. Blender, Maya, Photoshop, etc.)*
* Ability to compile a Harmony Project into a native executable. *(meaning you will be able to run your project outside of Harmony Engine)* 
* Mobile platform support

---

# Cloning and Building Project

1. **[Mac Instructions](#getting-started-with-mac)**
2. **[Windows 10 Instructions (Coming Soon)](#getting-started-with-windows)**
3. **[Linux Instructions (Coming Soon)](#getting-started-with-linux)**

<br />

---

## Getting Started With Mac

<br />

**1. Clone the Repository** - *Make sure you have git command line tools installed! (on MacOS simply running `git` in terminal will prompt its installation)*
```shell
git clone git@github.com:Nick-Fanelli/HarmonyEngine.git --recursive
```
***Note: Use flag --recursive to get access to the submodules***

**2. Installing CMake**

* Make sure [Homebrew](https://brew.sh/) is installed (if so skip this step)
    ```shell
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    ```

* Install, CMake
    ```shell
    brew install cmake
    ```
**3. Building the Project**

* Navigate to `HarmonyEngine/build` directory; or create it.
* Build project with **CMake**
    ```shell
    cmake ..
    ```
**4. Build and Run The C++ Code**

* Run the Makefile
    ```shell
    make
    ```
* Run the Executable
    ```shell
    ./HarmonyEditor/HarmonyExecutable
    ```

    ***Note: Make sure you run the executable from the build directory!***
---

## Getting Started with Windows

<br />

Getting started instructions for Windows coming soon...

---

## Getting Started with Linux

<br />

Getting started instructions for Linux coming soon...

---

# Generating the Changelog

**1. Make sure that [NPM](https://www.npmjs.com/) command line tools are installed**<br>
**2. Get the needed dependencies**
* Run the following command in the root directory of the project
```shell
npm install
```
**3. Create the version/change-log commit**
* Run the following command in the root directory of the project *(Replace {TARGET_VERSION_TAG} with the version tag. eg. v1.0.0)*
**Note: Please use the vX.X.X format when tagging versions ***(eg. v1.0.0, v0.1.0, v1.0.2)*****
```shell
npm run release -- --release-as {TARGET_VERSION_TAG}
```
**4. Push the new tag/version**
* Run the following command in the root directory of the project
```shell
git push --follow-tags
```

---

# How to Contribute

No contributions currently needed!
Instructions Coming soon...
*Follow the repo for alerts*
