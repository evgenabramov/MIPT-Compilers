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
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%nterm <int> expr

%printer { yyo << $$; } <*>;

%%
%start unit;
unit: 
    assignments expr { driver.result_ = $2; };

type_identifier:
    IDENTIFIER;

simple_type:
    "int"
    | "boolean"
    | "void" 
    | type_identifier;

array_type:
    simple_type "[" "]";

type:
    simple_type | array_type;

variable_declaraion:
    type IDENTIFIER ";" {};

assignments:
    %empty {}
    | assignments assignment {};

assignment:
    "identifier" ":=" expr {
        driver.variables_[$1] = $3;
        // std::cout << drv.location.begin.line << "-" << drv.location.end.line << std::endl;
    };

method_arguments:
    %empty
    | expr 
    | expr "," expr;

method_invocaion:
    expr "." IDENTIFIER "(" method_invocaion ")";

binary_operator:
    "&&" | "||" | "<" | ">" | "==" | "%";

%left "+" "-";
%left "*" "/";

expr:
    expr binary_operator expr {}
    | expr "[" expr "]" {}
    | expr "." "length" {}
    | "new" simple_type "[" expr "]" {}
    | "new" type_identifier "(" ")" {}
    | "!" expr {}
    | "(" expr ")" {}
    | NUMBER {}
    | "this" {}
    | "true" {$$ = 1;}
    | "false" {$$ = 0;}
    | method_invocaion {}
    | IDENTIFIER {$$ = driver.variables_[$1];}
    | expr "+" expr {$$ = $1 + $3;}
    | expr "-" expr {$$ = $1 - $3;}
    | expr "*" expr {$$ = $1 * $3;}
    | expr "/" expr {$$ = $1 / $3;}
    | "(" expr ")" {$$ = $2;};

%%

void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
