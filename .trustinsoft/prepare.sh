#!/bin/bash
env
cat ~/.gitconfig
cat .git/config
git --version
echo $BASH_VERSION
cd ..
git pull --prune --recurse-submodules --jobs 8
git submodule update --init --recursive --jobs 8
SKIP_COMPILER=true CLEAN_BUILD=yes ./prepare_deps
