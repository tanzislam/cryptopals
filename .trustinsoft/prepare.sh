#!/bin/bash
sudo apt install lsb-release
cd ..
SKIP_COMPILER=true CLEAN_BUILD=yes ./prepare_deps
