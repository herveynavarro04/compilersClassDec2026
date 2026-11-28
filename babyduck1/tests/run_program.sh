#!/usr/bin/env bash
# Usage: run_program.sh <babyduck> <vm.py> <source.bd> <output.obj> [stdin-string]
# Compiles the BabyDuck source to an .obj file (silently), then runs it with
# the Python VM. If a 5th argument is given, it is fed to the VM on stdin
# (interpreting \n as a newline). Propagates the exit code of whichever
# stage fails.
set -o pipefail

if [ "$#" -lt 4 ]; then
  echo "usage: $0 <babyduck> <vm.py> <source.bd> <output.obj> [stdin]" >&2
  exit 2
fi

BABYDUCK="$1"
VM="$2"
SRC="$3"
OBJ="$4"
INPUT="${5-}"

mkdir -p "$(dirname "$OBJ")"

if ! "$BABYDUCK" "$SRC" -o "$OBJ" > /dev/null; then
  rc=$?
  echo "compiler failed with exit $rc" >&2
  exit "$rc"
fi

if [ -n "$INPUT" ]; then
  printf '%b' "$INPUT" | python3 "$VM" "$OBJ"
else
  python3 "$VM" "$OBJ"
fi
