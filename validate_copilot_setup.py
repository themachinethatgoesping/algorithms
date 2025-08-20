#!/usr/bin/env python3
"""
GitHub Copilot setup validation script for themachinethatgoesping/algorithms
"""

import json
import os
import sys
from pathlib import Path

def validate_json_file(file_path):
    """Validate that a JSON file is properly formatted"""
    try:
        with open(file_path, 'r') as f:
            json.load(f)
        print(f"✓ {file_path} is valid JSON")
        return True
    except json.JSONDecodeError as e:
        print(f"✗ {file_path} has JSON error: {e}")
        return False
    except FileNotFoundError:
        print(f"✗ {file_path} not found")
        return False

def check_file_exists(file_path, description):
    """Check if a file exists"""
    if os.path.exists(file_path):
        print(f"✓ {description}: {file_path}")
        return True
    else:
        print(f"✗ {description}: {file_path} not found")
        return False

def main():
    """Main validation function"""
    print("GitHub Copilot Setup Validation")
    print("=" * 40)
    
    success_count = 0
    total_checks = 0
    
    # Check VS Code configuration files
    vscode_files = [
        ('.vscode/settings.json', 'VS Code settings'),
        ('.vscode/extensions.json', 'VS Code extensions'),
        ('.vscode/launch.json', 'VS Code launch config'),
        ('.vscode/tasks.json', 'VS Code tasks'),
    ]
    
    for file_path, description in vscode_files:
        total_checks += 1
        if validate_json_file(file_path):
            success_count += 1
    
    # Check other configuration files
    other_files = [
        ('.github/copilot.yml', 'GitHub Copilot configuration'),
        ('.gitattributes', 'Git attributes'),
        ('.gitignore', 'Git ignore'),
        ('COPILOT_SETUP.md', 'Setup documentation'),
    ]
    
    for file_path, description in other_files:
        total_checks += 1
        if check_file_exists(file_path, description):
            success_count += 1
    
    # Check for specific content in .gitattributes
    total_checks += 1
    try:
        with open('.gitattributes', 'r') as f:
            content = f.read()
            if 'linguist-language=C++' in content:
                print("✓ .gitattributes has C++ language detection")
                success_count += 1
            else:
                print("✗ .gitattributes missing C++ language detection")
    except FileNotFoundError:
        print("✗ .gitattributes not found")
    
    # Summary
    print("\n" + "=" * 40)
    print(f"Validation Results: {success_count}/{total_checks} checks passed")
    
    if success_count == total_checks:
        print("🎉 GitHub Copilot setup is complete!")
        return 0
    else:
        print("⚠️  Some issues found. Please review and fix.")
        return 1

if __name__ == "__main__":
    sys.exit(main())