#!/bin/bash
sudo apt install redhat-lsb
cd ..
SKIP_COMPILER=true CLEAN_BUILD=yes ./prepare_deps
