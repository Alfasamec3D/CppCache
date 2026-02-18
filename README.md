# LFU Cache

This program simulates LFU cache.

## How to build

1. Copy the repository
2. Go to the source tree directory (the one containing `CMakeLists.txt` file)
3. Generate a project buildsystem, then build the project through terminal. On linux just use

   `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build ./build`

   command.
   
5. Wait till the building is finished.

## How to test

1. Go to the build tree directory
2. In terminal use `make test` command.
3. See the results of the test.

## How to run

1. In the same directory enter `./main` command.
2. Enjoy the program.

## How to run Ideal cache

1. In the same directory enter `./cacheIdeal` command.
2. Enjoy the program.
