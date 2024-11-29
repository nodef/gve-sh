#!/usr/bin/env bash
# Default option values.
: "${PREFIX:=$HOME/.local}"
: "${ACTION:=install}"
HELP="0"


# Parse options.
while [[ $# -gt 0 ]]; do
  case "$1" in
    -h|--help)
      HELP="1";
      shift ;;
    -p|--prefix)
      PREFIX="$2"
      shift 2 ;;
    install)
      ACTION="install"
      shift ;;
    uninstall)
      ACTION="uninstall"
      shift ;;
    *)
      echo "Unknown option: $1"
      exit 1 ;;
  esac
done


# Print help message.
if [[ "$HELP" == "1" ]]; then
  echo "Usage: ./setup.sh [options] [install|uninstall]"
  echo "Install or uninstall gve.sh."
  echo ""
  echo "Options:"
  echo "  -h, --help      Print this message and exit."
  echo "  -p, --prefix    Install prefix (default: ~/.local)."
  exit 0
fi


# Set up the environment.
mkdir -p "$PREFIX/bin"

# Install program.
if [[ "$ACTION" == "install" ]]; then
  echo "Compiling gve.sh ..."
  DOWNLOAD=0 RUN=0 ./main.sh
  echo "Installing gve.sh ..."
  mv a.out "$PREFIX/bin/gve"
  echo "Installed gve.sh to '$PREFIX/bin/gve'."
# Uninstall program.
elif [[ "$ACTION" == "uninstall" ]]; then
  echo "Uninstalling gve.sh ..."
  rm -f "$PREFIX/bin/gve"
  echo "Uninstalled gve.sh from '$PREFIX/bin/gve'."
fi
