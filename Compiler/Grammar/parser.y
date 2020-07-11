%skeleton "lalr1.cc"
%require "3.5"

%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    #include <memory>

    class Scanner;
    class Driver;

    #include "forward_decl.hpp"
    using namespace ast;
}

// %param { Driver &drv }

%define parse.trace
%define parse.error verbose

%code {
    #include "driver.hh"
    #include "location.hh"
    #include "scanner.h"

    #include "include_list.hpp"

    static yy::parser::symbol_type yylex(Scanner &scanner, Driver& driver) {
        return scanner.ScanToken();
    }
}

%lex-param { Scanner &scanner }
%lex-param { Driver &driver }
%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

%locations

%define api.token.prefix {TOK_}

%token
    END 0 "end of file"
    ASSIGN "="
    MINUS "-"
    PLUS "+"
    STAR "*"
    SLASH "/"
    LPAREN "("
    RPAREN ")"
    AND "&&"
    OR "||"
    NOT "!"
    LT "<"
    LE "<="
    GT ">"
    GE ">="
    EQUAL "=="
    TRUE "true"
    FALSE "false"
    INT "int"
    BOOL "boolean"
    SEMICOLON ";"
    PUBLIC "public"
    STATIC "static"
    VOID "void"
    MAIN "main"
    STRING "String"
    ARGS "args"
    LSBRACKET "["
    RSBRACKET "]"
    SBRACKETS "[]"
    LCBRACKET "{"
    RCBRACKET "}"
    PRINT "System.out.println"
    ASSERT "assert"
    RETURN "return"
    IF "if"
    ELSE "else"
    WHILE "while"
    CLASS "class"
    NEW "new"
    THIS "this"
    DOT "."
    COMMA ","
;

// %token <std::string> TYPE "type";
%token <std::string> IDENTIFIER "identifier";
%token <int> NUMBER "number";

%nterm <Program*> program;
%nterm <MainClass*> main_class;

%nterm <Statement*> statement;
%nterm <StatementList*> statement_list;

%nterm <Declaration*> declaration;
%nterm <DeclarationList*> declaration_list;
%nterm <VariableDeclaration*> variable_declaration;
%nterm <MethodDeclaration*> method_declaration;
%nterm <ClassDeclaration*> class_declaration;
%nterm <ClassDeclarationList*> class_declaration_list;

%nterm <Formal*> formal;
%nterm <FormalList*> formal_list;

%nterm <Expression*> expression;
%nterm <ExpressionList*> expression_list;

%nterm <MethodInvocation*> method_invocation;

%nterm <Type*> type;
%nterm <SimpleType*> simple_type;
%nterm <ArrayType*> array_type;

%nterm <NamedEntity*> named_entity;

// %printer { yyo << $$; } <*>;

%%

%start program;

program:
    main_class class_declaration_list {
    	$$ = new Program($1, $2);
    	driver.program_ = $$;
    };

main_class:
    "class" IDENTIFIER "{"
    "public" "static" "void" "main" "(" "String" "[]" "args" ")" "{"
    statement_list
    "}"
    "}" {
    	$$ = new MainClass(std::move($2), $14);
    };

statement_list:
    %empty {
    	$$ = nullptr;
    }
    | statement statement_list {
    	$$ = new StatementList($1, $2);
    };

%left "new";

statement:
    "assert" "(" expression ")" ";" {
    	$$ = new AssertStatement($3);
    }
    | variable_declaration {
    	$$ = new VariableDeclarationStatement($1);
    }
    | "if" "(" expression ")" statement {
    	$$ = new IfStatement($3, $5);
    }
    | "if" "(" expression ")" statement "else" statement {
    	$$ = new IfElseStatement($3, $5, $7);
    }
    | "while" "(" expression ")" statement {
    	$$ = new WhileStatement($3, $5);
    }
    | named_entity "=" expression ";" {
	$$ = new AssignmentStatement($1, $3);
    }
    | "return" expression ";" {
    	$$ = new ReturnStatement($2);
    }
    | PRINT "(" expression ")" ";"{
    	$$ = new PrintStatement($3);
    }
    | "{" statement_list "}" {
    	$$ = new ScopeStatement($2);
    }
    | method_invocation ";" {
    	$$ = new MethodInvocationStatement($1);
    };

type:
    simple_type {
    	$$ = $1;
    }
    | array_type {
    	$$ = $1;
    };

simple_type:
    "int" {
    	$$ = new SimpleType("int");
    }
    | "boolean" {
    	$$ = new SimpleType("boolean");
    }
    | "void" {
    	$$ = new SimpleType("void");
    }
    | IDENTIFIER {
    	$$ = new SimpleType($1);
    };

array_type:
    simple_type "[]" {
    	$$ = new ArrayType($1->GetIdentifier());
    };

// Can be on the left side of assignment
named_entity:
    "identifier" {
    	$$ = new NamedVariable($1);
    }
    | "identifier" "[" expression "]" {
    	$$ = new NamedArrayElement($1, $3);
    };

%left "||";
%left "&&";
%left "<" "<=" ">" ">=" "==";

%left "+" "-";
%left "*" "/" "%";

%left "!";

expression:
    "number" {
    	$$ = new IntExpression($1);
    }
    | "-" "number" {
    	$$ = new IntExpression(-$2);
    }
    | "true" {
    	$$ = new BoolExpression(true);
    }
    | "false" {
    	$$ = new BoolExpression(false);
    }
    | "identifier" {
    	$$ = new IdentExpression($1);
    }
    | "identifier" "[" expression "]" {
	$$ = new ArrayAccessExpression($1, $3);
    }
    | expression "*" expression {
    	$$ = new MulExpression($1, $3);
    }
    | expression "/" expression {
    	$$ = new DivExpression($1, $3);
    }
    | expression "%" expression {
    	$$ = new ModExpression($1, $3);
    }
    | expression "+" expression {
    	$$ = new AddExpression($1, $3);
    }
    | expression "-" expression {
    	$$ = new SubExpression($1, $3);
    }
    | "!" expression {
    	$$ = new NotExpression($2);
    }
    | expression "<" expression {
    	$$ = new LessExpression($1, $3);
    }
    | expression "<=" expression {
    	$$ = new LessEqualExpression($1, $3);
    }
    | expression ">" expression {
    	$$ = new GreaterExpression($1, $3);
    }
    | expression ">=" expression {
    	$$ = new GreaterEqualExpression($1, $3);
    }
    | expression "&&" expression {
    	$$ = new AndExpression($1, $3);
    }
    | expression "||" expression {
    	$$ = new OrExpression($1, $3);
    }
    | expression "==" expression {
    	$$ = new EqualExpression($1, $3);
    }
    | "new" simple_type "[" expression "]" {
      	$$ = new NewArrayExpression($2, $4);
    }
    | "new" "identifier" "(" ")" {
    	$$ = new NewExpression($2);
    }
    | "(" expression ")" {
    	$$ = $2;
    }
    | method_invocation {
    	$$ = new MethodInvocationExpression($1);
    }
    | "this" {
    	$$ = new ThisExpression();
    };

class_declaration_list:
    %empty {
    	$$ = nullptr;
    }
    | class_declaration class_declaration_list {
    	$$ = new ClassDeclarationList($1, $2);
    };

class_declaration:
    "class" "identifier" "{"
    declaration_list
    "}" {
    	$$ = new ClassDeclaration($2, $4);
    };

declaration_list:
    %empty {
    	$$ = nullptr;
    }
    | declaration declaration_list {
    	$$ = new DeclarationList($1, $2);
    };

declaration:
    variable_declaration {
    	$$ = $1;
    }
    | method_declaration {
    	$$ = $1;
    };

variable_declaration:
    type IDENTIFIER ";" {
        $$ = new VariableDeclaration($1, $2);
    };

method_declaration:
    "public" type "identifier" "(" formal_list ")" "{"
    statement_list
    "}" {
    	$$ = new MethodDeclaration($2, $3, $5, $8);
    };

formal_list:
    %empty {
	$$ = nullptr;
    }
    | formal {
    	$$ = new FormalList($1, nullptr);
    }
    | formal "," formal_list {
    	$$ = new FormalList($1, $3);
    };

formal:
    simple_type "identifier" {
    	$$ = new Formal($1, $2);
    };

method_invocation:
    expression "." "identifier" "(" ")" {
    	$$ = new MethodInvocation($1, $3);
    }
    | expression "." "identifier" "(" expression_list ")" {
    	$$ = new MethodInvocation($1, $3, $5);
    };

expression_list:
    %empty {
    	$$ = nullptr;
    }
    | expression {
    	$$ = new ExpressionList($1, nullptr);
    }
    | expression "," expression_list {
    	$$ = new ExpressionList($1, $3);
    };

%%

void yy::parser::error(const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}
