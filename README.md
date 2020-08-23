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

Discover the result of `PrintVisitor` и `SymbolTreeVisitor` work:

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

Класс `Frame` предоставляет доступ к объектам в текущей области видимости, возвращаемому значению и родительскому фрейму.

`FunctionTable` - хранит индексы на стеке для переменных, доступных в текущей области видимости.

Класс `FrameTranslator` моделирует состояние Frame-а во время выполнения функции, а именно:

- Определяет место на стеке для:
    - Аргументов функции
    - Локальных переменных
- Хранит информацию о:
    - Числе аргументов функции
    - Размере стекового фрейма
- По названию переменной позволяет получать ее адрес в текущей области видимости. Адрес может быть в регистре или на стеке (см. [Address](Compiler/MethodMechanisms/Address))

### [Дерево промежуточного представления (IR Tree)](Compiler/IRTree)

Дерево промежуточного представления позволяет  перевести инструкции исходного языка на низкоуровневый (например, ассемблер) за счет использования ограниченного набора примитивных операций. Эти операции соответствуют вершинам построенного IR-дерева:

- Expression
    - `BINOP` (бинарная операция)
    - `CALL` (вызов функции)
    - `CONST` (константа)
    - `ESEQ` (последовательные *statement* и возврат значения)
    - `MEM` (обращение к памяти)
    - `NAME` (имя метки в другой операции)
    - `TEMP` (временная переменная)
- Statement
    - `EXP` (выражение, значение которого игнорируется)
    - `CJUMP` (условный переход к метке)
    - `LABEL` (создание метки)
    - `MOVE` (присваивание)
    - `SEQ` (последовательное выполнение *statements*)
- ExpressionList

**IR-дерево** строится из `AST` с помощью визитора [`IRTreeBuildVisitor`](Compiler/Visitors/IRTreeBuildVisitor.h). Для корректного преобразования типов вершин IR-дерева используется обертка `SubtreeWrapper`.

Для вывода и отладки построенного IR-дерева есть [`PrintVisitor`](Compiler/IRTree/Visitors/PrintVisitor.h).

Перед генерацией низкоуровневого кода построенное дерево приводится к каноническому виду за счет:

- Избавления от двойного `CALL`-а — когда среди переданных аргументов есть другой `CALL` ([`DoubleCallEliminateVisitor`](Compiler/IRTree/Visitors/DoubleCallEliminateVisitor.h))
- Поднятия вершин `ESEQ` на верхние уровни в дереве ([`ESEQEliminateVisitor`](Compiler/IRTree/Visitors/ESEQEliminateVisitor.h))
- Линеаризации — расположения `SEQ` в правостороннем порядке ([`LinearizationVisitor`](Compiler/IRTree/Visitors/LinearizationVisitor.h))

Посмотреть IR-дерево после работы визиторов:

```shell
cat <class name>_<function name>_IRTree_raw
cat <class name>_<function name>_IRTree_without_double_call
cat <class name>_<function name>_IRTree_without_ESEQ
cat <class name>_<function name>_IRTree_linearized
```

К построенному в результате дереву можно применить различные по уровню локальности оптимизации.

### [Блоки IR-дерева](Compiler/IRTree/Blocks) 

IR-блок — набор инструкций, которые обязаны выполниться последовательно, независимо от условий и переходов по меткам. Формально, каждый блок начинается с объявления метки и заканчивается `JUMP-` или `CJUMP-`инструкцией.

Такое упрощенное представление программы позволяет провести дополнительные оптимизации, в частности более эффективно назначать регистры временным переменным во время этапа генерации кода.

[`BlockBorderVisitor`](Compiler/IRTree/Visitors/BlockBorderVisitor.h) — отвечает за подготовку IR-дерева к построению IR-блоков, а именно:

- Закрывает неоконченный инструкцией `JUMP` блок перед объявлением метки
- Добавляет переход к эпилогу функции, в котором возвращается значение

Посмотреть IR-дерево после преобразования:

```shell
cat <class name>_<function_name>_IRTree_with_blocks
```

Построение блоков осуществляет [`BlockBuildVisitor`](Compiler/IRTree/Visitors/BlockBuildVisitor.h).

Посмотреть получившийся [граф](Compiler/IRTree/Blocks/BlockGraph.hpp) из IR-блоков:

```shell
cat <class name>_<function name>_IRTree_blocks
```

Для дополнительных оптимизаций несколько IR-блоков принято объединять в непересекающиеся следы ([Trace](Compiler/IRTree/Blocks/Trace.hpp)).

Посмотреть распределение блоков по следам:

```shell
cat <class name>_<function name>_IRTree_traces
```

### [Генерация кода (перевод в ассемблер)](Compiler/IRTree/Visitors/CodeGenerationVisitor.h)

За перевод в *ARM Assembly* отвечает визитор [`CodeGenerationVisitor`](Compiler/IRTree/Visitors/CodeGenerationVisitor.h).

Чтобы получить оптимальный по числу инструкций и промежуточных регистров код ассемблера, IR-дерево необходимо покрыть непересекающимися паттернами, состоящими из его вершин, образующих поддерево. Каждый паттерн необходимо рассматривать вручную, сопоставляя ему подходящий набор инструкций.

Существует два алгоритма покрытия IR-дерева паттернами: **Maximal munch** и **динамический**. В данном проекте реализован Maximal munch. Он предполагает рекурсивный обход дерева от корня к листьям, когда в текущей вершине последовательно проверяются паттерны от наибольшего по числу вершин к наименьшему. Если очередной паттерн подходит, соответствующая ему инструкция добавляется в общий список после рекурсивного обхода дочерних вершин.

Регистры на данном этапе заменены `Temporary`-переменными, т.е. считается, что в распоряжении есть бесконечное число виртуальных регистров.

Посмотреть результат работы `CodeGenerationVisitor`:

```shell
cat <class name>_<function name>.s
```

### [Аллокация регистров](Compiler/IRTree/Instructions)

В [инструкциях](Compiler/IRTree/Instructions/Instruction.hpp), полученных после генерации кода, учтены регистры, значения в которых влияют на результат и регистры, значения в которых изменяются.

Для перевода виртуальных регистров в физические необходимо выделить пары виртуальных регистров, которым не может быть назначен один и тот же физический регистр (построить граф конфликтов - [`InterferenceGraph`](Compiler/IRTree/Instructions/InterferenceGraph.hpp)). Предварительно строится [`ControlFlowGraph`](Compiler/IRTree/Instructions/ControlFlowGraph.hpp), в котором каждой инструкции соответствует отдельная вершина, а две инструкции соединены ребром, если после первой при некотором исполнении (в зависимости от условий) может следовать вторая. К нему применяется алгоритм поиска *актуальных* на ребрах переменных (**Liveness analysis**, переменная считается *актуальной* на ребрах между ее определением и использованием с этим значением), в ходе которого все вершины несколько раз перебираются в порядке, обратном порядку топологической сортировки. Во время этого перебора решаются уравнения потока (**Dataflow equations**).

Теперь исходная задача эквивалентна задаче раскраски графа конфликтов, когда связанные вершины не могут быть покрашены в один цвет. С учетом числа имеющихся в распоряжении регистров `InterferenceGraph` последовательно упрощается за счет исключения правильно подобранных вершин. Этим вершинам впоследствии можно назначить жадным образом цвета, отличные от цветов их соседей.

Посмотреть на ассемблерный код функций после назначения физических регистров (здесь `<iteration>` — номер итерации алгоритма покраски):

```shell
cat <class name>_<function name><iteration>.s
```

### [Полный код программы](Compiler/driver.cpp)

Для выполнения *calling conventions* к инструкциям каждой функции добавляются пролог и эпилог.

В начале функции:

- Сохраняется значение *Frame Pointer*
- *Stack Pointer* записывается во *Stack Pointer*
- Аллоцируется место на стеке для аргументов и локальных переменных
- Значения аргументов, переданных в регистрах, записываются на стек
- Сохраняются *callee-save* регистры

В конце функции:

- Возвращаемое значение записывается в регистр `r0` 
- Восстанавливаются значения *callee-save* регистров
- Восстанавливается значение *Stack Pointer*
- Восстанавливается значение *Frame pointer*
- Происходит переход по *Link register*

Для отладки и проверки корректности использовался [Compiler Explorer](https://godbolt.org).

Также добавляется `.data`-метка для корректной работы функции `printf` из библиотеки языка С.

После этого в [driver.cpp](Compiler/driver.cpp) код всех функций записывается в общий файл.

Посмотреть код программы, который можно запустить через **gcc**:

```shell
cat final_program.s
```

---

## Использованные учебные материалы

- Лекции и семинары по курсу **"Языки программирования и теории компиляции"** в 4 семестре на Факультете Инноваций и Высоких Технологий МФТИ (в составе ФПМИ МФТИ)
    
    Лектор — [@akhtyamovpavel](https://github.com/akhtyamovpavel) 

- Andrew W. Appel, **Modern Compiler Implementation in Java**, Cambridge University Press

---

## Направления дальнейшего развития проекта

- [ ] Сделать поддержку процессоров с *CISC-архитектурой* (генерация кода **x86-64 Assembly**) 
- [ ] Проверить отсутствие утечек памяти
- [ ] Попробовать разрешить Shift/Reduce конфликты грамматики
- [ ] Доделать `TODO`-фичи в коде
- [ ] Добавить тесты
- [ ] Настроить CI с запуском тестов и примеров программ
- [ ] Сделать подсветку синтаксиса для популярного редактора кода