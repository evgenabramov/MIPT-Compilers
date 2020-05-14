%{
    #include <cerrno>
    #include <climits>
    #include <cstdlib>
    #include <cstring> // strerror
    #include <string>
    #include <iostream>
    #include "driver.hh"
    #include "parser.hh"
%}

%option noyywrap nounput noinput batch debug

%option c++
%option yyclass="Scanner"

%{
  // A number symbol corresponding to the value in S.
  yy::parser::symbol_type make_NUMBER(
    const std::string& s,
    const yy::parser::location_type& loc
  );
%}

id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t\r]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = driver.location_;
  // Code run each time yylex is called.
  loc.step();
%}

{blank}+   loc.step ();
\n+        loc.lines (yyleng); loc.step ();

"-"        {
              // Example on how to process location info
              // std::cout << loc.begin.column << " " << loc.end.column << std::endl;
              return yy::parser::make_MINUS  (loc);
           }
"+"        return yy::parser::make_PLUS      (loc);
"*"        return yy::parser::make_STAR      (loc);
"/"        return yy::parser::make_SLASH     (loc);
"("        return yy::parser::make_LPAREN    (loc);
")"        return yy::parser::make_RPAREN    (loc);
"="       return yy::parser::make_ASSIGN    (loc);
"<"        return yy::parser::make_LT        (loc);
"<="       return yy::parser::make_LE        (loc);
">"        return yy::parser::make_GT        (loc);
">="       return yy::parser::make_GE        (loc);
"=="       return yy::parser::make_EQUAL     (loc);
"&&"       return yy::parser::make_AND       (loc);
"||"       return yy::parser::make_OR        (loc);
"!"        return yy::parser::make_NOT       (loc);
"true"     return yy::parser::make_TRUE      (loc);
"false"    return yy::parser::make_FALSE     (loc);
"int"      return yy::parser::make_INT       (loc);
"boolean"  return yy::parser::make_BOOL      (loc);
";"        return yy::parser::make_SEMICOLON (loc);
"public"   return yy::parser::make_PUBLIC    (loc);
"static"   return yy::parser::make_STATIC    (loc);
"void"     return yy::parser::make_VOID      (loc);
"main"     return yy::parser::make_MAIN      (loc);
"String"   return yy::parser::make_STRING    (loc);
"args"     return yy::parser::make_ARGS      (loc);
"["        return yy::parser::make_LSBRACKET (loc);
"]"        return yy::parser::make_RSBRACKET (loc);
"{"        return yy::parser::make_LCBRACKET (loc);
"}"        return yy::parser::make_RCBRACKET (loc);
"assert"   return yy::parser::make_ASSERT    (loc);
"return"   return yy::parser::make_RETURN    (loc);
"if"       return yy::parser::make_IF        (loc);
"else"     return yy::parser::make_ELSE      (loc);
"while"    return yy::parser::make_WHILE     (loc);
"class"    return yy::parser::make_CLASS     (loc);
"= new"    return yy::parser::make_EQUALNEW  (loc);
"this"     return yy::parser::make_THIS      (loc);
"."        return yy::parser::make_DOT       (loc);
","        return yy::parser::make_COMMA     (loc);

"System.out.println" return yy::parser::make_PRINT (loc);

{int}      {
              // Example on how to override standard function
              return make_NUMBER(yytext, loc);
           }
{id}       return yy::parser::make_IDENTIFIER(yytext, loc);
.          {
              throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext));
           }
<<EOF>>    return yy::parser::make_END (loc);
%%

yy::parser::symbol_type make_NUMBER(
  const std::string &s,
  const yy::parser::location_type& loc
) {
  errno = 0;
  long n = strtol(s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error(loc, "integer is out of range: " + s);
  return yy::parser::make_NUMBER((int) n, loc);
}