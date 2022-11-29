BUNDLE_NAME=${ARTIFACT_ZIP_PACKAGE%.zip}
output=${CI_PROJECT_DIR}/${BUNDLE_NAME}
src=${CI_PROJECT_DIR}/src
echo "src used: ${src}"
echo "$0 output dir: ${output}"
mkdir -p "${output}"
cd "${output}" && echo "Working in $(pwd)"

if [[ $CI_JOB_NAME == *"release"* ]] ; then
  echo "This is release build."
  export release_or_debug="Release"
elif [[ $CI_JOB_NAME == *"debug"* ]] ; then
  echo "This is debug build."
  export release_or_debug="Debug"
fi

#######################
# creating the bundle #
#######################

declare -a arch_arr=("x86_64" "x86" "arm64-v8a" "armeabi-v7a")
for arch in "${arch_arr[@]}"
do
  echo "create path ${arch}. Copying from ${src} into ${output}"
  mkdir -p "${output}/src/out/${arch}/${release_or_debug}/obj/"
  cp "${src}/out/${arch}/${release_or_debug}/obj/libwebrtc.a" "${output}/src/out/${arch}/${release_or_debug}/obj/"

  mkdir -p "${output}/src/out/${arch}/${release_or_debug}/gen/"

  #rtc event log
  mkdir -p "${output}/src/out/${arch}/${release_or_debug}/gen/logging/rtc_event_log"
  cp -r "${src}/out/${arch}/${release_or_debug}/gen/logging/rtc_event_log" "${output}/src/out/${arch}/${release_or_debug}/gen/logging/"

  #modules
  mkdir -p "${output}/src/out/${arch}/${release_or_debug}/gen/modules/audio_coding/audio_network_adaptor"
  mkdir -p "${output}/src/out/${arch}/${release_or_debug}/gen/modules/audio_processing"
  cp -r "${src}/out/${arch}/${release_or_debug}/gen/modules/audio_coding/audio_network_adaptor" "${output}/src/out/${arch}/${release_or_debug}/gen/modules/audio_coding/"
  cp -r "${src}/out/${arch}/${release_or_debug}/gen/modules/audio_processing" "${output}/src/out/${arch}/${release_or_debug}/gen/modules/"

  #generated
  mkdir -p "${output}/src/out/${arch}/${release_or_debug}/gen/sdk/android"
  cp -r "${src}/out/${arch}/${release_or_debug}/gen/sdk/android" "${output}/src/out/${arch}/${release_or_debug}/gen/sdk/"
  mkdir "${output}/libwebrtc"
  zip -ryq "${output}/libwebrtc/${arch}.zip" "${output}/src/out/${arch}"
done

declare -a old_java_paths=("modules/audio_device/android/java/src" "rtc_base/java/src" "sdk/android/api" "sdk/android/src/java")
for path in "${old_java_paths[@]}"
do
  echo "erase old java path: ${path}"
  if [[ ! -z "$output" ]] ; then
    rm -rf "${output}/src/${path}"
  else
    echo "Empty output variable, not executing rm -rf"
  fi
done

mkdir src/cicd
mkdir -p "${output}/src/modules/audio_device/android/java/src"
mkdir -p "${output}/src/rtc_base/java/src"
mkdir -p "${output}/src/sdk/android/api"
mkdir -p "${output}/src/sdk/android/src/java"
mkdir -p "${output}/src/sdk/android/src/jni"
mkdir -p "${output}/src/api/"
mkdir -p "${output}/src/third_party/abseil-cpp/absl"
mkdir -p "${output}/src/third_party/libyuv"
mkdir -p "${output}/src/third_party/libvpx"
mkdir -p "${output}/src/modules"
mkdir -p "${output}/src/common_video/include"
mkdir -p "${output}/src/common_video/libyuv"
mkdir -p "${output}/src/common_video/h264"
mkdir -p "${output}/src/common_audio"
mkdir -p "${output}/src/media/base"
mkdir -p "${output}/src/sdk/android/native_api"
mkdir -p "${output}/src/logging"
mkdir -p "${output}/src/call"
mkdir -p "${output}/src/p2p"
mkdir -p "${output}/src/pc"
mkdir -p "${output}/src/system_wrappers"
mkdir -p "${output}/src/audio"
mkdir -p "${output}/src/base"
mkdir -p "${output}/src/media"
mkdir -p "${output}/src/dvc"
mkdir -p "${output}/src/build"
mkdir -p "${output}/src/third_party/libvpx/source/libvpx/vpx/internal"
mkdir -p "${output}/src/third_party/libvpx/source/libvpx/vpx/src"

cp "${src}/build/buildflag.h" "${output}/src/build/"
cp "${src}/build/build_config.h" "${output}/src/build/"
cp "${src}/common_types.h" "${output}/src/"
cp -r "${src}/dvc" "${output}/src/"
cp -r "${src}/media" "${output}/src/"
cp -r "${src}/base" "${output}/src/"
cp -r "${src}/common_audio" "${output}/src/"
cp -r "${src}/audio" "${output}/src/"
cp -r "${src}/system_wrappers" "${output}/src/"
cp -r "${src}/p2p" "${output}/src/"
cp -r "${src}/pc" "${output}/src/"
cp -r "${src}/call" "${output}/src/"
cp -r "${src}/logging" "${output}/src/"
cp -r "${src}/sdk/android/native_api" "${output}/src/sdk/android/"
cp -r "${src}/media/base" "${output}/src/media/"
cp -r "${src}/common_video" "${output}/src/"
cp -r "${src}/modules" "${output}/src/"
cp -r "${src}/third_party/abseil-cpp/absl" "${output}/src/third_party/abseil-cpp/"
cp -r "${src}/third_party/libyuv" "${output}/src/third_party/"
cp -r "${src}/third_party/libvpx/source/libvpx/vpx/internal" "${output}/src/third_party/libvpx/source/libvpx/vpx/"
cp -r "${src}/third_party/libvpx/source/libvpx/vpx/src" "${output}/src/third_party/libvpx/source/libvpx/vpx/"
cp -r ${src}/third_party/libvpx/source/libvpx/vpx/*.h "${output}/src/third_party/libvpx/source/libvpx/vpx/"
if [[ ! -z "$output" ]] ; then
  rm -rf "${output}/src/third_party/libvpx/.git"
else
  echo "Empty output variable, not executing rm -rf"
fi
cp -r "${src}/api" "${output}/src/"
cp -r "${src}/rtc_base" "${output}/src/"
cp -r "${src}/sdk/android/src/jni" "${output}/src/sdk/android/src/"
cp -r ${src}/sdk/*.cc "${output}/src/sdk/"
cp -r ${src}/sdk/*.h "${output}/src/sdk/"
cp -r "${src}/rtc_base/java/src" "${output}/src/rtc_base/java/"
cp -r "${src}/sdk/android/api" "${output}/src/sdk/android/"
cp -r "${src}/sdk/android/src/java" "${output}/src/sdk/android/src/"

# create bundle
cd "${CI_PROJECT_DIR}"
zip -ryq "${ARTIFACT_ZIP_PACKAGE}" ${ARTIFACT_ZIP_PACKAGE%.zip}

# create checksum
sha256sum "${ARTIFACT_ZIP_PACKAGE}" > $ARTIFACT_ZIP_PACKAGE.sha256

