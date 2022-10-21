#!/bin/bash

#get prefixes for package name
if [[ $CI_JOB_NAME == *"android_release"* ]] ; then
    echo "This is android_release flow."
    export release_or_debug="release"
    export voxeet_s3_prefix="/android/snapshots/"

elif [[ $CI_JOB_NAME == *"android_debug"* ]] ; then
    echo "This is android_debug flow."
    export release_or_debug="debug"
    export voxeet_s3_prefix="/android/snapshots/"

elif [[ $CI_JOB_NAME == *"ios_debug"* ]] ; then
    echo "This is ios_debug flow."
    export release_or_debug="debug"
    export voxeet_s3_prefix="/ios/snapshots/"

elif [[ $CI_JOB_NAME == *"ios_release"* ]] ; then
    echo "This is ios_release flow."
    export release_or_debug="release"
    export voxeet_s3_prefix="/ios/snapshots/"

elif [[ $CI_JOB_NAME == *"linux64_debug"* ]] ; then
    echo "This is linux64_debug flow."
    export release_or_debug="debug"
    export voxeet_s3_prefix="/linux64/snapshots/"

elif [[ $CI_JOB_NAME == *"linux64_release"* ]] ; then
    echo "This is linux64_release flow."
    export release_or_debug="release"
    export voxeet_s3_prefix="/linux64/snapshots/"

elif [[ $CI_JOB_NAME == *"macos64_debug"* ]] ; then
    echo "This is macos64_debug flow."
    export release_or_debug="debug"
    export voxeet_s3_prefix="/macos64/snapshots/"

elif [[ $CI_JOB_NAME == *"macos64_release"* ]] ; then
    echo "This is macos64_release flow."
    export release_or_debug="release"
    export voxeet_s3_prefix="/macos64/snapshots/"

elif [[ $CI_JOB_NAME == *"MR_test"* ]] ; then
    echo "This is merge request test flow. No package check is needed. Build is continued."
    exit 0
fi

# The same as in ./cicd/create_meta_file.sh, in the name we are placing 12-char sha
COMMIT_SHA_12=$(echo ${CI_COMMIT_SHA}|cut -c-12)
echo "Seeking package for rev: ${COMMIT_SHA_12} and ${release_or_debug} flavour."
echo "S3 bucket: ${VOX_WEBRTC_S3_ROOT_URL}${voxeet_s3_prefix}"

#lets redirect ls of s3 bucket to iterate over output lines and filter output
aws s3 ls "${VOX_WEBRTC_S3_ROOT_URL}${voxeet_s3_prefix}" | grep 'WebRTC_' > aws_ls_s3_value

for line_ in $(cat aws_ls_s3_value | grep "WebRTC_"); do
  if [[ ${line_} == *"WebRTC_${COMMIT_SHA_12}"*  ]] && [[ ${line_} == *"${release_or_debug}"*  ]]; then
    echo "${line_} already exists: ${VOX_WEBRTC_S3_ROOT_URL}${voxeet_s3_prefix}${line_}"
    exit 1
  fi
done

echo "WebRTC package for rev: [${COMMIT_SHA_12}] and flavour: [${release_or_debug}] deos not exist on ${VOX_WEBRTC_S3_ROOT_URL}${voxeet_s3_prefix} yet."
echo "Build is continued."
