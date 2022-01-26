#!/bin/bash
sudo apt update
sudo apt install -y lsb-release libtool
cd ..
git fetch --prune --recurse-submodules=yes
git submodule update --init --recursive
SKIP_COMPILER=true CLEAN_BUILD=yes ./prepare_deps
