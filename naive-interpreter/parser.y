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
;

%token <std::string> IDENTIFIER "identifier";
%token <int> NUMBER "number";

%nterm <int> expr;
%nterm <bool> bool_expr;
%nterm <int> int_expr;

%printer { yyo << $$; } <*>;

%%
%start unit;
unit: 
    assignments expr { driver.result_ = $2; };

type: 
    INT | BOOL;

variable_declaraion:
    type IDENTIFIER ";" {
        if (driver.variables_.count($2) > 0 || driver.bool_variables_.count($2) > 0) {
            std::cerr << "Redefenition of '" << $2 << "'" << std::endl;
            return;
        }
    }
    | type IDENTIFIER ":=" expr ";" {
        if (driver.variables_.count($2) > 0 || driver.bool_variables_.count($2) > 0) {
            std::cerr << "Redefenition of '" << $2 << "'" << std::endl;
            return;
        }
        if ($1 == "int") {
            driver.variables_[$2] = $4;            
        } else {
            driver.bool_variables_[$2] = $4;
        }
    };

assignments:
    %empty {}
    | assignments assignment {}
    | assignments variable_declaraion {};

assignment:
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
    | "false" {$$ = false;}
    ;

%left "+" "-";
%left "*" "/";

int_expr: 
    NUMBER {$$ = $1;}
    | IDENTIFIER {$$ = driver.variables_[$1];}
    | int_expr "+" int_expr {$$ = $1 + $3;}
    | int_expr "-" int_expr {$$ = $1 - $3;}
    | int_expr "*" int_expr {$$ = $1 * $3;}
    | int_expr "/" int_expr {$$ = $1 / $3;}
    | "(" int_expr ")" {$$ = $2;}
    ;

expr: 
    bool_expr {$$ = static_cast<int>($1);}
    | int_expr {$$ = $1;}
    ;

%%

void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
