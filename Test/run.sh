# run shell for cygwin gcc
#!/bin/sh

function show_version {
echo "
*******************************************************************
      ****************    Agile Unit Testing ****************
*******************************************************************
Copyright 2015 Nokia

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License."
}

function exit_error {
    echo "
*******************************************************************
      ****************     Test Fail        ****************
*******************************************************************"
    exit 1
}

function exit_success {
    echo "
*******************************************************************
      ****************     Test Success     ****************
*******************************************************************"
    exit 0
}

function do_clean {
    echo "
*******************************************************************
      ****************          clean        ****************
*******************************************************************"
    make clean
}

function do_make {
    echo "
*******************************************************************
      ****************          make         ****************
*******************************************************************"
    make
    if [ ! -f AutoTest.exe ]; then
        echo
        echo "Abort abnormal: Make Fail!"
        exit_error
    fi
}

function do_run {
echo "
*******************************************************************
      ****************          run          ****************
*******************************************************************"
    ./AutoTest.exe
    if [ $? -ne 0 ]; then
        echo
        echo "Abort abnormal: Test Fail!"
        exit_error
    fi
}

function do_lcov {
    echo "
*******************************************************************
      ****************          lcov         ****************
*******************************************************************
"
    mkdir Report
    lcov --directory ./ --capture --output-file Report/AutoTest.info
    if [ $? -ne 0 ]; then
        echo 
        echo "Abort abnormal: Lcov Fail!"
        exit_error
    fi
}

function do_genhtml {    
    echo "
*******************************************************************
      ****************          genhtml      ****************
*******************************************************************"
    cd Report && genhtml AutoTest.info
    if [ $? -ne 0 ]; then
        echo 
        echo "Abort abnormal: genhtml Fail!"
        exit_error
    fi
}

function main { 
    show_version
    do_clean
    do_make
    do_run
    do_lcov
    do_genhtml
    exit_success
}

main
