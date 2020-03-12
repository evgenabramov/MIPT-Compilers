%skeleton "lalr1.cc"
%require "3.5"

%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    class Scanner;
    class Driver;
}

// %param { Driver &drv }

%define parse.trace
%define parse.error verbose

%code {
    #include "driver.hh"
    #include "location.hh"

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
    ASSIGN ":="
    MINUS "-"
    PLUS "+"
    STAR "*"
    SLASH "/"
    LPAREN "("
    RPAREN ")"
    AND "&&"
    OR "||"
    LT "<"
    LE "<="
    GT ">"
    GE ">="
    TRUE "true"
    FALSE "false"
    INT "int"
    BOOL "bool"
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
;

// %token <std::string> TYPE "type";
%token <std::string> IDENTIFIER "identifier";
%token <int> NUMBER "number";

%nterm <int> expr;
%nterm <bool> bool_expr;
%nterm <int> int_expr;
%nterm <std::string> type;

%printer { yyo << $$; } <*>;

%%

%start unit;

unit: 
    "public" "static" "void" "main" "(" "String" "[" "]" "args" ")" "{" 
    statements expr 
    "}" {driver.result_ = $13;};

type:
    INT {$$ = "int";}
    | BOOL {$$ = "bool";};

variable_declaraion:
    type IDENTIFIER ";" {
        if (driver.variables_.count($2) > 0 || driver.bool_variables_.count($2) > 0) {
            std::cerr << "Redefenition of '" << $2 << "'" << std::endl;
        } else if ($1 != "int" && $1 != "bool") {
            // Process user-defined type
        }
    }
    | type IDENTIFIER ":=" expr ";" {
        if (driver.variables_.count($2) > 0 || driver.bool_variables_.count($2) > 0) {
            std::cerr << "Redefenition of '" << $2 << "'" << std::endl;
        } else if ($1 == "int") {
            driver.variables_[$2] = $4;            
        } else if ($1 == "bool") {
            driver.bool_variables_[$2] = $4;
        } else {
            // Process user-defined type
        }
    };

statements:
    %empty {}
    | statements statement {}
    | statements variable_declaraion {};

statement:
    IDENTIFIER ":=" expr ";" {
        driver.variables_[$1] = $3;
    };

%left "||";
%left "&&";
%left "<" "<=" ">" ">=";

bool_expr: 
    int_expr "<" int_expr {$$ = static_cast<bool>($1 < $3);}
    | int_expr "<=" int_expr {$$ = static_cast<bool>($1 <= $3);}
    | int_expr ">" int_expr {$$ = static_cast<bool>($1 > $3);}
    | int_expr ">=" int_expr {$$ = static_cast<bool>($1 >= $3);}
    | bool_expr "&&" bool_expr {$$ = static_cast<bool>($1 && $3);}
    | bool_expr "||" bool_expr {$$ = static_cast<bool>($1 || $3);}
    | "(" bool_expr ")" {$$ = $2;}
    | "true" {$$ = true;}
    | "false" {$$ = false;};

%left "+" "-";
%left "*" "/";

int_expr: 
    NUMBER {$$ = $1;}
    | IDENTIFIER {$$ = driver.variables_[$1];}
    | int_expr "+" int_expr {$$ = $1 + $3;}
    | int_expr "-" int_expr {$$ = $1 - $3;}
    | int_expr "*" int_expr {$$ = $1 * $3;}
    | int_expr "/" int_expr {$$ = $1 / $3;}
    | "(" int_expr ")" {$$ = $2;};

expr: 
    bool_expr {$$ = static_cast<int>($1);}
    | int_expr {$$ = $1;};

%%

void yy::parser::error(const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}
