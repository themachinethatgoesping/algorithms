# algorithms
Algorithms for processing ping data (a.g. apply absorption, raytrace beams or images, bottom detection in echograms ...)

## GitHub Copilot Setup

This repository is configured to work optimally with GitHub Copilot for AI-assisted coding.

### Prerequisites
- VS Code with the GitHub Copilot extension
- C++ extension pack for VS Code
- Meson build system support

### Recommended VS Code Extensions
The repository includes extension recommendations in `.vscode/extensions.json`:
- GitHub Copilot & Copilot Chat
- C/C++ Extension Pack
- Meson Build Support
- Python Extension (for pymodule development)

### Configuration
- **Language Detection**: Enhanced `.gitattributes` for better language identification
- **IntelliSense**: Configured for c++23 standard with project-specific include paths
- **Code Formatting**: Uses existing `.clang-format` configuration
- **Project Context**: `.github/copilot.yml` provides project-specific context for better suggestions

### Usage Tips
- Copilot works best with clear, descriptive function and variable names
- Comments describing intent help generate better suggestions
- The configuration automatically enables Copilot for C++, Python, and Meson files
- Build artifacts and cache directories are excluded from Copilot suggestions
