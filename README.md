# Advent of Code in modern C++

Advent of Code ( https://adventofcode.com ) 2015-2018 solutions using c++-17 with wide usage of boost and ranges-v3

Prerequisites
-----

* C++17 compiler ( tested with gcc 8.2.1, clang 7.0.1 )
* cmake 3.13 or later
* git
* bash

Build
-----

    $ git clone https://github.com/voivoid/advent-of-code/
    $ mkdir advent-of-code-build
    $ cd advent-of-code-build
    $ cmake -DCMAKE_BUILD_TYPE=RELEASE ../advent-of-code
    $ cmake --build . -- -j4


During the build several thirdparty libraries ( boost, rangev3, json for modern c++ ) will be downloaded in the build directory

Tests
-----

    $ ctest -j4

Solve Problem
-----

    $ ./app/AocApp 2015_01_1 < ../advent-of-code/tests/input/2015_01
    280
    $ ./app/AocApp 2015_01_2 < ../advent-of-code/tests/input/2015_01
    1797

Sources
-----

Day | 2015 | 2016 | 2017 | 2018 |
--- | ---- | ---- | ---- | ---- |
01  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_01.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2016/problem_01.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2017/problem_01.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2018/problem_01.cpp)     |
02  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_02.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2016/problem_02.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2017/problem_02.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2018/problem_02.cpp)     |
03  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_03.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2016/problem_03.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2017/problem_03.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2018/problem_03.cpp)     |
04  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_04.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2016/problem_04.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2017/problem_04.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2018/problem_04.cpp)     |
05  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_05.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2016/problem_05.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2017/problem_05.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2018/problem_05.cpp)     |
06  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_06.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2016/problem_06.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2017/problem_06.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2018/problem_06.cpp)     |
07  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_07.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2016/problem_07.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2017/problem_07.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2018/problem_07.cpp)     |
08  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_08.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2016/problem_08.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2017/problem_08.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2018/problem_08.cpp)     |
09  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_09.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2016/problem_09.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2017/problem_09.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2018/problem_09.cpp)     |
10  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_10.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2016/problem_10.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2017/problem_10.cpp)     |      |
11  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_11.cpp)     |      | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2017/problem_11.cpp)     | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2018/problem_11.cpp)     |
12  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_12.cpp)     |      |      |      |
13  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_13.cpp)     |      |      |      |
14  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_14.cpp)     |      |      |      |
15  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_15.cpp)     |      |      |      |
16  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_16.cpp)     |      |      |      |
17  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_17.cpp)     |      |      |      |
18  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_18.cpp)     |      |      |      |
19  |      |      |      |      |
20  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_20.cpp)     |      |      |      |
21  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_21.cpp)     |      |      |      |
22  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_22.cpp)     |      |      |      |
23  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_23.cpp)     |      |      |      |
24  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_24.cpp)     |      |      |      |
25  | [✓](https://github.com/voivoid/advent-of-code/blob/master/problems/src/2015/problem_25.cpp)     |      |      |      |
