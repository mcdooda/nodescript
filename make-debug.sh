#!/bin/sh
mkdir -p Debug/ && cd Debug/ && cmake -DCMAKE_BUILD_TYPE=Debug .. -G "Unix Makefiles" && make
