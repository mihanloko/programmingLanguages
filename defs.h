//
// Created by mikhail on 01.10.2019.
//

#ifndef SCANNER_DEFS_H
#define SCANNER_DEFS_H

#define DEC 1
#define OCT 2
#define HEX 3
#define CONST 4
#define TYPE_INT 10
#define TYPE_CHAR 11
#define IDENT 12
#define EQUALS 20
#define GREATER_EQUAL 21
#define LESS_EQUAL 22
#define LESS 23
#define GREATER 24
#define NOT_EQUAL 25
#define PLUS 30
#define MINUS 31
#define ASSIGN 32
#define RIGHT_SHIFT 33
#define LEFT_SHIFT 34
#define STAR 35
#define SLASH 36
#define PERCENT 37
#define ROUND_LEFT 40
#define ROUND_RIGHT 41
#define CURLY_LEFT 42
#define CURLY_RIGHT 43
#define SQUARE_LEFT 44
#define SQUARE_RIGHT 45
#define DOT 50
#define VIRGULE 51
#define SEMICOLON 52
#define WORD_CLASS 60
#define WORD_IF 61
#define WORD_ELSE 62
#define WORD_RETURN 63
#define WORD_MAIN 64
#define ERROR 100
#define END 200

#define S 300
#define DES 301
#define CL 302
#define CLIN 303
#define MAIN 304
#define OPVAR 305
#define T 306
#define OP 307
#define EMOP 308
#define COMPOP 309
#define SIMOP 310
#define IF 311
#define ELSE 312
#define ASSIG 313
#define A0 314
#define A1 315
#define A11 316
#define A2 317
#define A22 318
#define A3 319
#define A33 320
#define A4 321
#define A44 322
#define A5 323
#define A55 324
#define A6 325
#define A7 326
#define EXP 327
#define VARDEF 328
#define VARLIST 329
#define VARDOP 330
#define VAR 331
#define P 332
#define OBJ 333
#define ARCL 334
#define AR 335
#define FIELD 336
#define SQ 337

#define SET_CLASS 400
#define BACK 401
#define START_MAIN 402
#define SAVE_TYPE 403
#define CHECK_TYPE 404
#define CHECK_CONDITION 405
#define PUSH 406
#define MATCH_ASSIGN 407
#define MATCH_1 408
#define MATCH_2 409
#define MATCH_3 410
#define MATCH_4 411
#define MATCH_5 412
#define FIND 413
#define MAKE_ARRAY 414
#define POP 415
#define CHECK_ARRAY 416
#define ADD 417
#define NEW_BLOCK 418
#define MATCH_6 419
#define MATCH_CONST 420
#define FIND_FIELD 421
#define PUSH_CONST 422


// ----------------------------------- Подпрограммы генерации  -----------------------------------
//==
#define GEN_EQ -1400
//!=
#define GEN_NEQ -1500
//>
#define GEN_GT -1600
//>=
#define GEN_GE -1700
//<
#define GEN_LT -1800
//<=
#define GEN_LE -1900
#define GEN_MUL -400
#define GEN_DIV -410
#define GEN_MOD -411
#define GEN_PLUS -412
#define GEN_MINUS -413
#define GEN_ASSIGNMENT -414
#define GEN_CMP -415
#define GEN_PROC -419
#define GEN_ENDP -420
#define GEN_LEFT_SHIFT -423
#define GEN_RIGHT_SHIFT -424
#define GEN_IDX -425
#define GEN_DOT -426

#define GEN_PUSH -300
#define GEN_IF -314
#define GEN_FORM_IF -317
#define GEN_SET_ADDR_NOP -610
#define GEN_GOTO -611
#define GEN_SET_TRUE_ADDR -612
#define GEN_SET_FALSE_ADDR -614
// ----------------------------------- Операции для триад  -----------------------------------
//==
#define TRI_EQ -1400
//!=
#define TRI_NEQ -1500
//>
#define TRI_GT -1600
//>=
#define TRI_GE -1700
//<
#define TRI_LT -1800
//<=
#define TRI_LE -1900
#define TRI_PLUS -2000
#define TRI_MINUS -2100
#define TRI_MUL -2200
#define TRI_DIV -2300
#define TRI_MOD -2400
#define TRI_ASSIGNMENT -2500
#define TRI_CMP -2600
#define TRI_CALL -2800
#define TRI_PROC -3000
#define TRI_ENDP -3100
#define TRI_JMP -3200
#define TRI_MOV -3300
#define TRI_IF -3400
#define TRI_RET -3500
#define TRI_NOP -3600
#define TRI_UNIQUE_LABEL -4120
#define TRI_LEFT_SHIFT -4200
#define TRI_RIGHT_SHIFT -4210
#define TRI_IDX -4220
#define TRI_DOT -4230
#define TRI_CHAR_INT -4000
#define TRI_INT_CHAR -4010

#endif //SCANNER_DEFS_H
