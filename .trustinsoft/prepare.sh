#!/bin/bash
sudo apt-get update
sudo apt-get install -y lsb-release libtool
cd ..
git status
git submodule status --recursive
SKIP_COMPILER=true CLEAN_BUILD=yes ./prepare_deps
