# C++ Search Engine
## Project Overview

This project implements a simplified **Search Server** core written in C++. The application is designed to simulate the core functionality of a search engine, focusing on efficient data indexing and query processing.

The key features include:
1.  **Indexing**: Creating an **Inverted Index** that maps words to the documents they appear in, enabling fast lookups.
2.  **Configuration**: Reading configuration settings, document paths, and query requests from JSON files (`config.json`, `requests.json`).
4.  **Output**: Writing the ranked search results to an output JSON file (`answers.json`).

## How to run the program

1. To run the program, intall the project then open with VS Code.
2. Make sure you have those extensions intalled:
    1. **C/C++ Extension Pack**
    2. **CMake Tools**
3. You can modify and change the path to config folder in converter.h (optional) 
4. Click build and run