
# in src folder

CURRENT=`pwd`

cat patches/build_config_cpp_cpp.gni >> build/config/c++/c++.gni
cat build/config/c++/c++.gni

# Expose unwind to prevent symbol issues
pushd buildtools/
git apply $CURRENT/patches/buildtools.patch
git status
popd

pushd build/config/
git apply $CURRENT/patches/src_build_config.patch
git status
popd
