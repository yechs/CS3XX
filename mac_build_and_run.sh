if [ "$#" -eq 0  ] || ! [ -f "$1" ]; then
    echo "build and   run in   debug mode: [35m./mac_build_and_run.bat filename.cpp [0m"
    echo "build and   run in release mode: [35m./mac_build_and_run.bat filename.cpp  --release[0m"
    echo "build and debug in      VS Code: [35m./mac_build_and_run.bat filename.cpp  --debug-vscode[0m"
else
    if [ -f "executable.exe" ]; then
        rm executable.exe
    fi

    clear

    OPTARG=0
    if [ "$2" = "--release" ]; then
        echo "[35m[cow] building $1 in release mode[0m"
        OPTARG=3
    else
        echo "[35m[cow] building $1 in debug mode[0m"
    fi

    clang++ \
        $1 \
        -o executable.exe \
        -std=c++11 \
        -fno-strict-aliasing \
        -O$OPTARG \
        -g \
        -Wall -Wextra \
        -Wshadow \
        -Werror=vla \
        -Wno-deprecated-declarations -Wno-missing-braces -Wno-missing-field-initializers -Wno-char-subscripts -Wno-writable-strings \
        -I./codebase/ext -I./codebase/ext \
        -L./codebase/ext \
        -lglfw3 \
        -framework Cocoa -framework OpenGL -framework IOKit \
        -framework AudioUnit -framework CoreAudio -framework AudioToolbox

    if [ -f "executable.exe" ]; then
        if [ "$2" = "--debug-vscode" ]; then
            echo "[35m[cow] TODO: debugging $1 in Visual Studio Code[0m"
            source _xplat_debug_vscode.bat
        else
            echo "[35m[cow] running executable.exe[0m"
            ./executable.exe

            rm -r executable.exe.dSYM
        fi
    fi
fi
