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

// %nterm <Declaration*> declaration;
// %nterm <DeclarationList*> declaration_list;
%nterm <VariableDeclaration*> variable_declaration;

%nterm <Expression*> expression;
// %nterm <ExpressionList*> expression_list;

%nterm <Type*> type;
%nterm <SimpleType*> simple_type;
%nterm <ArrayType*> array_type;

%nterm <NamedEntity*> named_entity;

// %printer { yyo << $$; } <*>;

%%

%start program;

program:
    main_class {
    	$$ = new Program($1);
    	driver.program_ = $$;
    };

main_class:
    "class" IDENTIFIER "{"
    "public" "static" "void" "main" "(" "String" "[" "]" "args" ")" "{"
    statement_list
    "}"
    "}" {
    	$$ = new MainClass(std::move($2), $15);
    };

statement_list:
    %empty {
    	$$ = new StatementList();
    }
    | statement statement_list {
    	$$ = new StatementList($1, $2);
    };

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
    };

variable_declaration:
    type IDENTIFIER ";" {
        $$ = new VariableDeclaration($1, $2);
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
    }

named_entity:
    IDENTIFIER {
    	$$ = new NamedVariable($1);
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
    | "(" expression ")" {
    	$$ = $2;
    };

%%

void yy::parser::error(const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}
