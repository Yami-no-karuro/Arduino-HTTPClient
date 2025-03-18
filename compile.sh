#!/bin/bash

# ====
# Global variables
# ====

FQBN=arduino:renesas_uno:unor4wifi
BUILD_PATH=build

# ====
# Restrictions
# ====

if ! command -v arduino-cli &> /dev/null; then
    echo "Error: \"arduino-cli\" is not installed."
    echo "Exiting..."
    exit 1
fi

# ====
# Helper functions
# ====

confirm_action() {
    while true; do
        echo -n "$1 (y/n): "
        read -r confirmation
        case "$confirmation" in
            y|Y) return 0 ;;
            n|N) return 1 ;;
            *) echo "Invalid input. Please enter 'y' or 'n'." ;;
        esac
    done
}

# ====
# Main
# ====

echo "Arduino CLI - Settings:"
echo "---------------------------------------------"
echo "Working directory: '$(pwd)'"
echo "Build path: '$(pwd)/$BUILD_PATH'"
echo "FQBN: '$FQBN'"

if ! confirm_action "Do you want to proceed?"; then
    echo "Exiting..."
    exit 0
fi

arduino-cli compile --fqbn "$FQBN" . --build-path "$BUILD_PATH"
echo "Exiting..."
