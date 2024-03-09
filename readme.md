### Project Description

This compiler project provided a foundational understanding and hands-on experience in building the core components of a compiler for a simplified programming language. The project focuses on the development of two critical modules: the Lexical Analyzer and the Syntax Analyzer. These modules are essential for recognizing the lexical tokens and analyzing the syntactical structure of the language, respectively. Although the language specification includes advanced features like type checking and scope information, these are not directly implemented in the project. However, their constructs are recognized as valid tokens, and their structural validity is verified using grammar rules.

The project encompasses the implementation of syntax analysis, utilizing a top-down LL(1) parsing technique for parse tree generation and error recovery. The grammar, detailed in `Documents/Grammar/Grammar.md`, supports a dummy language designed for educational purposes. To ensure quality and robustness, Continuous Integration and Continuous Deployment (CI/CD) pipelines are set up for testing across six test cases (`Test cases`). These tests include both error-free and error-prone code scenarios.

The compiler, implemented in C, showcases efficient compilation of the testcase files in approximately 100 µs. To facilitate ease of use, the project follows a workflow similar to CMake. Users can compile and run the project by navigating to the `Group 01` directory, running `make`, and executing `./stage1exe "filename" "parsetreefilename"` to generate the parse tree in order traversal. For enhanced visualization, a Python script `network.py` can be executed on the serialized JSON output file. This script uses the Pyvis library to generate an interactive parse tree, offering a more intuitive understanding of the language's syntactical structure.


#### Replicating the project

1. **Prerequisites**: Ensure you have a C compiler and Python installed on your system. The project requires Python for visualization purposes.

2. **Clone the Repository**: Clone this project to your local machine to get started.

   ```
   git clone https://github.com/yashpandey474/CSF363-Compiler-Construction-Project.git
   ```

3. **Navigate to Project Directory**:

   ```
   cd Group\ 01/
   ```

4. **Compilation**: Compile the project using the `make` command.

   ```
   make
   ```

5. **Running the Compiler**:
   Execute the compiler by specifying the source file and the desired output file for the parse tree.

   ```
   ./stage1exe "filename" "parsetreefilename"
   ```

   This generates the parse tree in an inorder traversal format.

6. **Visualization**:
   To visualize the parse tree interactively, run the `network.py` script with the serialized JSON output file.

   ```
   python network.py
   ```

#### Contributing

Semantic Analysis and Intermediate Code Generation are currently in development. Do let us know if you're interested!

#### Support

If you encounter any issues or have questions, please file an issue on the project's GitHub page.

#### License

This project is licensed under the GNU AFFERO GENERAL PUBLIC LICENSE - see the LICENSE file for details.

---
