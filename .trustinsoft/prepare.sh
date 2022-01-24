#!/bin/bash
sudo apt install -y lsb-release
cd ..
SKIP_COMPILER=true CLEAN_BUILD=yes ./prepare_deps
