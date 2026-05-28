#!/bin/bash

# If you need your LSP to work, run this

set -e

rm -rf .cache/ compile_commands.json
make clean
bear -- make
