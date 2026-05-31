#include "stdheader.h"

/*  A Bison parser, made from ..\test.y with Bison version GNU Bison version 1.24
*/

#define YYBISON 1  /* Identify Bison output.  */

#define	tAssignment	258
#define	tLeftBracket	259
#define	tRightBracket	260
#define	tQuotedString	261
#define	tID	262
#define	tInteger	263
#define	tLParen	264
#define	tRParen	265
#define	tDot	266
#define	tComma	267
#define	tPlus	268
#define	tMinus	269
#define	tSlesh	270
#define	tStar	271
#define	t4Dot	272
#define	t2Dot	273
#define	tDotComma	274
#define	tNot	275
#define	tGEq	276
#define	tLEq	277
#define	tEq	278
#define	tNEq	279
#define	tIF	280
#define	tELSE	281
#define	tLQt	282
#define	tRQt	283
#define	tAnd	284
#define	tOr	285
#define	tBrigadeVar	286
#define	Paren	287
#define	NEG	288


#include <math.h>
#include "stdafx.h"
#include <malloc.h>
#include <stdio.h>
#include <ctype.h>
#include "QParser.h"

#define alloca _alloca

void yyerror (const char *s);
int yylex (void);

extern CQParser* QParser;
const int c_MaxTokenSize = 4096;
class Token
{
	char	buf[c_MaxTokenSize];
	int		size;
public:
	void	SetString( const char* str, int len )
	{
		len = (len >= c_MaxTokenSize) ? c_MaxTokenSize - 1 : len;
		strncpy( buf, str, len );
		buf[len] = 0; 
		size = len;
	}
	operator const char*() const { return buf; } 
	int len() { return size; }
};

#define YYSTYPE castom


typedef union {              /* определение набора типов */
	Token str;
	Operand* OP;
} YYSTYPE;

#ifndef YYLTYPE
typedef
struct yyltype
{
	int timestamp;
	int first_line;
	int first_column;
	int last_line;
	int last_column;
	char *text;
}
yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		85
#define	YYFLAG		-32768
#define	YYNTBASE	34

#define YYTRANSLATE(x) ((unsigned)(x) <= 288 ? yytranslate[x] : 42)

static const char yytranslate[] = {     0,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
0,     1,     6,     8,    14,    16,    18,    22,    23,    25,
28,    32,    37,    43,    51,    57,    59,    63,    67,    71,
75,    79,    83,    87,    90,    94,    98,    99,   101,   105,
107,   109,   114,   118,   122,   126,   130,   134,   137,   141
};

static const short yyrhs[] = {    -1,
7,    27,    37,    28,     0,    35,     0,    41,    19,    19,
19,    19,     0,    19,     0,    38,     0,    27,    37,    28,
0,     0,    38,     0,    37,    38,     0,     7,     7,    19,
0,    41,     3,    41,    19,     0,    25,     9,    39,    10,
36,     0,    25,     9,    39,    10,    36,    26,    36,     0,
7,     9,    40,    10,    19,     0,    41,     0,    41,     4,
41,     0,    41,     5,    41,     0,    41,    21,    41,     0,
41,    22,    41,     0,    41,    23,    41,     0,    41,    24,
41,     0,     9,    39,    10,     0,    20,    39,     0,    39,
29,    39,     0,    39,    30,    39,     0,     0,    41,     0,
40,    12,    41,     0,     8,     0,     6,     0,     7,     9,
40,    10,     0,     7,    18,     7,     0,    41,    13,    41,
0,    41,    14,    41,     0,    41,    16,    41,     0,    41,
15,    41,     0,    14,    41,     0,     9,    41,    10,     0,
7,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
81,    82,    83,    86,    92,    93,    94,    96,    97,    98,
101,   102,   103,   104,   105,   107,   108,   109,   110,   111,
112,   113,   114,   115,   116,   117,   121,   122,   123,   125,
126,   127,   128,   129,   130,   131,   132,   133,   134,   135
};

static const char * const yytname[] = {   "$","error","$undefined.","tAssignment",
"tLeftBracket","tRightBracket","tQuotedString","tID","tInteger","tLParen","tRParen",
"tDot","tComma","tPlus","tMinus","tSlesh","tStar","t4Dot","t2Dot","tDotComma",
"tNot","tGEq","tLEq","tEq","tNEq","tIF","tELSE","tLQt","tRQt","tAnd","tOr","tBrigadeVar",
"Paren","NEG","script","line","functions","functionlist","function","boolexp",
"argumentlist","exp",""
};
#endif

static const short yyr1[] = {     0,
34,    34,    34,    35,    36,    36,    36,    37,    37,    37,
38,    38,    38,    38,    38,    39,    39,    39,    39,    39,
39,    39,    39,    39,    39,    39,    40,    40,    40,    41,
41,    41,    41,    41,    41,    41,    41,    41,    41,    41
};

static const short yyr2[] = {     0,
0,     4,     1,     5,     1,     1,     3,     0,     1,     2,
3,     4,     5,     7,     5,     1,     3,     3,     3,     3,
3,     3,     3,     2,     3,     3,     0,     1,     3,     1,
1,     4,     3,     3,     3,     3,     3,     2,     3,     1
};

static const short yydefact[] = {     1,
31,    40,    30,     0,     0,     3,     0,    27,     0,     8,
40,     0,    38,     0,     0,     0,     0,     0,     0,    28,
33,    40,     0,     0,     9,     0,    39,    34,    35,    37,
36,     0,    32,     0,     0,    27,     0,     2,    10,     0,
0,    29,    11,     0,     0,     0,     0,    16,     0,     4,
32,     0,    16,    24,     0,     0,     0,     0,     0,     0,
0,     0,     0,    12,    15,    23,     5,     8,    13,     6,
25,    26,    17,    18,    19,    20,    21,    22,     0,     0,
7,    14,     0,     0,     0
};

static const short yydefgoto[] = {    83,
6,    69,    24,    25,    47,    19,    26
};

static const short yypact[] = {   131,
-32768,    98,-32768,   135,   135,-32768,   137,   135,     4,    92,
9,   114,-32768,   135,   135,   135,   135,   -16,    -3,   151,
-32768,    67,    10,    14,-32768,   120,-32768,    28,    28,-32768,
-32768,     5,-32768,   135,    32,   135,   112,-32768,-32768,   135,
34,   151,-32768,    38,   112,   112,    -4,    89,   144,-32768,
45,    25,    68,    66,    61,   112,   112,   135,   135,   135,
135,   135,   135,-32768,-32768,-32768,-32768,    92,     3,-32768,
-32768,-32768,   151,   151,   151,   151,   151,   151,    24,    61,
-32768,-32768,    47,    71,-32768
};

static const short yypgoto[] = {-32768,
-32768,    -1,    19,   -14,   -44,    41,     0
};


#define	YYLAST		167


static const short yytable[] = {     7,
52,    54,    32,    12,    13,    55,    33,    20,    34,    39,
21,    71,    72,    28,    29,    30,    31,     8,    37,     1,
22,     3,     4,    41,    56,    57,     9,     5,    80,     1,
22,     3,     4,    42,    66,    20,    48,     5,    23,    49,
70,    38,    16,    17,    53,    48,    84,    51,    23,    34,
43,    81,    50,    56,    57,    48,    48,    73,    74,    75,
76,    77,    78,    65,    39,    70,     1,    22,     3,     4,
85,    58,    59,    35,     5,    36,    44,    27,    82,    67,
14,    15,    16,    17,     9,    23,    79,    68,    60,    61,
62,    63,    58,    59,    56,    57,     0,     1,    22,     3,
4,    14,    15,    16,    17,     5,     8,     0,     0,    60,
61,    62,    63,     0,     0,     9,    23,     1,    11,     3,
45,     0,    40,    27,    10,     5,    14,    15,    16,    17,
0,    46,    14,    15,    16,    17,     1,     2,     3,     4,
1,    11,     3,     4,     5,     0,     0,     0,     5,    14,
15,    16,    17,     0,     0,    18,    14,    15,    16,    17,
0,     0,    64,    14,    15,    16,    17
};

static const short yycheck[] = {     0,
45,    46,    19,     4,     5,    10,    10,     8,    12,    24,
7,    56,    57,    14,    15,    16,    17,     9,     9,     6,
7,     8,     9,    19,    29,    30,    18,    14,    26,     6,
7,     8,     9,    34,    10,    36,    37,    14,    25,    40,
55,    28,    15,    16,    45,    46,     0,    10,    25,    12,
19,    28,    19,    29,    30,    56,    57,    58,    59,    60,
61,    62,    63,    19,    79,    80,     6,     7,     8,     9,
0,     4,     5,     7,    14,     9,    36,    10,    80,    19,
13,    14,    15,    16,    18,    25,    68,    27,    21,    22,
23,    24,     4,     5,    29,    30,    -1,     6,     7,     8,
9,    13,    14,    15,    16,    14,     9,    -1,    -1,    21,
22,    23,    24,    -1,    -1,    18,    25,     6,     7,     8,
9,    -1,     3,    10,    27,    14,    13,    14,    15,    16,
-1,    20,    13,    14,    15,    16,     6,     7,     8,     9,
6,     7,     8,     9,    14,    -1,    -1,    -1,    14,    13,
14,    15,    16,    -1,    -1,    19,    13,    14,    15,    16,
-1,    -1,    19,    13,    14,    15,    16
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */


/* Skeleton output parser for bison,
Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
Bison output file, you may use that output file without restriction.
This special exception was added by the Free Software Foundation
in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
#pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
	void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
when the %semantic_parser declaration is not specified in the grammar.
It was written by Richard Stallman by simplifying the hairy parser
used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
It is replaced by the list of actions, each action
as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
This remains here temporarily to ease the
transition to the new meaning of YYERROR, for GCC.
Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
	do								\
	if (yychar == YYEMPTY && yylen == 1)				\
{ yychar = (token), yylval = (value);			\
	yychar1 = YYTRANSLATE (yychar);				\
	YYPOPSTACK;						\
	goto yybackup;						\
}								\
  else								\
{ yyerror ("syntax error: cannot back up"); YYERROR; }	\
	while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
(effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
char *from;
char *to;
int count;
{
	register char *f = from;
	register char *t = to;
	register int i = count;

	while (i-- > 0)
		*t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
	register char *f = from;
	register char *t = to;
	register int i = count;

	while (i-- > 0)
		*t++ = *f++;
}

#endif
#endif



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
into yyparse.  The argument should have type void *.
It should actually point to an object.
Grammar actions can access the variable by casting it
to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
YYPARSE_PARAM_DECL
{
	register int yystate;
	register int yyn;
	register short *yyssp;
	register YYSTYPE *yyvsp;
	int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
	int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

	short	yyssa[YYINITDEPTH];	/*  the state stack			*/
	YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

	short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
	YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
	YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
	YYLTYPE *yyls = yylsa;
	YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

	int yystacksize = YYINITDEPTH;

#ifdef YYPURE
	int yychar;
	YYSTYPE yylval;
	int yynerrs;
#ifdef YYLSP_NEEDED
	YYLTYPE yylloc;
#endif
#endif

	YYSTYPE yyval;		/*  the variable used to return		*/
	/*  semantic values from the action	*/
	/*  routines				*/

	int yylen;

#if YYDEBUG != 0
	if (yydebug)
		fprintf(stderr, "Starting parse\n");
#endif

	yystate = 0;
	yyerrstatus = 0;
	yynerrs = 0;
	yychar = YYEMPTY;		/* Cause a token to be read.  */

	/* Initialize stack pointers.
	Waste one element of value and location stack
	so that they stay on the same level as the state stack.
	The wasted elements are never initialized.  */

	yyssp = yyss - 1;
	yyvsp = yyvs;
#ifdef YYLSP_NEEDED
	yylsp = yyls;
#endif

	/* Push a new state, which is found in  yystate  .  */
	/* In all cases, when you get here, the value and location stacks
	have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

	*++yyssp = yystate;

	if (yyssp >= yyss + yystacksize - 1)
	{
		/* Give user a chance to reallocate the stack */
		/* Use copies of these so that the &'s don't force the real ones into memory. */
		YYSTYPE *yyvs1 = yyvs;
		short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
		YYLTYPE *yyls1 = yyls;
#endif

		/* Get the current used size of the three stacks, in elements.  */
		int size = yyssp - yyss + 1;

#ifdef yyoverflow
		/* Each stack pointer address is followed by the size of
		the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
		/* This used to be a conditional around just the two extra args,
		but that might be undefined if yyoverflow is a macro.  */
		yyoverflow("parser stack overflow",
			&yyss1, size * sizeof (*yyssp),
			&yyvs1, size * sizeof (*yyvsp),
			&yyls1, size * sizeof (*yylsp),
			&yystacksize);
#else
		yyoverflow("parser stack overflow",
			&yyss1, size * sizeof (*yyssp),
			&yyvs1, size * sizeof (*yyvsp),
			&yystacksize);
#endif

		yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
		yyls = yyls1;
#endif
#else /* no yyoverflow */
		/* Extend the stack our own way.  */
		if (yystacksize >= YYMAXDEPTH)
		{
			yyerror("parser stack overflow");
			return 2;
		}
		yystacksize *= 2;
		if (yystacksize > YYMAXDEPTH)
			yystacksize = YYMAXDEPTH;
		yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
		__yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
		yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
		__yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
		yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
		__yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

		yyssp = yyss + size - 1;
		yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
		yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
		if (yydebug)
			fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

		if (yyssp >= yyss + yystacksize - 1)
			YYABORT;
	}

#if YYDEBUG != 0
	if (yydebug)
		fprintf(stderr, "Entering state %d\n", yystate);
#endif

	goto yybackup;
yybackup:

	/* Do appropriate processing given the current state.  */
	/* Read a lookahead token if we need one and don't already have one.  */
	/* yyresume: */

	/* First try to decide what to do without reference to lookahead token.  */

	yyn = yypact[yystate];
	if (yyn == YYFLAG)
		goto yydefault;

	/* Not known => get a lookahead token if don't already have one.  */

	/* yychar is either YYEMPTY or YYEOF
	or a valid token in external form.  */

	if (yychar == YYEMPTY)
	{
#if YYDEBUG != 0
		if (yydebug)
			fprintf(stderr, "Reading a token: ");
#endif
		yychar = YYLEX;
	}

	/* Convert token to internal form (in yychar1) for indexing tables with */

	if (yychar <= 0)		/* This means end of input. */
	{
		yychar1 = 0;
		yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
		if (yydebug)
			fprintf(stderr, "Now at end of input.\n");
#endif
	}
	else
	{
		yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
		if (yydebug)
		{
			fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
			/* Give the individual parser a way to print the precise meaning
			of a token, for further debugging info.  */
#ifdef YYPRINT
			YYPRINT (stderr, yychar, yylval);
#endif
			fprintf (stderr, ")\n");
		}
#endif
	}

	yyn += yychar1;
	if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
		goto yydefault;

	yyn = yytable[yyn];

	/* yyn is what to do for this token type in this state.
	Negative => reduce, -yyn is rule number.
	Positive => shift, yyn is new state.
	New state is final state => don't bother to shift,
	just return success.
	0, or most negative number => error.  */

	if (yyn < 0)
	{
		if (yyn == YYFLAG)
			goto yyerrlab;
		yyn = -yyn;
		goto yyreduce;
	}
	else if (yyn == 0)
		goto yyerrlab;

	if (yyn == YYFINAL)
		YYACCEPT;

	/* Shift the lookahead token.  */

#if YYDEBUG != 0
	if (yydebug)
		fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

	/* Discard the token being shifted unless it is eof.  */
	if (yychar != YYEOF)
		yychar = YYEMPTY;

	*++yyvsp = yylval;
#ifdef YYLSP_NEEDED
	*++yylsp = yylloc;
#endif

	/* count tokens shifted since error; after three, turn off error status.  */
	if (yyerrstatus) yyerrstatus--;

	yystate = yyn;
	goto yynewstate;

	/* Do the default action for the current state.  */
yydefault:

	yyn = yydefact[yystate];
	if (yyn == 0)
		goto yyerrlab;

	/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
	yylen = yyr2[yyn];
	if (yylen > 0)
		yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
	if (yydebug)
	{
		int i;

		fprintf (stderr, "Reducing via rule %d (line %d), ",
			yyn, yyrline[yyn]);

		/* Print the symbols being reduced, and their result.  */
		for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
			fprintf (stderr, "%s ", yytname[yyrhs[i]]);
		fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
	}
#endif


	switch (yyn) {

case 2:
	{	QParser->OnScript(yyvsp[-1].OP);	;
	break;}
case 4:
	{
		QParser->OnExpression(yyvsp[-4].OP); 
		;
		break;}
case 6:
	{	yyval.OP=yyvsp[0].OP;	;
	break;}
case 7:
	{	yyval.OP=QParser->OnOperandBlock(yyvsp[-1].OP);	;
	break;}
case 9:
	{	yyval.OP=QParser->OnFunctionList(yyvsp[0].OP);	;
	break;}
case 10:
	{	yyval.OP=QParser->OnFunctionList(yyvsp[-1].OP,yyvsp[0].OP);	;
	break;}
case 11:
	{	yyval.OP=QParser->OnDeclareVariable(yyvsp[-2].str,yyvsp[-1].str);	;
	break;}
case 12:
	{	yyval.OP=QParser->OnAssigment(yyvsp[-3].OP,yyvsp[-1].OP);	;
	break;}
case 13:
	{	yyval.OP=QParser->OnIf(yyvsp[-2].OP,yyvsp[0].OP);	;
	break;}
case 14:
	{	yyval.OP=QParser->OnIfElse(yyvsp[-4].OP,yyvsp[-2].OP,yyvsp[0].OP);	;
	break;}
case 15:
	{	yyval.OP=QParser->OnProcedure(yyvsp[-4].str,yyvsp[-2].OP);	;
	break;}
case 16:
	{	yyval.OP=yyvsp[0].OP;	;
	break;}
case 17:
	{	yyval.OP=QParser->OnCompareFun(yyvsp[-2].OP,2,yyvsp[0].OP);    ;
	break;}
case 18:
	{	yyval.OP=QParser->OnCompareFun(yyvsp[-2].OP,1,yyvsp[0].OP);    ;
	break;}
case 19:
	{	yyval.OP=QParser->OnCompareFun(yyvsp[-2].OP,3,yyvsp[0].OP);    ;
	break;}
case 20:
	{	yyval.OP=QParser->OnCompareFun(yyvsp[-2].OP,4,yyvsp[0].OP);    ;
	break;}
case 21:
	{	yyval.OP=QParser->OnCompareFun(yyvsp[-2].OP,0,yyvsp[0].OP);    ;
	break;}
case 22:
	{	yyval.OP=QParser->OnCompareFun(yyvsp[-2].OP,5,yyvsp[0].OP);    ;
	break;}
case 23:
	{	yyval.OP=QParser->OnBracket(yyvsp[-1].OP);	;
	break;}
case 25:
	{	yyval.OP=QParser->OnLogical(yyvsp[-2].OP,0,yyvsp[0].OP);    ;
	break;}
case 26:
	{	yyval.OP=QParser->OnLogical(yyvsp[-2].OP,1,yyvsp[0].OP);    ;
	break;}
case 27:
	{	yyval.OP=0;	;
	break;}
case 28:
	{	yyval.OP=QParser->OnAddFunctionArgument(yyvsp[0].OP);	;
	break;}
case 29:
	{ yyval.OP=QParser->OnAddFunctionArgument(yyvsp[0].OP,yyvsp[-2].OP);	;
	break;}
case 30:
	{	yyval.OP=QParser->OnIntegerConst(yyvsp[0].str);	;
	break;}
case 31:
	{	yyval.OP=QParser->OnStringConst(yyvsp[0].str);	;
	break;}
case 32:
	{	yyval.OP=QParser->OnFunction(yyvsp[-3].str,yyvsp[-1].OP);	;
	break;}
case 33:
	{	yyval.OP=QParser->OnFunction2(yyvsp[-2].str,yyvsp[0].str);	;
	break;}
case 34:
	{	yyval.OP=QParser->OnArifmetical(yyvsp[-2].OP,0,yyvsp[0].OP);    ;
	break;}
case 35:
	{	yyval.OP=QParser->OnArifmetical(yyvsp[-2].OP,1,yyvsp[0].OP);    ;
	break;}
case 36:
	{	yyval.OP=QParser->OnArifmetical(yyvsp[-2].OP,2,yyvsp[0].OP);    ;
	break;}
case 37:
	{	yyval.OP=QParser->OnArifmetical(yyvsp[-2].OP,3,yyvsp[0].OP);    ;
	break;}
case 38:
	{	yyval.OP=QParser->OnUnarMinus(yyvsp[0].OP);    ;
	break;}
case 39:
	{	yyval.OP=QParser->OnBracket(yyvsp[-1].OP);      ;
	break;}
case 40:
	{	yyval.OP=QParser->OnVariable(yyvsp[0].str);	;
	break;}
	}
	/* the action file gets copied in in place of this dollarsign */

	
	yyvsp -= yylen;
	yyssp -= yylen;
#ifdef YYLSP_NEEDED
	yylsp -= yylen;
#endif

#if YYDEBUG != 0
	if (yydebug)
	{
		short *ssp1 = yyss - 1;
		fprintf (stderr, "state stack now");
		while (ssp1 != yyssp)
			fprintf (stderr, " %d", *++ssp1);
		fprintf (stderr, "\n");
	}
#endif

	*++yyvsp = yyval;

#ifdef YYLSP_NEEDED
	yylsp++;
	if (yylen == 0)
	{
		yylsp->first_line = yylloc.first_line;
		yylsp->first_column = yylloc.first_column;
		yylsp->last_line = (yylsp-1)->last_line;
		yylsp->last_column = (yylsp-1)->last_column;
		yylsp->text = 0;
	}
	else
	{
		yylsp->last_line = (yylsp+yylen-1)->last_line;
		yylsp->last_column = (yylsp+yylen-1)->last_column;
	}
#endif

	/* Now "shift" the result of the reduction.
	Determine what state that goes to,
	based on the state we popped back to
	and the rule number reduced by.  */

	yyn = yyr1[yyn];

	yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
	if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
		yystate = yytable[yystate];
	else
		yystate = yydefgoto[yyn - YYNTBASE];

	goto yynewstate;

yyerrlab:   /* here on detecting error */

	if (! yyerrstatus)
		/* If not already recovering from an error, report this error.  */
	{
		++yynerrs;

#ifdef YYERROR_VERBOSE
		yyn = yypact[yystate];

		if (yyn > YYFLAG && yyn < YYLAST)
		{
			int size = 0;
			char *msg;
			int x, count;

			count = 0;
			/* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
			for (x = (yyn < 0 ? -yyn : 0);
				x < (sizeof(yytname) / sizeof(char *)); x++)
				if (yycheck[x + yyn] == x)
					size += strlen(yytname[x]) + 15, count++;
			msg = (char *) malloc(size + 15);
			if (msg != 0)
			{
				strcpy(msg, "parse error");

				if (count < 5)
				{
					count = 0;
					for (x = (yyn < 0 ? -yyn : 0);
						x < (sizeof(yytname) / sizeof(char *)); x++)
						if (yycheck[x + yyn] == x)
						{
							strcat(msg, count == 0 ? ", expecting `" : " or `");
							strcat(msg, yytname[x]);
							strcat(msg, "'");
							count++;
						}
				}
				yyerror(msg);
				free(msg);
			}
			else
				yyerror ("parse error; also virtual memory exceeded");
		}
		else
#endif /* YYERROR_VERBOSE */
			yyerror("parse error");
	}

	goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

	if (yyerrstatus == 3)
	{
		/* if just tried and failed to reuse lookahead token after an error, discard it.  */

		/* return failure if at end of input */
		if (yychar == YYEOF)
			YYABORT;

#if YYDEBUG != 0
		if (yydebug)
			fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

		yychar = YYEMPTY;
	}

	/* Else will try to reuse lookahead token
	after shifting the error token.  */

	yyerrstatus = 3;		/* Each real token shifted decrements this */

	goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
	/* This is wrong; only states that explicitly want error tokens
	should shift them.  */
	yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
	if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

	if (yyssp == yyss) YYABORT;
	yyvsp--;
	yystate = *--yyssp;
#ifdef YYLSP_NEEDED
	yylsp--;
#endif

#if YYDEBUG != 0
	if (yydebug)
	{
		short *ssp1 = yyss - 1;
		fprintf (stderr, "Error: state stack now");
		while (ssp1 != yyssp)
			fprintf (stderr, " %d", *++ssp1);
		fprintf (stderr, "\n");
	}
#endif

yyerrhandle:

	yyn = yypact[yystate];
	if (yyn == YYFLAG)
		goto yyerrdefault;

	yyn += YYTERROR;
	if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
		goto yyerrdefault;

	yyn = yytable[yyn];
	if (yyn < 0)
	{
		if (yyn == YYFLAG)
			goto yyerrpop;
		yyn = -yyn;
		goto yyreduce;
	}
	else if (yyn == 0)
		goto yyerrpop;

	if (yyn == YYFINAL)
		YYACCEPT;

#if YYDEBUG != 0
	if (yydebug)
		fprintf(stderr, "Shifting error token, ");
#endif

	*++yyvsp = yylval;
#ifdef YYLSP_NEEDED
	*++yylsp = yylloc;
#endif

	yystate = yyn;
	goto yynewstate;
}

/* A lexical scanner generated by flex */

/* Scanner skeleton version:
* $Header: /home/daffy/u0/vern/flex/RCS/flex.skl,v 2.85 95/04/24 10:48:47 vern Exp $
*/

#define FLEX_SCANNER
#define YY_FLEX_MAJOR_VERSION 2
#define YY_FLEX_MINOR_VERSION 5

#include <stdio.h>


/* cfront 1.2 defines "c_plusplus" instead of "__cplusplus" */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif


#ifdef __cplusplus

#include <stdlib.h>

/* Use prototypes in function declarations. */
#define YY_USE_PROTOS

/* The "const" storage-class-modifier is valid. */
#define YY_USE_CONST

#else	/* ! __cplusplus */

#if __STDC__

#define YY_USE_PROTOS
#define YY_USE_CONST

#endif	/* __STDC__ */
#endif	/* ! __cplusplus */

#ifdef __TURBOC__
#pragma warn -rch
#pragma warn -use
#include <io.h>
#include <stdlib.h>
#define YY_USE_CONST
#define YY_USE_PROTOS
#endif

#ifdef YY_USE_CONST
#define yyconst const
#else
#define yyconst
#endif


#ifdef YY_USE_PROTOS
#define YY_PROTO(proto) proto
#else
#define YY_PROTO(proto) ()
#endif

/* Returned upon end-of-file. */
#define YY_NULL 0

/* Promotes a possibly negative, possibly signed char to an unsigned
* integer for use as an array index.  If the signed char is negative,
* we want to instead treat it as an 8-bit unsigned char, hence the
* double cast.
*/
#define YY_SC_TO_UI(c) ((unsigned int) (unsigned char) c)

/* Enter a start condition.  This macro really ought to take a parameter,
* but we do it the disgusting crufty way forced on us by the ()-less
* definition of BEGIN.
*/
#define BEGIN yy_start = 1 + 2 *

/* Translate the current start state into a value that can be later handed
* to BEGIN to return to the state.  The YYSTATE alias is for lex
* compatibility.
*/
#define YY_START ((yy_start - 1) / 2)
#define YYSTATE YY_START

/* Action number for EOF rule of a given start state. */
#define YY_STATE_EOF(state) (YY_END_OF_BUFFER + state + 1)

/* Special action meaning "start processing a new file". */
#define YY_NEW_FILE yyrestart( yyin )

#define YY_END_OF_BUFFER_CHAR 0

/* Size of default input buffer. */
#define YY_BUF_SIZE 16384

typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern int yyleng;
extern FILE *yyin, *yyout;

#define EOB_ACT_CONTINUE_SCAN 0
#define EOB_ACT_END_OF_FILE 1
#define EOB_ACT_LAST_MATCH 2

/* The funky do-while in the following #define is used to turn the definition
* int a single C statement (which needs a semi-colon terminator).  This
* avoids problems with code like:
*
* 	if ( condition_holds )
*		yyless( 5 );
*	else
*		do_something_else();
*
* Prior to using the do-while the compiler would get upset at the
* "else" because it interpreted the "if" statement as being all
* done when it reached the ';' after the yyless() call.
*/

/* Return all but the first 'n' matched characters back to the input stream. */

#define yyless(n) \
	do \
{ \
	/* Undo effects of setting up yytext. */ \
	*yy_cp = yy_hold_char; \
	yy_c_buf_p = yy_cp = yy_bp + n - YY_MORE_ADJ; \
	YY_DO_BEFORE_ACTION; /* set up yytext again */ \
} \
	while ( 0 )

#define unput(c) yyunput( c, yytext_ptr )

/* The following is because we cannot portably get our hands on size_t
* (without autoconf's help, which isn't available because we want
* flex-generated scanners to compile on their own).
*/
typedef unsigned int yy_size_t;


struct yy_buffer_state
{
	FILE *yy_input_file;

	char *yy_ch_buf;		/* input buffer */
	char *yy_buf_pos;		/* current position in input buffer */

	/* Size of input buffer in bytes, not including room for EOB
	* characters.
	*/
	yy_size_t yy_buf_size;

	/* Number of characters read into yy_ch_buf, not including EOB
	* characters.
	*/
	int yy_n_chars;

	/* Whether we "own" the buffer - i.e., we know we created it,
	* and can realloc() it to grow it, and should free() it to
	* delete it.
	*/
	int yy_is_our_buffer;

	/* Whether this is an "interactive" input source; if so, and
	* if we're using stdio for input, then we want to use getc()
	* instead of fread(), to make sure we stop fetching input after
	* each newline.
	*/
	int yy_is_interactive;

	/* Whether we're considered to be at the beginning of a line.
	* If so, '^' rules will be active on the next match, otherwise
	* not.
	*/
	int yy_at_bol;

	/* Whether to try to fill the input buffer when we reach the
	* end of it.
	*/
	int yy_fill_buffer;

	int yy_buffer_status;
#define YY_BUFFER_NEW 0
#define YY_BUFFER_NORMAL 1
	/* When an EOF's been seen but there's still some text to process
	* then we mark the buffer as YY_EOF_PENDING, to indicate that we
	* shouldn't try reading from the input source any more.  We might
	* still have a bunch of tokens to match, though, because of
	* possible backing-up.
	*
	* When we actually see the EOF, we change the status to "new"
	* (via yyrestart()), so that the user can continue scanning by
	* just pointing yyin at a new input file.
	*/
#define YY_BUFFER_EOF_PENDING 2
};

static YY_BUFFER_STATE yy_current_buffer = 0;

/* We provide macros for accessing buffer states in case in the
* future we want to put the buffer states in a more general
* "scanner state".
*/
#define YY_CURRENT_BUFFER yy_current_buffer


/* yy_hold_char holds the character lost when yytext is formed. */
static char yy_hold_char;

static int yy_n_chars;		/* number of characters read into yy_ch_buf */


int yyleng;

/* Points to current character in buffer. */
char *yy_c_buf_p = (char *) 0;
int yy_init = 1;		/* whether we need to initialize */
int yy_start = 0;	/* start state number */

/* Flag which is used to allow yywrap()'s to do buffer switches
* instead of setting up a fresh yyin.  A bit of a hack ...
*/
static int yy_did_buffer_switch_on_eof;

void yyrestart YY_PROTO(( FILE *input_file ));

void yy_switch_to_buffer YY_PROTO(( YY_BUFFER_STATE new_buffer ));
void yy_load_buffer_state YY_PROTO(( void ));
YY_BUFFER_STATE yy_create_buffer YY_PROTO(( FILE *file, int size ));
void yy_delete_buffer YY_PROTO(( YY_BUFFER_STATE b ));
void yy_init_buffer YY_PROTO(( YY_BUFFER_STATE b, FILE *file ));
void yy_flush_buffer YY_PROTO(( YY_BUFFER_STATE b ));
#define YY_FLUSH_BUFFER yy_flush_buffer( yy_current_buffer )

YY_BUFFER_STATE yy_scan_buffer YY_PROTO(( char *base, yy_size_t size ));
YY_BUFFER_STATE yy_scan_string YY_PROTO(( yyconst char *str ));
YY_BUFFER_STATE yy_scan_bytes YY_PROTO(( yyconst char *bytes, int len ));

static void *yy_flex_alloc YY_PROTO(( yy_size_t ));
static void *yy_flex_realloc YY_PROTO(( void *, yy_size_t ));
static void yy_flex_free YY_PROTO(( void * ));

#define yy_new_buffer yy_create_buffer

#define yy_set_interactive(is_interactive) \
{ \
	if ( ! yy_current_buffer ) \
	yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE ); \
	yy_current_buffer->yy_is_interactive = is_interactive; \
}

#define yy_set_bol(at_bol) \
{ \
	if ( ! yy_current_buffer ) \
	yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE ); \
	yy_current_buffer->yy_at_bol = at_bol; \
}

#define YY_AT_BOL() (yy_current_buffer->yy_at_bol)

typedef unsigned char YY_CHAR;
FILE *yyin = (FILE *) 0, *yyout = (FILE *) 0;
typedef int yy_state_type;
extern char *yytext;
#define yytext_ptr yytext

static yy_state_type yy_get_previous_state YY_PROTO(( void ));
static yy_state_type yy_try_NUL_trans YY_PROTO(( yy_state_type current_state ));
static int yy_get_next_buffer YY_PROTO(( void ));
static void yy_fatal_error YY_PROTO(( yyconst char msg[] ));

/* Done after the current pattern has been matched and before the
* corresponding action - sets up yytext.
*/
#define YY_DO_BEFORE_ACTION \
	yytext_ptr = yy_bp; \
	yyleng = (int) (yy_cp - yy_bp); \
	yy_hold_char = *yy_cp; \
	*yy_cp = '\0'; \
	yy_c_buf_p = yy_cp;

#define YY_NUM_RULES 33
#define YY_END_OF_BUFFER 34
static yyconst short int yy_accept[62] =
{   0,
0,    0,   34,   32,   33,    5,   32,   32,   15,   16,
22,   19,   18,   20,   17,   21,   31,   24,   27,   13,
12,   14,   30,   30,   30,   30,   25,   32,   26,    9,
0,   29,    3,    0,    0,   31,   23,    7,    8,    6,
30,   30,   30,   10,    4,    0,    0,    0,    1,   30,
30,    2,   30,   11,   30,   30,   30,   30,   30,   28,
0
} ;

static yyconst int yy_ec[256] =
{   0,
1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    3,    4,    1,    1,    1,    5,    1,    6,
7,    8,    9,   10,   11,   12,   13,   14,   14,   14,
14,   14,   14,   14,   14,   14,   14,   15,   16,   17,
18,   19,    1,    1,   20,   21,   20,   20,   20,   20,
20,   20,   20,   20,   20,   20,   20,   20,   20,   20,
20,   20,   20,   20,   20,   22,   20,   20,   20,   20,
1,    1,    1,    1,   20,    1,   23,   20,   20,   24,

25,   26,   27,   20,   28,   20,   20,   29,   20,   20,
20,   20,   20,   30,   31,   20,   20,   20,   20,   20,
20,   20,   32,   33,   34,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,

1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1
} ;

static yyconst int yy_meta[35] =
{   0,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    2,    1,    1,    1,    1,    1,    2,
2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
2,    1,    1,    1
} ;

static yyconst short int yy_base[65] =
{   0,
0,    0,   75,   76,   76,   56,   69,   67,   76,   76,
76,   76,   76,   76,   76,   27,   57,   55,   76,   51,
50,   49,    0,   36,   36,   38,   76,   30,   76,   76,
58,   76,   76,   34,   59,   46,   76,   76,   76,   76,
0,   31,   27,    0,   76,   35,   44,   54,   76,   28,
29,   76,   30,    0,   28,   20,   22,   18,    9,    0,
76,   47,   36,   49
} ;

static yyconst short int yy_def[65] =
{   0,
61,    1,   61,   61,   61,   61,   62,   61,   61,   61,
61,   61,   61,   61,   61,   61,   61,   61,   61,   61,
61,   61,   63,   63,   63,   63,   61,   61,   61,   61,
62,   61,   61,   61,   64,   61,   61,   61,   61,   61,
63,   63,   63,   63,   61,   61,   61,   64,   61,   63,
63,   61,   63,   63,   63,   63,   63,   63,   63,   63,
0,   61,   61,   61
} ;

static yyconst short int yy_nxt[111] =
{   0,
4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
24,   23,   23,   23,   25,   23,   23,   26,   23,   23,
23,   27,   28,   29,   34,   46,   46,   41,   60,   35,
59,   47,   47,   58,   57,   46,   46,   31,   31,   48,
48,   56,   55,   54,   53,   49,   52,   51,   50,   36,
49,   32,   45,   44,   43,   42,   40,   39,   38,   37,
36,   33,   32,   30,   61,    3,   61,   61,   61,   61,
61,   61,   61,   61,   61,   61,   61,   61,   61,   61,
61,   61,   61,   61,   61,   61,   61,   61,   61,   61,

61,   61,   61,   61,   61,   61,   61,   61,   61,   61
} ;

static yyconst short int yy_chk[111] =
{   0,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
1,    1,    1,    1,   16,   34,   46,   63,   59,   16,
58,   34,   46,   57,   56,   34,   46,   62,   62,   64,
64,   55,   53,   51,   50,   48,   47,   43,   42,   36,
35,   31,   28,   26,   25,   24,   22,   21,   20,   18,
17,    8,    7,    6,    3,   61,   61,   61,   61,   61,
61,   61,   61,   61,   61,   61,   61,   61,   61,   61,
61,   61,   61,   61,   61,   61,   61,   61,   61,   61,

61,   61,   61,   61,   61,   61,   61,   61,   61,   61
} ;

static yy_state_type yy_last_accepting_state;
static char *yy_last_accepting_cpos;

/* The intent behind this definition is that it'll catch
* any uses of REJECT which flex missed.
*/
#define REJECT reject_used_but_not_detected
#define yymore() yymore_used_but_not_detected
#define YY_MORE_ADJ 0
char *yytext;
#define INITIAL 0
#define YY_INPUT(buf,result,max_size) \
{ \
	result = QParser->yyInput( buf, max_size ); \
}

#define yywrap() 1


/* Macros after this point can all be overridden by user definitions in
* section 1.
*/

#ifndef YY_SKIP_YYWRAP
#ifdef __cplusplus
extern "C" int yywrap YY_PROTO(( void ));
#else
extern int yywrap YY_PROTO(( void ));
#endif
#endif

#ifndef YY_NO_UNPUT
static void yyunput YY_PROTO(( int c, char *buf_ptr ));
#endif

#ifndef yytext_ptr
static void yy_flex_strncpy YY_PROTO(( char *, yyconst char *, int ));
#endif

#ifndef YY_NO_INPUT
#ifdef __cplusplus
static int yyinput YY_PROTO(( void ));
#else
static int input YY_PROTO(( void ));
#endif
#endif

#if YY_STACK_USED
static int yy_start_stack_ptr = 0;
static int yy_start_stack_depth = 0;
static int *yy_start_stack = 0;
#ifndef YY_NO_PUSH_STATE
static void yy_push_state YY_PROTO(( int new_state ));
#endif
#ifndef YY_NO_POP_STATE
static void yy_pop_state YY_PROTO(( void ));
#endif
#ifndef YY_NO_TOP_STATE
static int yy_top_state YY_PROTO(( void ));
#endif

#else
#define YY_NO_PUSH_STATE 1
#define YY_NO_POP_STATE 1
#define YY_NO_TOP_STATE 1
#endif

#ifdef YY_MALLOC_DECL
YY_MALLOC_DECL
#else
#if __STDC__
#ifndef __cplusplus
#include <stdlib.h>
#endif
#else
/* Just try to get by without declaring the routines.  This will fail
* miserably on non-ANSI systems for which sizeof(size_t) != sizeof(int)
* or sizeof(void*) != sizeof(int).
*/
#endif
#endif

/* Amount of stuff to slurp up with each read. */
#ifndef YY_READ_BUF_SIZE
#define YY_READ_BUF_SIZE 8192
#endif

/* Copy whatever the last rule matched to the standard output. */

#ifndef ECHO
/* This used to be an fputs(), but since the string might contain NUL's,
* we now use fwrite().
*/
#define ECHO (void) fwrite( yytext, yyleng, 1, yyout )
#endif

/* Gets input and stuffs it into "buf".  number of characters read, or YY_NULL,
* is returned in "result".
*/
#ifndef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( yy_current_buffer->yy_is_interactive ) \
{ \
	int c = '*', n; \
	for ( n = 0; n < max_size && \
	(c = getc( yyin )) != EOF && c != '\n'; ++n ) \
	buf[n] = (char) c; \
	if ( c == '\n' ) \
	buf[n++] = (char) c; \
	if ( c == EOF && ferror( yyin ) ) \
	YY_FATAL_ERROR( "input in flex scanner failed" ); \
	result = n; \
} \
	else if ( ((result = fread( buf, 1, max_size, yyin )) == 0) \
	&& ferror( yyin ) ) \
	YY_FATAL_ERROR( "input in flex scanner failed" );
#endif

/* No semi-colon after return; correct usage is to write "yyterminate();" -
* we don't want an extra ';' after the "return" because that will cause
* some compilers to complain about unreachable statements.
*/
#ifndef yyterminate
#define yyterminate() return YY_NULL
#endif

/* Number of entries by which start-condition stack grows. */
#ifndef YY_START_STACK_INCR
#define YY_START_STACK_INCR 25
#endif

/* Report a fatal error. */
#ifndef YY_FATAL_ERROR
#define YY_FATAL_ERROR(msg) yy_fatal_error( msg )
#endif

/* Default declaration of generated scanner - a define so the user can
* easily add parameters.
*/
#ifndef YY_DECL
#define YY_DECL int yylex YY_PROTO(( void ))
#endif

/* Code executed at the beginning of each rule, after yytext and yyleng
* have been set up.
*/
#ifndef YY_USER_ACTION
#define YY_USER_ACTION
#endif

/* Code executed at the end of each rule. */
#ifndef YY_BREAK
#define YY_BREAK break;
#endif

#define YY_RULE_SETUP \
	YY_USER_ACTION

YY_DECL
{
	register yy_state_type yy_current_state;
	register char *yy_cp, *yy_bp;
	register int yy_act;




	if ( yy_init )
	{
		yy_init = 0;

#ifdef YY_USER_INIT
		YY_USER_INIT;
#endif

		if ( ! yy_start )
			yy_start = 1;	/* first start state */

		if ( ! yyin )
			yyin = stdin;

		if ( ! yyout )
			yyout = stdout;

		if ( ! yy_current_buffer )
			yy_current_buffer =
			yy_create_buffer( yyin, YY_BUF_SIZE );

		yy_load_buffer_state();
	}

	while ( 1 )		/* loops until end-of-file is reached */
	{
		yy_cp = yy_c_buf_p;

		/* Support of yytext. */
		*yy_cp = yy_hold_char;

		/* yy_bp points to the position in yy_ch_buf of the start of
		* the current run.
		*/
		yy_bp = yy_cp;

		yy_current_state = yy_start;
yy_match:
		do
		{
			register YY_CHAR yy_c = yy_ec[YY_SC_TO_UI(*yy_cp)];
			if ( yy_accept[yy_current_state] )
			{
				yy_last_accepting_state = yy_current_state;
				yy_last_accepting_cpos = yy_cp;
			}
			while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
			{
				yy_current_state = (int) yy_def[yy_current_state];
				if ( yy_current_state >= 62 )
					yy_c = yy_meta[(unsigned int) yy_c];
			}
			yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
			++yy_cp;
		}
		while ( yy_base[yy_current_state] != 76 );

yy_find_action:
		yy_act = yy_accept[yy_current_state];
		if ( yy_act == 0 )
		{ /* have to back up */
			yy_cp = yy_last_accepting_cpos;
			yy_current_state = yy_last_accepting_state;
			yy_act = yy_accept[yy_current_state];
		}

		YY_DO_BEFORE_ACTION;


do_action:	/* This label is used only to access EOF actions. */


		switch ( yy_act )
		{ /* beginning of action switch */
		case 0: /* must back up */
			/* undo the effects of YY_DO_BEFORE_ACTION */
			*yy_cp = yy_hold_char;
			yy_cp = yy_last_accepting_cpos;
			yy_current_state = yy_last_accepting_state;
			goto yy_find_action;

		case 1:
			YY_RULE_SETUP
			{   /* comment */   }
			YY_BREAK
		case 2:
			YY_RULE_SETUP
			{   /* comment */   }
			YY_BREAK
		case 3:
			YY_RULE_SETUP
				return tAnd;
			YY_BREAK
		case 4:
			YY_RULE_SETUP
				return tOr;
			YY_BREAK
		case 5:
			YY_RULE_SETUP
				return tNot;
			YY_BREAK
		case 6:
			YY_RULE_SETUP
				return tGEq;
			YY_BREAK
		case 7:
			YY_RULE_SETUP
				return tLEq;
			YY_BREAK
		case 8:
			YY_RULE_SETUP
				return tEq;
			YY_BREAK
		case 9:
			YY_RULE_SETUP
				return tNEq;
			YY_BREAK
		case 10:
			YY_RULE_SETUP
				return tIF;
			YY_BREAK
		case 11:
			YY_RULE_SETUP
				return tELSE;
			YY_BREAK
		case 12:
			YY_RULE_SETUP
				return tAssignment;			
			YY_BREAK
		case 13:
			YY_RULE_SETUP
				return tLeftBracket;		
			YY_BREAK
		case 14:
			YY_RULE_SETUP
				return tRightBracket;	
			YY_BREAK
		case 15:
			YY_RULE_SETUP
				return tLParen;		
			YY_BREAK
		case 16:
			YY_RULE_SETUP
				return tRParen;		
			YY_BREAK
		case 17:
			YY_RULE_SETUP
				return tDot;		
			YY_BREAK
		case 18:
			YY_RULE_SETUP
				return tComma;		
			YY_BREAK
		case 19:
			YY_RULE_SETUP
				return tPlus;		
			YY_BREAK
		case 20:
			YY_RULE_SETUP
				return tMinus;		
			YY_BREAK
		case 21:
			YY_RULE_SETUP
				return tSlesh;		
			YY_BREAK
		case 22:
			YY_RULE_SETUP
				return tStar;		
			YY_BREAK
		case 23:
			YY_RULE_SETUP
				return t4Dot;		
			YY_BREAK
		case 24:
			YY_RULE_SETUP
				return t2Dot;		
			YY_BREAK
		case 25:
			YY_RULE_SETUP
				return tLQt;		
			YY_BREAK
		case 26:
			YY_RULE_SETUP
				return tRQt;	
			YY_BREAK
		case 27:
			YY_RULE_SETUP
				return tDotComma;	
			YY_BREAK
		case 28:
			YY_RULE_SETUP
				return tBrigadeVar;
			YY_BREAK
		case 29:
			YY_RULE_SETUP
			{	
				yylval.str.SetString( yytext + 1, yyleng - 2 );
				return tQuotedString;		
			}
			YY_BREAK
		case 30:
			YY_RULE_SETUP
			{	
				yylval.str.SetString( yytext, yyleng );
				return tID;					
			}
			YY_BREAK
		case 31:
			YY_RULE_SETUP
			{	
				yylval.str.SetString( yytext, yyleng );
				return tInteger;	
			}
			YY_BREAK
		case 32:
			YY_RULE_SETUP
			{	/*  skip anything else	*/	}
			YY_BREAK
		case 33:
			YY_RULE_SETUP
				ECHO;
			YY_BREAK
		case YY_STATE_EOF(INITIAL):
			yyterminate();

		case YY_END_OF_BUFFER:
			{
				/* Amount of text matched not including the EOB char. */
				int yy_amount_of_matched_text = (int) (yy_cp - yytext_ptr) - 1;

				/* Undo the effects of YY_DO_BEFORE_ACTION. */
				*yy_cp = yy_hold_char;

				if ( yy_current_buffer->yy_buffer_status == YY_BUFFER_NEW )
				{
					/* We're scanning a new file or input source.  It's
					* possible that this happened because the user
					* just pointed yyin at a new source and called
					* yylex().  If so, then we have to assure
					* consistency between yy_current_buffer and our
					* globals.  Here is the right place to do so, because
					* this is the first action (other than possibly a
					* back-up) that will match for the new input source.
					*/
					yy_n_chars = yy_current_buffer->yy_n_chars;
					yy_current_buffer->yy_input_file = yyin;
					yy_current_buffer->yy_buffer_status = YY_BUFFER_NORMAL;
				}

				/* Note that here we test for yy_c_buf_p "<=" to the position
				* of the first EOB in the buffer, since yy_c_buf_p will
				* already have been incremented past the NUL character
				* (since all states make transitions on EOB to the
				* end-of-buffer state).  Contrast this with the test
				* in input().
				*/
				if ( yy_c_buf_p <= &yy_current_buffer->yy_ch_buf[yy_n_chars] )
				{ /* This was really a NUL. */
					yy_state_type yy_next_state;

					yy_c_buf_p = yytext_ptr + yy_amount_of_matched_text;

					yy_current_state = yy_get_previous_state();

					/* Okay, we're now positioned to make the NUL
					* transition.  We couldn't have
					* yy_get_previous_state() go ahead and do it
					* for us because it doesn't know how to deal
					* with the possibility of jamming (and we don't
					* want to build jamming into it because then it
					* will run more slowly).
					*/

					yy_next_state = yy_try_NUL_trans( yy_current_state );

					yy_bp = yytext_ptr + YY_MORE_ADJ;

					if ( yy_next_state )
					{
						/* Consume the NUL. */
						yy_cp = ++yy_c_buf_p;
						yy_current_state = yy_next_state;
						goto yy_match;
					}

					else
					{
						yy_cp = yy_c_buf_p;
						goto yy_find_action;
					}
				}

				else switch ( yy_get_next_buffer() )
				{
		case EOB_ACT_END_OF_FILE:
			{
				yy_did_buffer_switch_on_eof = 0;

				if ( yywrap() )
				{
					/* Note: because we've taken care in
					* yy_get_next_buffer() to have set up
					* yytext, we can now set up
					* yy_c_buf_p so that if some total
					* hoser (like flex itself) wants to
					* call the scanner after we return the
					* YY_NULL, it'll still work - another
					* YY_NULL will get returned.
					*/
					yy_c_buf_p = yytext_ptr + YY_MORE_ADJ;

					yy_act = YY_STATE_EOF(YY_START);
					goto do_action;
				}

				else
				{
					if ( ! yy_did_buffer_switch_on_eof )
						YY_NEW_FILE;
				}
				break;
			}

		case EOB_ACT_CONTINUE_SCAN:
			yy_c_buf_p =
				yytext_ptr + yy_amount_of_matched_text;

			yy_current_state = yy_get_previous_state();

			yy_cp = yy_c_buf_p;
			yy_bp = yytext_ptr + YY_MORE_ADJ;
			goto yy_match;

		case EOB_ACT_LAST_MATCH:
			yy_c_buf_p =
				&yy_current_buffer->yy_ch_buf[yy_n_chars];

			yy_current_state = yy_get_previous_state();

			yy_cp = yy_c_buf_p;
			yy_bp = yytext_ptr + YY_MORE_ADJ;
			goto yy_find_action;
				}
				break;
			}

		default:
			YY_FATAL_ERROR(
				"fatal flex scanner internal error--no action found" );
		} /* end of action switch */
	} /* end of scanning one token */
} /* end of yylex */


/* yy_get_next_buffer - try to read in a new buffer
*
* Returns a code representing an action:
*	EOB_ACT_LAST_MATCH -
*	EOB_ACT_CONTINUE_SCAN - continue scanning from current position
*	EOB_ACT_END_OF_FILE - end of file
*/

static int yy_get_next_buffer()
{
	register char *dest = yy_current_buffer->yy_ch_buf;
	register char *source = yytext_ptr;
	register int number_to_move, i;
	int ret_val;

	if ( yy_c_buf_p > &yy_current_buffer->yy_ch_buf[yy_n_chars + 1] )
		YY_FATAL_ERROR(
		"fatal flex scanner internal error--end of buffer missed" );

	if ( yy_current_buffer->yy_fill_buffer == 0 )
	{ /* Don't try to fill the buffer, so this is an EOF. */
		if ( yy_c_buf_p - yytext_ptr - YY_MORE_ADJ == 1 )
		{
			/* We matched a singled characater, the EOB, so
			* treat this as a final EOF.
			*/
			return EOB_ACT_END_OF_FILE;
		}

		else
		{
			/* We matched some text prior to the EOB, first
			* process it.
			*/
			return EOB_ACT_LAST_MATCH;
		}
	}

	/* Try to read more data. */

	/* First move last chars to start of buffer. */
	number_to_move = (int) (yy_c_buf_p - yytext_ptr) - 1;

	for ( i = 0; i < number_to_move; ++i )
		*(dest++) = *(source++);

	if ( yy_current_buffer->yy_buffer_status == YY_BUFFER_EOF_PENDING )
		/* don't do the read, it's not guaranteed to return an EOF,
		* just force an EOF
		*/
		yy_n_chars = 0;

	else
	{
		int num_to_read =
			yy_current_buffer->yy_buf_size - number_to_move - 1;

		while ( num_to_read <= 0 )
		{ /* Not enough room in the buffer - grow it. */
#ifdef YY_USES_REJECT
			YY_FATAL_ERROR(
				"input buffer overflow, can't enlarge buffer because scanner uses REJECT" );
#else

			/* just a shorter name for the current buffer */
			YY_BUFFER_STATE b = yy_current_buffer;

			int yy_c_buf_p_offset =
				(int) (yy_c_buf_p - b->yy_ch_buf);

			if ( b->yy_is_our_buffer )
			{
				int new_size = b->yy_buf_size * 2;

				if ( new_size <= 0 )
					b->yy_buf_size += b->yy_buf_size / 8;
				else
					b->yy_buf_size *= 2;

				b->yy_ch_buf = (char *)
					/* Include room in for 2 EOB chars. */
					yy_flex_realloc( (void *) b->yy_ch_buf,
					b->yy_buf_size + 2 );
			}
			else
				/* Can't grow it, we don't own it. */
				b->yy_ch_buf = 0;

			if ( ! b->yy_ch_buf )
				YY_FATAL_ERROR(
				"fatal error - scanner input buffer overflow" );

			yy_c_buf_p = &b->yy_ch_buf[yy_c_buf_p_offset];

			num_to_read = yy_current_buffer->yy_buf_size -
				number_to_move - 1;
#endif
		}

		if ( num_to_read > YY_READ_BUF_SIZE )
			num_to_read = YY_READ_BUF_SIZE;

		/* Read in more data. */
		YY_INPUT( (&yy_current_buffer->yy_ch_buf[number_to_move]),
			yy_n_chars, num_to_read );
	}

	if ( yy_n_chars == 0 )
	{
		if ( number_to_move == YY_MORE_ADJ )
		{
			ret_val = EOB_ACT_END_OF_FILE;
			yyrestart( yyin );
		}

		else
		{
			ret_val = EOB_ACT_LAST_MATCH;
			yy_current_buffer->yy_buffer_status =
				YY_BUFFER_EOF_PENDING;
		}
	}

	else
		ret_val = EOB_ACT_CONTINUE_SCAN;

	yy_n_chars += number_to_move;
	yy_current_buffer->yy_ch_buf[yy_n_chars] = YY_END_OF_BUFFER_CHAR;
	yy_current_buffer->yy_ch_buf[yy_n_chars + 1] = YY_END_OF_BUFFER_CHAR;

	yytext_ptr = &yy_current_buffer->yy_ch_buf[0];

	return ret_val;
}


/* yy_get_previous_state - get the state just before the EOB char was reached */

static yy_state_type yy_get_previous_state()
{
	register yy_state_type yy_current_state;
	register char *yy_cp;

	yy_current_state = yy_start;

	for ( yy_cp = yytext_ptr + YY_MORE_ADJ; yy_cp < yy_c_buf_p; ++yy_cp )
	{
		register YY_CHAR yy_c = (*yy_cp ? yy_ec[YY_SC_TO_UI(*yy_cp)] : 1);
		if ( yy_accept[yy_current_state] )
		{
			yy_last_accepting_state = yy_current_state;
			yy_last_accepting_cpos = yy_cp;
		}
		while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
		{
			yy_current_state = (int) yy_def[yy_current_state];
			if ( yy_current_state >= 62 )
				yy_c = yy_meta[(unsigned int) yy_c];
		}
		yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
	}

	return yy_current_state;
}


/* yy_try_NUL_trans - try to make a transition on the NUL character
*
* synopsis
*	next_state = yy_try_NUL_trans( current_state );
*/

#ifdef YY_USE_PROTOS
static yy_state_type yy_try_NUL_trans( yy_state_type yy_current_state )
#else
static yy_state_type yy_try_NUL_trans( yy_current_state )
yy_state_type yy_current_state;
#endif
{
	register int yy_is_jam;
	register char *yy_cp = yy_c_buf_p;

	register YY_CHAR yy_c = 1;
	if ( yy_accept[yy_current_state] )
	{
		yy_last_accepting_state = yy_current_state;
		yy_last_accepting_cpos = yy_cp;
	}
	while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
	{
		yy_current_state = (int) yy_def[yy_current_state];
		if ( yy_current_state >= 62 )
			yy_c = yy_meta[(unsigned int) yy_c];
	}
	yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
	yy_is_jam = (yy_current_state == 61);

	return yy_is_jam ? 0 : yy_current_state;
}


#ifndef YY_NO_UNPUT
#ifdef YY_USE_PROTOS
static void yyunput( int c, register char *yy_bp )
#else
static void yyunput( c, yy_bp )
int c;
register char *yy_bp;
#endif
{
	register char *yy_cp = yy_c_buf_p;

	/* undo effects of setting up yytext */
	*yy_cp = yy_hold_char;

	if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
	{ /* need to shift things up to make room */
		/* +2 for EOB chars. */
		register int number_to_move = yy_n_chars + 2;
		register char *dest = &yy_current_buffer->yy_ch_buf[
			yy_current_buffer->yy_buf_size + 2];
			register char *source =
				&yy_current_buffer->yy_ch_buf[number_to_move];

			while ( source > yy_current_buffer->yy_ch_buf )
				*--dest = *--source;

			yy_cp += (int) (dest - source);
			yy_bp += (int) (dest - source);
			yy_n_chars = yy_current_buffer->yy_buf_size;

			if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
				YY_FATAL_ERROR( "flex scanner push-back overflow" );
	}

	*--yy_cp = (char) c;


	yytext_ptr = yy_bp;
	yy_hold_char = *yy_cp;
	yy_c_buf_p = yy_cp;
}
#endif	/* ifndef YY_NO_UNPUT */


#ifdef __cplusplus
static int yyinput()
#else
static int input()
#endif
{
	int c;

	*yy_c_buf_p = yy_hold_char;

	if ( *yy_c_buf_p == YY_END_OF_BUFFER_CHAR )
	{
		/* yy_c_buf_p now points to the character we want to return.
		* If this occurs *before* the EOB characters, then it's a
		* valid NUL; if not, then we've hit the end of the buffer.
		*/
		if ( yy_c_buf_p < &yy_current_buffer->yy_ch_buf[yy_n_chars] )
			/* This was really a NUL. */
			*yy_c_buf_p = '\0';

		else
		{ /* need more input */
			yytext_ptr = yy_c_buf_p;
			++yy_c_buf_p;

			switch ( yy_get_next_buffer() )
			{
			case EOB_ACT_END_OF_FILE:
				{
					if ( yywrap() )
					{
						yy_c_buf_p =
							yytext_ptr + YY_MORE_ADJ;
						return EOF;
					}

					if ( ! yy_did_buffer_switch_on_eof )
						YY_NEW_FILE;
#ifdef __cplusplus
					return yyinput();
#else
					return input();
#endif
				}

			case EOB_ACT_CONTINUE_SCAN:
				yy_c_buf_p = yytext_ptr + YY_MORE_ADJ;
				break;

			case EOB_ACT_LAST_MATCH:
#ifdef __cplusplus
				YY_FATAL_ERROR(
					"unexpected last match in yyinput()" );
#else
				YY_FATAL_ERROR(
					"unexpected last match in input()" );
#endif
			}
		}
	}

	c = *(unsigned char *) yy_c_buf_p;	/* cast for 8-bit char's */
	*yy_c_buf_p = '\0';	/* preserve yytext */
	yy_hold_char = *++yy_c_buf_p;


	return c;
}


#ifdef YY_USE_PROTOS
void yyrestart( FILE *input_file )
#else
void yyrestart( input_file )
FILE *input_file;
#endif
{
	if ( ! yy_current_buffer )
		yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE );

	yy_init_buffer( yy_current_buffer, input_file );
	yy_load_buffer_state();
}


#ifdef YY_USE_PROTOS
void yy_switch_to_buffer( YY_BUFFER_STATE new_buffer )
#else
void yy_switch_to_buffer( new_buffer )
YY_BUFFER_STATE new_buffer;
#endif
{
	if ( yy_current_buffer == new_buffer )
		return;

	if ( yy_current_buffer )
	{
		/* Flush out information for old buffer. */
		*yy_c_buf_p = yy_hold_char;
		yy_current_buffer->yy_buf_pos = yy_c_buf_p;
		yy_current_buffer->yy_n_chars = yy_n_chars;
	}

	yy_current_buffer = new_buffer;
	yy_load_buffer_state();

	/* We don't actually know whether we did this switch during
	* EOF (yywrap()) processing, but the only time this flag
	* is looked at is after yywrap() is called, so it's safe
	* to go ahead and always set it.
	*/
	yy_did_buffer_switch_on_eof = 1;
}


#ifdef YY_USE_PROTOS
void yy_load_buffer_state( void )
#else
void yy_load_buffer_state()
#endif
{
	yy_n_chars = yy_current_buffer->yy_n_chars;
	yytext_ptr = yy_c_buf_p = yy_current_buffer->yy_buf_pos;
	yyin = yy_current_buffer->yy_input_file;
	yy_hold_char = *yy_c_buf_p;
}


#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_create_buffer( FILE *file, int size )
#else
YY_BUFFER_STATE yy_create_buffer( file, size )
FILE *file;
int size;
#endif
{
	YY_BUFFER_STATE b;

	b = (YY_BUFFER_STATE) yy_flex_alloc( sizeof( struct yy_buffer_state ) );
	if ( ! b )
		YY_FATAL_ERROR( "out of dynamic memory in yy_create_buffer()" );

	b->yy_buf_size = size;

	/* yy_ch_buf has to be 2 characters longer than the size given because
	* we need to put in 2 end-of-buffer characters.
	*/
	b->yy_ch_buf = (char *) yy_flex_alloc( b->yy_buf_size + 2 );
	if ( ! b->yy_ch_buf )
		YY_FATAL_ERROR( "out of dynamic memory in yy_create_buffer()" );

	b->yy_is_our_buffer = 1;

	yy_init_buffer( b, file );

	return b;
}


#ifdef YY_USE_PROTOS
void yy_delete_buffer( YY_BUFFER_STATE b )
#else
void yy_delete_buffer( b )
YY_BUFFER_STATE b;
#endif
{
	if ( ! b )
		return;

	if ( b == yy_current_buffer )
		yy_current_buffer = (YY_BUFFER_STATE) 0;

	if ( b->yy_is_our_buffer )
		yy_flex_free( (void *) b->yy_ch_buf );

	yy_flex_free( (void *) b );
}


#ifndef YY_ALWAYS_INTERACTIVE
#ifndef YY_NEVER_INTERACTIVE
extern int isatty YY_PROTO(( int ));
#endif
#endif

#ifdef YY_USE_PROTOS
void yy_init_buffer( YY_BUFFER_STATE b, FILE *file )
#else
void yy_init_buffer( b, file )
YY_BUFFER_STATE b;
FILE *file;
#endif


{
	yy_flush_buffer( b );

	b->yy_input_file = file;
	b->yy_fill_buffer = 1;

#if YY_ALWAYS_INTERACTIVE
	b->yy_is_interactive = 1;
#else
#if YY_NEVER_INTERACTIVE
	b->yy_is_interactive = 0;
#else
	b->yy_is_interactive = file ? (isatty( fileno(file) ) > 0) : 0;
#endif
#endif
}


#ifdef YY_USE_PROTOS
void yy_flush_buffer( YY_BUFFER_STATE b )
#else
void yy_flush_buffer( b )
YY_BUFFER_STATE b;
#endif

{
	b->yy_n_chars = 0;

	/* We always need two end-of-buffer characters.  The first causes
	* a transition to the end-of-buffer state.  The second causes
	* a jam in that state.
	*/
	b->yy_ch_buf[0] = YY_END_OF_BUFFER_CHAR;
	b->yy_ch_buf[1] = YY_END_OF_BUFFER_CHAR;

	b->yy_buf_pos = &b->yy_ch_buf[0];

	b->yy_at_bol = 1;
	b->yy_buffer_status = YY_BUFFER_NEW;

	if ( b == yy_current_buffer )
		yy_load_buffer_state();
}


#ifndef YY_NO_SCAN_BUFFER
#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_scan_buffer( char *base, yy_size_t size )
#else
YY_BUFFER_STATE yy_scan_buffer( base, size )
char *base;
yy_size_t size;
#endif
{
	YY_BUFFER_STATE b;

	if ( size < 2 ||
		base[size-2] != YY_END_OF_BUFFER_CHAR ||
		base[size-1] != YY_END_OF_BUFFER_CHAR )
		/* They forgot to leave room for the EOB's. */
		return 0;

	b = (YY_BUFFER_STATE) yy_flex_alloc( sizeof( struct yy_buffer_state ) );
	if ( ! b )
		YY_FATAL_ERROR( "out of dynamic memory in yy_scan_buffer()" );

	b->yy_buf_size = size - 2;	/* "- 2" to take care of EOB's */
	b->yy_buf_pos = b->yy_ch_buf = base;
	b->yy_is_our_buffer = 0;
	b->yy_input_file = 0;
	b->yy_n_chars = b->yy_buf_size;
	b->yy_is_interactive = 0;
	b->yy_at_bol = 1;
	b->yy_fill_buffer = 0;
	b->yy_buffer_status = YY_BUFFER_NEW;

	yy_switch_to_buffer( b );

	return b;
}
#endif


#ifndef YY_NO_SCAN_STRING
#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_scan_string( yyconst char *str )
#else
YY_BUFFER_STATE yy_scan_string( str )
yyconst char *str;
#endif
{
	int len;
	for ( len = 0; str[len]; ++len )
		;

	return yy_scan_bytes( str, len );
}
#endif


#ifndef YY_NO_SCAN_BYTES
#ifdef YY_USE_PROTOS
YY_BUFFER_STATE yy_scan_bytes( yyconst char *bytes, int len )
#else
YY_BUFFER_STATE yy_scan_bytes( bytes, len )
yyconst char *bytes;
int len;
#endif
{
	YY_BUFFER_STATE b;
	char *buf;
	yy_size_t n;
	int i;

	/* Get memory for full buffer, including space for trailing EOB's. */
	n = len + 2;
	buf = (char *) yy_flex_alloc( n );
	if ( ! buf )
		YY_FATAL_ERROR( "out of dynamic memory in yy_scan_bytes()" );

	for ( i = 0; i < len; ++i )
		buf[i] = bytes[i];

	buf[len] = buf[len+1] = YY_END_OF_BUFFER_CHAR;

	b = yy_scan_buffer( buf, n );
	if ( ! b )
		YY_FATAL_ERROR( "bad buffer in yy_scan_bytes()" );

	/* It's okay to grow etc. this buffer, and we should throw it
	* away when we're done.
	*/
	b->yy_is_our_buffer = 1;

	return b;
}
#endif


#ifndef YY_NO_PUSH_STATE
#ifdef YY_USE_PROTOS
static void yy_push_state( int new_state )
#else
static void yy_push_state( new_state )
int new_state;
#endif
{
	if ( yy_start_stack_ptr >= yy_start_stack_depth )
	{
		yy_size_t new_size;

		yy_start_stack_depth += YY_START_STACK_INCR;
		new_size = yy_start_stack_depth * sizeof( int );

		if ( ! yy_start_stack )
			yy_start_stack = (int *) yy_flex_alloc( new_size );

		else
			yy_start_stack = (int *) yy_flex_realloc(
			(void *) yy_start_stack, new_size );

		if ( ! yy_start_stack )
			YY_FATAL_ERROR(
			"out of memory expanding start-condition stack" );
	}

	yy_start_stack[yy_start_stack_ptr++] = YY_START;

	BEGIN(new_state);
}
#endif


#ifndef YY_NO_POP_STATE
static void yy_pop_state()
{
	if ( --yy_start_stack_ptr < 0 )
		YY_FATAL_ERROR( "start-condition stack underflow" );

	BEGIN(yy_start_stack[yy_start_stack_ptr]);
}
#endif


#ifndef YY_NO_TOP_STATE
static int yy_top_state()
{
	return yy_start_stack[yy_start_stack_ptr - 1];
}
#endif

#ifndef YY_EXIT_FAILURE
#define YY_EXIT_FAILURE 2
#endif

#ifdef YY_USE_PROTOS
static void yy_fatal_error( yyconst char msg[] )
#else
static void yy_fatal_error( msg )
char msg[];
#endif
{
	(void) fprintf( stderr, "%s\n", msg );
	exit( YY_EXIT_FAILURE );
}



/* Redefine yyless() so it works in section 3 code. */

#undef yyless
#define yyless(n) \
	do \
{ \
	/* Undo effects of setting up yytext. */ \
	yytext[yyleng] = yy_hold_char; \
	yy_c_buf_p = yytext + n - YY_MORE_ADJ; \
	yy_hold_char = *yy_c_buf_p; \
	*yy_c_buf_p = '\0'; \
	yyleng = n; \
} \
	while ( 0 )


/* Internal utility routines. */

#ifndef yytext_ptr
#ifdef YY_USE_PROTOS
static void yy_flex_strncpy( char *s1, yyconst char *s2, int n )
#else
static void yy_flex_strncpy( s1, s2, n )
char *s1;
yyconst char *s2;
int n;
#endif
{
	register int i;
	for ( i = 0; i < n; ++i )
		s1[i] = s2[i];
}
#endif


#ifdef YY_USE_PROTOS
static void *yy_flex_alloc( yy_size_t size )
#else
static void *yy_flex_alloc( size )
yy_size_t size;
#endif
{
	return (void *) malloc( size );
}

#ifdef YY_USE_PROTOS
static void *yy_flex_realloc( void *ptr, yy_size_t size )
#else
static void *yy_flex_realloc( ptr, size )
void *ptr;
yy_size_t size;
#endif
{
	/* The cast to (char *) in the following accommodates both
	* implementations that use char* generic pointers, and those
	* that use void* generic pointers.  It works with the latter
	* because both ANSI C and C++ allow castless assignment from
	* any pointer type to void*, and deal with argument conversions
	* as though doing an assignment.
	*/
	return (void *) realloc( (char *) ptr, size );
}

#ifdef YY_USE_PROTOS
static void yy_flex_free( void *ptr )
#else
static void yy_flex_free( ptr )
void *ptr;
#endif
{
	free( ptr );
}

#if YY_MAIN
int main()
{
	yylex();
	return 0;
}
#endif

/*[\n]+					{	return tEndLine;	} */

int isatty( int )
{
	return 1;
}

void	_yyundo()
{
	yyless(0);
	yychar = yylex();
}
const char*	_getcurpos()
{
	return yytext;
}
void yyerror (const char *s)  /* вызывается yyparse в случае ошибки */
{
	printf ("%s\n", s);	
}
void yy_reset(){
    YY_FLUSH_BUFFER;
	BEGIN(0);
}


