#!/usr/bin/env bash
# make_docs.sh - generate Doxygen HTML + PDF docs for the project in the CURRENT directory.
#
# Usage:
#   cd /path/to/your/project
#   /path/to/make_docs.sh <project_name> [src_dir] [header_dir]
#
# Examples:
#   cd /home/workspace/source/contentScanner_BEFORE_LIBCONTENTSCAN
#
#   doxygen.sh myProject src include     # custom folder names
#
#
#   ./doxygen.sh contentScanner -> This is to run will get docs/contentScanner.pdf

set -euo pipefail

PROJECT_NAME="${1:-}"
SRC_DIR="${2:-source}"
HDR_DIR="${3:-header}"
OUT_DIR="docs"

if [ -z "$PROJECT_NAME" ]; then
    echo "Usage: $0 <project_name> [src_dir] [header_dir]"
    echo "Run it from inside the project directory."
    exit 1
fi

PROJECT_DIR="$(pwd)"
CONFIG="${PROJECT_NAME}.doxygen"

SUDO=""
[ "$(id -u)" -ne 0 ] && SUDO="sudo"

log() { echo -e "\n=== $* ==="; }

# ---------- 1. Install missing tools ----------
log "Checking required tools"
NEED_PKGS=()
command -v doxygen  >/dev/null 2>&1 || NEED_PKGS+=(doxygen)
command -v dot      >/dev/null 2>&1 || NEED_PKGS+=(graphviz)
command -v fc-cache >/dev/null 2>&1 || NEED_PKGS+=(fontconfig)
if ! command -v pdflatex >/dev/null 2>&1; then
    NEED_PKGS+=(texlive-latex-base texlive-latex-extra texlive-latex-recommended texlive-fonts-recommended)
fi

if [ "${#NEED_PKGS[@]}" -gt 0 ]; then
    echo "Installing: ${NEED_PKGS[*]}"
    $SUDO apt-get update
    $SUDO apt-get install -y "${NEED_PKGS[@]}"
else
    echo "All tools already installed."
fi

# ---------- 2. Check project folders ----------
log "Project: $PROJECT_NAME"
echo "Directory: $PROJECT_DIR"
[ -d "$SRC_DIR" ] || { echo "ERROR: '$SRC_DIR' folder not found in $PROJECT_DIR"; exit 1; }
[ -d "$HDR_DIR" ] || { echo "ERROR: '$HDR_DIR' folder not found in $PROJECT_DIR"; exit 1; }

# ---------- 3. Generate config ----------
log "Generating $CONFIG"
rm -f "$CONFIG"
doxygen -g "$CONFIG" >/dev/null

set_opt() {   # set_opt KEY VALUE
    sed -i -E "s|^($1[[:space:]]*=).*|\1 $2|" "$CONFIG"
}

set_opt PROJECT_NAME        "\"$PROJECT_NAME\""
set_opt OUTPUT_DIRECTORY    "$OUT_DIR"
set_opt INPUT               "$SRC_DIR $HDR_DIR"
set_opt FILE_PATTERNS       "*.cpp *.h"
set_opt RECURSIVE           "NO"
set_opt EXTRACT_ALL         "YES"
set_opt EXTRACT_STATIC      "YES"
set_opt SOURCE_BROWSER      "YES"
set_opt GENERATE_HTML       "YES"
set_opt GENERATE_LATEX      "YES"
set_opt LATEX_BATCHMODE     "YES"
set_opt HAVE_DOT            "YES"
set_opt CALL_GRAPH          "YES"
set_opt CALLER_GRAPH        "YES"
set_opt DOT_GRAPH_MAX_NODES "100"
set_opt LATEX_SOURCE_CODE   "NO"
set_opt INLINE_SOURCES      "NO"

# ---------- 4. Run doxygen ----------
log "Running doxygen"
rm -rf "$OUT_DIR"
doxygen "$CONFIG"

# ---------- 5. Build PDF ----------
log "Building PDF"
if make -C "$OUT_DIR/latex"; then
    cp "$OUT_DIR/latex/refman.pdf" "$OUT_DIR/${PROJECT_NAME}.pdf"
    echo "PDF : $PROJECT_DIR/$OUT_DIR/${PROJECT_NAME}.pdf"
else
    echo "WARNING: PDF build failed. HTML docs are still available."
    echo "Debug with: cd $PROJECT_DIR/$OUT_DIR/latex && pdflatex -interaction=nonstopmode refman"
fi

# ---------- 6. Summary ----------
log "Done"
echo "HTML: $PROJECT_DIR/$OUT_DIR/html/index.html"
