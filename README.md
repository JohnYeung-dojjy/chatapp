# Simple chatapp in C++

Single Server Multi-client model

## Preparation

### Install QT

```sh
sudo apt install sudo apt install qtbase5-dev
```

### Add QT intellisense to vscode (Optional)

Create the file .vscode/c_cpp_properties.json:

You can locate the install qt path using the command `dpkg -L qtbase5-dev | grep QtCore`

```json
{
  "configurations": [
    {
      "name": "Linux",
      "includePath": [
        "/usr/include",
        "/usr/include/x86_64-linux-gnu",
        "/{qt installed path}/qt5",
        "/{qt installed path}/QtWidgets",
        "/{qt installed path}/QtCore",
        "/{qt installed path}/QtGui"
      ],
      "defines": [
        "QT_WIDGETS_LIB"
      ],
      "compilerPath": "/usr/bin/g++",
      "cStandard": "c11",
      "cppStandard": "c++11",
      "intelliSenseMode": "linux-gcc-x64"
    }
  ],
  "version": 4
}
```

## Build and run

### Using vscode command (Ctrl+Shift+B)

Create .vscode/tasks.json

```json
{
    "version": "2.0.0",
    "tasks": [
      {
        "label": "build",
        "type": "shell",
        "command": "cmake --build build",
        "group": "build",
        "problemMatcher": []
      }
    ]
  }
```

### Using terminal

```sh
mkdir build
cd build
cmake ..
make
./MyQtApp
```
