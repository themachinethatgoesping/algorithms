# GitHub Copilot Setup Guide

This repository is now configured for optimal GitHub Copilot integration. This guide explains what has been set up and how to use it effectively.

## What's Been Configured

### 1. VS Code Configuration (`.vscode/`)
- **`settings.json`**: Configures c++23 standard, include paths, and enables Copilot for relevant file types
- **`extensions.json`**: Recommends essential extensions including GitHub Copilot
- **`tasks.json`**: Provides build, test, and clean tasks using Meson
- **`launch.json`**: Debug configuration for C++ tests

### 2. Language Detection (`.gitattributes`)
Enhanced to properly identify C++, C, and Meson files for better Copilot context:
```
*.hpp linguist-language=C++
*.cpp linguist-language=C++
*.h linguist-language=C
meson.build linguist-language=Meson
```

### 3. Project Context (`.github/copilot.yml`)
Provides GitHub Copilot with project-specific information:
- Build system: Meson
- C++ standard: c++23
- Key dependencies: Eigen, xtensor, pybind11
- Namespace structure and conventions

### 4. Documentation (`README.md`)
Added comprehensive Copilot setup instructions.

## Getting Started

### Prerequisites
1. Install VS Code
2. Install the recommended extensions (VS Code will prompt you when opening the project)
3. Ensure you have GitHub Copilot access

### Required System Dependencies
For building the project, you'll need:
```bash
# Ubuntu/Debian
sudo apt install build-essential cmake pkg-config libboost-all-dev libeigen3-dev

# Install Meson and Ninja
pip3 install meson ninja
```

### Using the Configuration

#### Building the Project
1. Open VS Code in the project directory
2. Press `Ctrl+Shift+P` and type "Tasks: Run Task"
3. Select "build" to build the project
4. Or use the terminal: `meson setup builddir && meson compile -C builddir`

#### Copilot Tips for This Project
1. **Function Documentation**: Start with `/**` to get Doxygen-style documentation suggestions
2. **Class Implementation**: Copilot understands the project's namespace structure (`themachinethatgoesping::algorithms::`)
3. **Test Writing**: When in test files, Copilot will suggest Catch2-style test cases
4. **Python Bindings**: In `pymodule/` directory, Copilot understands pybind11 patterns

#### Example Copilot Prompts
- "Create a new algorithm class for signal processing"
- "Add unit tests for the raytracer functionality"
- "Generate Python bindings for this C++ class"
- "Implement Eigen-based matrix operations"

## File Structure Context
- `src/themachinethatgoesping/algorithms/`: Core C++ implementation
- `src/pymodule/`: Python bindings using pybind11
- `src/tests/`: Unit tests using Catch2 framework
- `meson.build`: Build configuration

## Troubleshooting
- If IntelliSense doesn't work, ensure the C++ extension is installed
- If build tasks fail, check that Meson and dependencies are installed
- If Copilot suggestions are poor, ensure file types are properly detected in `.gitattributes`

## Contributing
When adding new files:
1. Follow the existing namespace structure
2. Add appropriate file extensions to maintain language detection
3. Use descriptive function/variable names for better Copilot suggestions
4. Include comments describing intent for complex algorithms