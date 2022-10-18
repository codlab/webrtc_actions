#!/bin/bash

for repository in "." src ; do
    cd ${CI_PROJECT_DIR}/${repository} &&
        echo -ne "$(git remote get-url origin|cut -d'@' -f2) - " >> ${CI_PROJECT_DIR}/${META_FILE} &&
        echo -ne "export $(echo ${PWD##*/}|sed 's/-/_/g')=" >> ${CI_PROJECT_DIR}/${ENV_FILE} &&
        echo "$(git rev-parse HEAD|cut -c-12)" >> ${CI_PROJECT_DIR}/${META_FILE} >> ${CI_PROJECT_DIR}/${ENV_FILE} ; done

cd ${CI_PROJECT_DIR} && pwd

# Create name of artifact zip package
source ${CI_PROJECT_DIR}/${ENV_FILE}
if [[ $CI_JOB_NAME == *"release"* ]] ; then
  echo "This is release."
  export release_or_debug="release"
elif [[ $CI_JOB_NAME == *"debug"* ]] ; then
  echo "This is debug."
  export release_or_debug="debug"
fi

ARTIFACT_ZIP_PACKAGE="WebRTC_${src}-${release_or_debug}.zip"
echo "export ARTIFACT_ZIP_PACKAGE=${ARTIFACT_ZIP_PACKAGE}" >> ${CI_PROJECT_DIR}/${ENV_FILE}

echo "CAT ${ENV_FILE}" && cat ${ENV_FILE}
