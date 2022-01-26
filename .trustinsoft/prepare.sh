#!/bin/bash
sudo apt-get update
sudo apt-get install -y lsb-release libtool
cd ..
SKIP_COMPILER=true CLEAN_BUILD=yes ./prepare_deps
