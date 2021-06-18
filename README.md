# HarmonyEngine
3D Game Engine developed for making games in OpenGL with C++.

## Quick Links

1. [Cloning and Building Project](#cloning-and-building-project)
2. [Generating Changelog](#generating-the-changelog)
3. [How to Contribute](#how-to-contribute)

---

## Cloning and Building Project

1. [Mac Instructions](#getting-started-with-mac)
2. [Windows 10 Instructions](#getting-started-with-windows)
3. [Linux Instructions](#getting-started-with-linux)

### Getting Started With Mac

**1. Clone the Repository**
```shell
git clone https://github.com/Nick-Fanelli/HarmonyEngine --recursive
```
***Note: Use flag --recursive to get access to the submodules***

**2. Installing the Dependencies**

* Install [Homebrew](https://brew.sh/)
    ```shell
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    ```

* Install GLFW, CMake and GLEW
    ```shell
    brew install cmake glfw glew
    ```
**3. Building the Project**

* Navigate to `HarmonyEngine/build` directory; or create it.
* Build project with CMake
    ```shell
    cmake ..
    ```
**4. Build and Run The C++ Code**

* Run the Makefile
    ```shell
    make
    ```
* Run the Excecutable
    ```shell
    ./HarmonyEngine
    ```
---

### Getting Started with Windows

Getting statred instructions for Windows coming soon...

---

### Getting Started with Linux

Getting statred instructions for Linux coming soon...

---

## Generating the Changelog

**1. Make sure that [NPM](https://www.npmjs.com/) command line tools are installed**<br>
**2. Get the needed dependencies**
* Run the following command in the root directory of the project
```shell
npm install
```
**3. Create the version/change-log commit**
* Run the following command in the root directory of the project *(Replace {TARGET_VERSION_TAG} with the version tag. eg. v1.0.0 or v1.0.0-snapshot)*
```shell
npm run release -- --release-as {TARGET_VERSION_TAG}
```
**4. Push the new tag/version**
* Run the following command in the root directory of the project
```shell
git push --follow-tags
```

---

## How to Contribute

No contributions currently needed!
Instructions Coming soon...
*Follow the repo for alerts*
