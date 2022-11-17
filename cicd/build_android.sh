#!/usr/bin/env bash
set -e
set -x

cd ${CI_PROJECT_DIR}/src

debug_params="target_os=\"android\" is_debug=true symbol_level=2 android_full_debug=true use_rtti=true"

if [[ $CI_JOB_NAME == *"release"* ]] ; then
  echo "compiling release..."
  gn gen out/arm64-v8a/Release --args="target_os=\"android\" target_cpu=\"arm64\" is_debug=false use_rtti=true"
  gn gen out/armeabi-v7a/Release --args="target_os=\"android\" target_cpu=\"arm\" is_debug=false use_rtti=true"
  gn gen out/x86/Release --args="target_os=\"android\" target_cpu=\"x86\" is_debug=false use_rtti=true"
  gn gen out/x86_64/Release --args="target_os=\"android\" target_cpu=\"x64\" is_debug=false use_rtti=true"
  ninja -C out/armeabi-v7a/Release webrtc metrics
  ninja -C out/arm64-v8a/Release webrtc metrics
  ninja -C out/x86/Release webrtc metrics
  ninja -C out/x86_64/Release webrtc metrics
elif [[ $CI_JOB_NAME == *"debug"* ]] ; then
  echo "compiling debug..."
  gn gen out/arm64-v8a/Debug --args="target_cpu=\"arm64\" ${debug_params}"
  gn gen out/armeabi-v7a/Debug --args="target_cpu=\"arm\" ${debug_params}"
  gn gen out/x86/Debug --args="target_cpu=\"x86\" ${debug_params}"
  gn gen out/x86_64/Debug --args="target_cpu=\"x64\" ${debug_params}"
  ninja -C out/armeabi-v7a/Debug webrtc
  ninja -C out/arm64-v8a/Debug webrtc
  ninja -C out/x86/Debug webrtc
  ninja -C out/x86_64/Debug webrtc
fi

