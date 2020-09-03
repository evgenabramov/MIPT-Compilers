# MiniJava language compiler

> ### [Русская версия](README_ru.md)

## Requirements installation

Option for **Ubuntu** package manager:

```shell
sudo apt-get install arm-linux-gnueabihf qemu-user-static
```

--- 

## Example of work

### **Compiler build**

```shell
mkdir -p Compiler/build
cd Compiler/build
cmake ..
make
```

### **Compiler launch**

```shell
./Compiler --src ../examples/<example>
```

As a result, intermediate compilation files and assembly code are generated in the current *build* directory.

### **Assembly code execution (with ARM cross compiler)**

```shell
arm-linux-gnueabihf-gcc -static final_program.s -o final_program
./final_program
```

---

## Description of structure and stages of work

[main.cpp](Compiler/main.cpp) — processes the parameters passed to the compiler input.

[driver.cpp](Compiler/driver.cpp) — links the individual compiler components together.

### [Language grammar](Compiler/Grammar)

The grammar rules are described in the file [`parser.y`](Compiler/Grammar/parser.y).
Possible tokens are listed in - [`scanner.l`](Compiler/Grammar/scanner.l). Discover the [textual grammar description](Compiler/Grammar/README.md).

During compilation, to check the correctness of the written code and collect statistics, an abstract syntax tree (`AST`) is built, in which each node corresponds to a terminal or nonterminal symbol of the grammar.

In particular, the following classes are used:
- [`Declaration`](Compiler/Grammar/Declaration)
- [`Expression`](Compiler/Grammar/Expression)
- [`Statement`](Compiler/Grammar/Statement)
- [`NamedEntity`](Compiler/Grammar/NamedEntity) (the same as lvalue)
- [`Type`](Compiler/Grammar/Type)

[Grammar/Service](Compiler/Grammar/Service) - more abstract classes for linking code together.

The lexical analyzer is built with [Flex](https://linux.die.net/man/1/flex), the parser with [GNU Bison](https://www.gnu.org/software/bison/).

Several visitors for various purposes interact with the AST obtained at the first stage of the compiler's work: [`PrintVisitor`](Compiler/Visitors/PrintVisitor.hpp), [`SymbolTreeVisitor`](Compiler/Visitors/SymbolTreeVisitor.hpp), [`MethodCallVisitor`](Compiler/Visitors/MethodCallVisitor.h), [`TypeVisitor`](Compiler/Visitors/TypeVisitor.hpp) and [`IRTreeBuildVisitor`](Compiler/Visitors/IRTreeBuildVisitor.h). They are used for debugging, statistics collection, type checking, modeling stack frames for functions, and building an intermediate representation tree.

Inspect the result of `PrintVisitor` и `SymbolTreeVisitor` work:

```shell
cat PrintVisitor_output
cat SymbolTreeVisitor_output
```

### [Symbol Table](Compiler/SymbolTable)

`ScopeLayer` — stores information about named entities in the current scope (classes, methods, primitive types). Uses an abstract class [`MemberType`](Compiler/MemberType).

`ScopeLayerTree` — a nested-scoped tree that allows shadowing of variables and detects compilation errors such as declaring twice in the same scope and using without declaration.

An example of traversing this tree can be found in `SymbolTreeVisitor.hpp`.

`ClassStorage` — storage of fields and methods of classes in the program. Used with a generative pattern **Singleton**.

### [Function call mechanisms](Compiler/MethodMechanisms)

The Frame class provides access to objects in the current scope, return value, and parent frame.

`FunctionTable` - stores indexes on the stack for variables available in the current scope.

The FrameTranslator class simulates the state of a Frame during the execution of a function, namely:

- Defines stack space for:
    - Function arguments
    - local variables
- Stores information about:
    - The number of function arguments
    - Stack frame size
- By the name of a variable, allows you to get its address in the current scope. The address can be in a register or on the stack (see [Address](Compiler/MethodMechanisms/Address))

### [Intermediate representation tree (IR Tree)](Compiler/IRTree)

The intermediate representation tree allows you to translate the instructions of the source language to a low-level (for example, assembler) using a limited set of primitive operations. These operations correspond to the vertices of the constructed IR-tree:

- Expression
    - `BINOP` (binary operation)
    - `CALL` (function call)
    - `CONST` (constant)
    - `ESEQ` (sequential *statement* и value return)
    - `MEM` (access to memory)
    - `NAME` (label name in another operation)
    - `TEMP` (temporary variable)
- Statement
    - `EXP` (expression whose value is ignored)
    - `CJUMP` (conditional jump to label)
    - `LABEL` (label creation)
    - `MOVE` (assignment)
    - `SEQ` (sequential execution of *statements*)
- ExpressionList

**IR-Tree** is built from `AST` using the [`IRTreeBuildVisitor`](Compiler/Visitors/IRTreeBuildVisitor.h). For correct conversion of the types of the IR-tree vertices, the `SubtreeWrapper` wrapper is used.

To output and debug the built IR tree, there is [`PrintVisitor`](Compiler/IRTree/Visitors/PrintVisitor.h).

Before generating the low-level code, the constructed tree is brought to a canonical form by:

- Getting rid of double `CALL` when there is another `CALL` among the passed arguments ([`DoubleCallEliminateVisitor`](Compiler/IRTree/Visitors/DoubleCallEliminateVisitor.h))
- Raising the `ESEQ` vertices to the upper levels in the tree ([`ESEQEliminateVisitor`](Compiler/IRTree/Visitors/ESEQEliminateVisitor.h))
- Linearization - Right-handed `SEQ` arrangement ([`LinearizationVisitor`](Compiler/IRTree/Visitors/LinearizationVisitor.h))

Inspect IR tree after visitors work:

```shell
cat <class name>_<function name>_IRTree_raw
cat <class name>_<function name>_IRTree_without_double_call
cat <class name>_<function name>_IRTree_without_ESEQ
cat <class name>_<function name>_IRTree_linearized
```

Optimizations of various levels of locality can be applied to the resulting tree.

### [IR tree blocks](Compiler/IRTree/Blocks) 

IR-block is a set of instructions that must be executed sequentially, regardless of conditions and label transitions. Formally, each block begins with a label declaration and ends with a `JUMP-` or `CJUMP-`instruction.

This simplified representation of the program allows additional optimizations, in particular, to more efficiently assign registers to temporary variables during the code generation phase.

[`BlockBorderVisitor`](Compiler/IRTree/Visitors/BlockBorderVisitor.h) - is responsible for preparing the IR-tree for the construction of IR-blocks, namely:

- Closes an incomplete `JUMP` block before a label declaration
- Adds a transition to the function epilogue where the value is returned

Inspect the IR tree after transformation:

```shell
cat <class name>_<function_name>_IRTree_with_blocks
```

Building blocks is carried out by [`BlockBuildVisitor`](Compiler/IRTree/Visitors/BlockBuildVisitor.h).

Inspect the resulting [graph](Compiler/IRTree/Blocks/BlockGraph.hpp) из IR-блоков:

```shell
cat <class name>_<function name>_IRTree_blocks
```

For additional optimizations, it is customary to combine several IR blocks into disjoint traces ([Trace](Compiler/IRTree/Blocks/Trace.hpp)).

Inspect block distribution by traces:

```shell
cat <class name>_<function name>_IRTree_traces
```

### [Code generation (translation into assembler)](Compiler/IRTree/Visitors/CodeGenerationVisitor.h)

Translation into *ARM Assembly* is carried out by visitor [`CodeGenerationVisitor`](Compiler/IRTree/Visitors/CodeGenerationVisitor.h).

To get the assembly code optimal in terms of the number of instructions and intermediate registers, the IR-tree must be covered with non-intersecting patterns consisting of its vertices that form a subtree. Each pattern must be examined manually by matching it with a suitable set of instructions.

There are two algorithms for covering the IR tree with patterns: **Maximal munch** and **dynamic**. In this project Maximal munch is implemented. It assumes a recursive traversal of the tree from the root to the leaves, when patterns are sequentially checked at the current vertex from the largest number of vertices to the smallest. If the next pattern matches, the corresponding instruction is added to the general list after recursive traversal of the child vertices.

Registers at this stage are replaced by `Temporary`-variables, i.e. it is considered that there are an infinite number of virtual registers available.

Inspect the `CodeGenerationVisitor` work result:

```shell
cat <class name>_<function name>.s
```

### [Register allocation](Compiler/IRTree/Instructions)

In the [instructions](Compiler/IRTree/Instructions/Instruction.hpp) received after generating the code, registers, the values of which affect the result, and registers, the values of which change, are taken into account.

To translate virtual registers into physical registers, it is necessary to select pairs of virtual registers that cannot be assigned the same physical register (build a conflict graph - [`InterferenceGraph`](Compiler/IRTree/Instructions/InterferenceGraph.hpp)). [`ControlFlowGraph`](Compiler/IRTree/Instructions/ControlFlowGraph.hpp) is preliminarily constructed, in which a separate vertex corresponds to each instruction, and two instructions are connected by an edge, if after the first one under some execution (depending on conditions) the second one can follow. An algorithm is applied to it to search for *live* variables on the edges (**Liveness analysis**, the variable is considered to be *live* on the edges between its definition and the usage of this value). During this algorithm all vertices are iterated over several times in the reverse order of the topological sorting. This iteration solves the **Dataflow equations**.

Now the original problem is equivalent to the problem of coloring the conflict graph, when the connected vertices cannot be painted in the same color. Given the number of registers available, `InterferenceGraph` is consistently simplified by excluding correctly matched vertices. These vertices can subsequently be greedily assigned colors different from the colors of their neighbors.

Inspect the assembly code of the functions after assigning the physical registers (`<iteration>` here is the iteration number of the painting algorithm):

```shell
cat <class name>_<function name><iteration>.s
```

### [Complete program code](Compiler/driver.cpp)

A prologue and an epilogue are added to the instructions for each function to follow the *calling conventions*.

At the beginning of the function:

- *Frame Pointer* value is saved
- *Stack Pointer* is stored into *Frame Pointer*
- Stack memory is allocated for arguments and local variable
- The values of the arguments passed in registers are pushed onto the stack
- *Callee-save* registers are saved

At the end of the function:

- Return value is stored into `r0` register 
- *Callee-save* registers are restored
- *Stack Pointer* value is restored
- *Frame pointer* value is restored
- *Link register* is used to return to caller

For debugging and validation, I used [Compiler Explorer](https://godbolt.org).

Also a `.data`-label is added for the correct operation of the `printf` function from the C library.

After that, in [driver.cpp](Compiler/driver.cpp), the code of all functions is written to a common file.

Inspect the program code that can be run through **gcc**:

```shell
cat final_program.s
```

---

## Used teaching materials

- Lectures and seminars of **"Programming languages and compilation theory"** course in the 4th semester at the Department of Innovations and High Technologies of MIPT (as part of the PSAMI MIPT)
    
    Lecturer — [@akhtyamovpavel](https://github.com/akhtyamovpavel) 

- Andrew W. Appel, **Modern Compiler Implementation in Java**, Cambridge University Press

---

## Directions for further development of the project

- [ ] Add support for processors with *CISC* architecture (**x86-64 Assembly** code generation) 
- [ ] Check for memory leaks
- [ ] Try to resolve Shift/Reduce grammar conflicts
- [ ] Complete `TODO`-features in the code
- [ ] Add tests
- [ ] Set up CI with running tests and sample programs
- [ ] Make syntax highlighting for popular code editor