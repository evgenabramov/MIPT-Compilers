## Описание сборки и запуска

```
mkdir build
cd build
cmake ..
make
./Compiler --src ../examples/example[1-6].in
```

Посмотреть на выхлоп работы визиторов (2-3 чекпоинты):
> Deprecated, см. в соответствующих ветках

```
cat PrintVisitor_output
cat Interpreter_output
```

4 чекпоинт:
```
cat PrintVisitor_output
cat SymbolTreeVisitor_output
```

5-му чекпоинту соответствуют примеры 5 и 6.

Выхлоп `MethodCallVisitor`'a выводится в консоль. 

6 чекпоинт (на тех же примерах):

```
cat <class name>::<function name>_IRTreePrintVisitor_output
```

7 чекпоинт (на тех же примерах - они усложнены):
```
cat <class name>::<function name>_IRTree_raw
cat <class name>::<function name>_IRTree_without_double_call
cat <class name>::<function name>_IRTree_without_ESEQ
cat <class name>::<function name>_IRTree_linearized
```

8 чекпоинт:
```
cat <class name>::<function name>_IRTree_with_blocks
cat <class name>::<function name>_IRTree_blocks
cat <class name>::<function name>_IRTree_traces
```

9 чекпоинт:
```
cat <class name>::<function name>.s
```

10-11 чекпоинты (`iteration`— итерация работы алгоритма покраски регистров):
```
cat <class name>::<function name><iteration>.s
```