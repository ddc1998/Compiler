/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    PROGRAM = 258,
    LPAREN = 259,
    RPAREN = 260,
    SEMICOLON = 261,
    DOT = 262,
    COMMA = 263,
    VAR = 264,
    COLON = 265,
    ARRAY = 266,
    LBRAC = 267,
    RBRAC = 268,
    OF = 269,
    INTEGER = 270,
    REAL = 271,
    FUNCTION = 272,
    PROCEDURE = 273,
    PBEGIN = 274,
    END = 275,
    ASSIGNOP = 276,
    IF = 277,
    THEN = 278,
    ELSE = 279,
    WHILE = 280,
    DO = 281,
    LT = 282,
    GT = 283,
    LE = 284,
    GE = 285,
    EQUAL = 286,
    NOTEQUAL = 287,
    PLUS = 288,
    MINUS = 289,
    STAR = 290,
    SLASH = 291,
    NOT = 292,
    DOTDOT = 293,
    STRING = 294,
    AND = 295,
    OR = 296,
    Node_TOKEN = 297,
    DIGNUM = 298,
    ID = 299,
    REALNUM = 300,
    STRINGCONST = 301
  };
#endif
/* Tokens.  */
#define PROGRAM 258
#define LPAREN 259
#define RPAREN 260
#define SEMICOLON 261
#define DOT 262
#define COMMA 263
#define VAR 264
#define COLON 265
#define ARRAY 266
#define LBRAC 267
#define RBRAC 268
#define OF 269
#define INTEGER 270
#define REAL 271
#define FUNCTION 272
#define PROCEDURE 273
#define PBEGIN 274
#define END 275
#define ASSIGNOP 276
#define IF 277
#define THEN 278
#define ELSE 279
#define WHILE 280
#define DO 281
#define LT 282
#define GT 283
#define LE 284
#define GE 285
#define EQUAL 286
#define NOTEQUAL 287
#define PLUS 288
#define MINUS 289
#define STAR 290
#define SLASH 291
#define NOT 292
#define DOTDOT 293
#define STRING 294
#define AND 295
#define OR 296
#define Node_TOKEN 297
#define DIGNUM 298
#define ID 299
#define REALNUM 300
#define STRINGCONST 301

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 28 "minipas.y" /* yacc.c:1909  */

    struct Node *node;
    int number;
    double realnum;
    char *string;
    int line_no;

#line 154 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
