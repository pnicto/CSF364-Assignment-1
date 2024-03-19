# DAA Assignment 1

# Development

## Setup

-   Install [Visual Studio Code](https://code.visualstudio.com/). You are free to use other text editors and IDEs but you will be on your own for the intellisense setup.
-   Install raylib dependencies for your operating system following the wiki [here](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).
-   Install [emscripten](https://emscripten.org/docs/getting_started/downloads.html) in the `external` directory.
    ```bash
        # run these commands in the project root
        cd external
        git clone https://github.com/emscripten-core/emsdk.git
        cd emsdk
        # this is optional
        # rm -rf .git
        ./emsdk install latest
        ./emsdk activate latest # it shows you some information on updating the PATH either follow them or do it on your own
        # verify the installation
        emcc --check
    ```
-   In `VSCode`, in the command center(<kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>P</kbd>), choose the `Select configuration` option given by the `C/C++` extension by `Microsoft`. Choose the `raylib web` option.
-   There should be no errors when you open the project's `cpp` files in `VSCode`.
-   [Optional] Install `live-server` using `pnpm i`.
-   Test your setup by running both the commands. Note that both commands should run successfully, especially the first one should not fail if you have setup everything correctly.

```bash
# building web
make PLATFORM=PLATFORM_WEB run

# building desktop PLATFORM=PLATFORM_DESKTOP is the default
make run
```

## Workflow

-   `main.cpp` is the root of the project.
-   `src` and `include` is where most code is going to be written in.
-   After making changes to the code, you have to run the `make` command with appropriate arguments.
-   You can use the experimental utility script `run` to run make on **saving** files. For which you will have to install `entr`. You can probably do this using your package manager.
-   **Note that the utility script runs `make`(i.e., it compiles the project using `make`) for you but it does not run the build.**

### Desktop

```bash
make run # or make run PLATFORM=PLATFORM_DESKTOP

# Alternatively, you can build first and run the binary yourself
make PLATFORM=PLATFORM_DESKTOP
./build/convex_hull
```

### Web

#### Without using live server

Whenever you make changes and build, you have to reload the html page for the changes to take effect. If the output is still not what you expected you can try <kbd>Ctrl</kbd> + <kbd>F5</kbd> to reload without cache.

```bash
make PLATFORM=PLATFORM_WEB run # this will use emrun to open the page on your browser

# Alternatively, you can build first and open the html file on your own.
make PLATFORM=PLATFORM_WEB
emrun ./build/convex_hull.html

```

#### With live server

You can read about the flags [here](https://www.npmjs.com/package/live-server). When using `live-server`, you just have to create the build using `make`, whenever live-server detects changes it will reload the page for you.

```bash
pnpm live-server build --entry-file=convex_hull.html --wait=500 -q

# run this another terminal window when you make changes
make PLATFORM=PLATFORM_WEB
```

```bash
# using the experimental `run` script
# with this setup whenever you save changes on your editor, the page will reload.
# Note that with autosave it might trigger unnecessary reloads so it's your call whether you want to use this or not.

# run the script in one window
./run PLATFORM_WEB

# in another window run the live-server
pnpm live-server build --entry-file=convex_hull.html --wait=500 -q
```
