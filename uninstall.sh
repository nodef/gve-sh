#!/usr/bin/env bash
# Default option values.
: "${PREFIX:=$HOME/.local}"
HELP="0"


# Parse options.
while [[ $# -gt 0 ]]; do
  case "$1" in
    -h|--help)
      HELP="1"
      shift
      ;;
    -p|--prefix)
      PREFIX="$2"
      shift 2
      ;;
    *)
      echo "Unknown option: $1"
      exit 1
      ;;
  esac
done


# Print help message.
if [[ "$HELP" == "1" ]]; then
  echo "Usage: ./uninstall.sh [options]"
  echo "Options:"
  echo "  -h, --help      Print this message and exit."
  echo "  -p, --prefix    Install prefix (default: ~/.local)."
  exit 0
fi


# Uninstall program.
mkdir -p "$PREFIX/bin"
echo "Uninstalling gve.sh ..."
rm -f "$PREFIX/bin/gve"
echo "Uninstalled gve.sh from '$PREFIX/bin/gve'."
