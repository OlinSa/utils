#!/bin/sh

if [ $# -eq  1 ] ; then
	CURRENT_PATH=$1
else
	CURRENT_PATH="."
fi

echo ${CURRENT_PATH}

googletest_dir=${CURRENT_PATH}"/googletest-release-1.8.1"
googletest_package=${googletest_dir}".tar.gz"
googletest_build=${googletest_dir}"/googletest/build"

if [ ! -d ${googletest_dir} ];then
        tar -xzvf ${googletest_package} -C ${CURRENT_PATH}
	mkdir ${googletest_build}
fi

cd ${googletest_build} && cmake ../ && make && cd -
