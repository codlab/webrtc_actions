#!/bin/bash

patch_dir="$(dirname "$0")"

# for instance bash android.sh ${CI_PROJECT_DIR}/src
out_src_dir=$1


cp $patch_dir/build_config_cpp_cpp.gni $out_src_dir/build/config/c++/c++.gni

