
/*  A Bison parser, made from xmlgrammar.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define    tAssignment    258
#define    tLeftCloseBracket    259
#define    tRightCloseBracket    260
#define    tLeftBracket    261
#define    tQuotedString    262
#define    tRightBracket    263
#define    tInsideTag    264
#define    tNumber    265
#define    tID    266



#include "stdafx.h"
#include <math.h>
#include <malloc.h>
#include <string.h> 
#include "kXMLParser.h"

//  disable some MSVC7 pain in the arse...
#pragma runtime_checks( "", off )
#pragma warning ( disable : 4102 )

BEGIN_NAMESPACE(xmlParser)

int        yylex();
int        yyerror(char *s);
void    _yyundo();
const    char*    _getcurpos();
void    _setcurpos( char* pos );

#define alloca _alloca

#define YYDEBUG 1
#define YYERROR_VERBOSE
static XMLParser* pParser = NULL;
unsigned int curTag;


typedef union{
    unsigned int    strID;
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



#define    YYFINAL        28
#define    YYFLAG        -32768
#define    YYNTBASE    12

#define YYTRANSLATE(x) ((unsigned)(x) <= 266 ? yytranslate[x] : 22)

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
     6,     7,     8,     9,    10,    11
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     3,     6,    11,    15,    16,    17,    24,    25,
    30,    32,    35,    39,    40,    42,    45
};

static const short yyrhs[] = {    13,
     0,     0,    16,     5,     0,    16,     8,    18,    19,     0,
    16,     8,    19,     0,     0,     0,    16,     8,    14,     9,
    15,    19,     0,     0,     6,    11,    17,    20,     0,    13,
     0,    18,    13,     0,     4,    11,     8,     0,     0,    21,
     0,    20,    21,     0,    11,     3,     7,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    56,    59,    59,    60,    61,    62,    67,    71,    73,    79,
    82,    83,    86,    92,    93,    94,    97
};

static const char * const yytname[] = {   "$","error","$undefined.","tAssignment",
"tLeftCloseBracket","tRightCloseBracket","tLeftBracket","tQuotedString","tRightBracket",
"tInsideTag","tNumber","tID","rXMLFile","rXMLBlock","@1","@2","rOpenTag","@3",
"rBlockList","rCloseTag","rAttrList","rAttribute",""
};
#endif

static const short yyr1[] = {     0,
    12,    13,    13,    13,    13,    14,    15,    13,    17,    16,
    18,    18,    19,    20,    20,    20,    21
};

static const short yyr2[] = {     0,
     1,     0,     2,     4,     3,     0,     0,     6,     0,     4,
     1,     2,     3,     0,     1,     2,     3
};

static const short yydefact[] = {     2,
     0,     1,     0,     9,     3,     6,    14,     0,    11,     0,
     0,     5,     0,    10,    15,     0,     7,    12,     4,     0,
    16,    13,     0,    17,     8,     0,     0,     0
};

static const short yydefgoto[] = {    26,
     2,    10,    23,     3,     7,    11,    12,    14,    15
};

static const short yypact[] = {    -2,
    -8,-32768,    -3,-32768,-32768,     3,    -1,     0,-32768,     4,
     3,-32768,     5,    -1,-32768,     6,-32768,-32768,-32768,     8,
-32768,-32768,    12,-32768,-32768,    17,    18,-32768
};

static const short yypgoto[] = {-32768,
    -5,-32768,-32768,-32768,-32768,-32768,   -11,-32768,     7
};


#define    YYLAST        21


static const short yytable[] = {    19,
     9,     5,     4,     1,     6,    18,     8,    20,     1,    13,
    16,    25,    17,    22,    24,     8,    27,    28,     0,     0,
    21
};

static const short yycheck[] = {    11,
     6,     5,    11,     6,     8,    11,     4,     3,     6,    11,
    11,    23,     9,     8,     7,     4,     0,     0,    -1,    -1,
    14
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

#define yyerrok        (yyerrstatus = 0)
#define yyclearin    (yychar = YYEMPTY)
#define YYEMPTY        -2
#define YYEOF        0
#define YYACCEPT    return(0)
#define YYABORT     return(1)
#define YYERROR        goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL        goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do                                \
  if (yychar == YYEMPTY && yylen == 1)                \
    { yychar = (token), yylval = (value);            \
      yychar1 = YYTRANSLATE (yychar);                \
      YYPOPSTACK;                        \
      goto yybackup;                        \
    }                                \
  else                                \
    { yyerror ("syntax error: cannot back up"); YYERROR; }    \
while (0)

#define YYTERROR    1
#define YYERRCODE    256

#ifndef YYPURE
#define YYLEX        yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX        yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX        yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX        yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX        yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int    yychar;            /*  the lookahead symbol        */
YYSTYPE    yylval;            /*  the semantic value of the        */
                /*  lookahead symbol            */

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;            /*  location data for the lookahead    */
                /*  symbol                */
#endif

int yynerrs;            /*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;            /*  nonzero means print parse trace    */
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks    */

#ifndef    YYINITDEPTH
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

#if __GNUC__ > 1        /* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)    __builtin_memcpy(TO,FROM,COUNT)
#else                /* not GNU C or C++ */
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
  int yyerrstatus;    /*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;        /*  lookahead token as an internal (translated) token number */

  short    yyssa[YYINITDEPTH];    /*  the state stack            */
  YYSTYPE yyvsa[YYINITDEPTH];    /*  the semantic value stack        */

  short *yyss = yyssa;        /*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;    /*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];    /*  the location stack            */
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

  YYSTYPE yyval;        /*  the variable used to return        */
                /*  semantic values from the action    */
                /*  routines                */

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;        /* Cause a token to be read.  */

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

  if (yychar <= 0)        /* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;        /* Don't call YYLEX any more */

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

case 3:
{ pParser->OnClose( GetPooledString( curTag ) ); ;
    break;}
case 6:
{
                        _yyundo();                    
                    ;
    break;}
case 7:
{
                        pParser->OnValue( GetPooledString( yyvsp[0].strID ) );
                    ;
    break;}
case 9:
{
                        pParser->OnOpen( GetPooledString( yyvsp[0].strID ) );
                        curTag = yyvsp[0].strID;
                    ;
    break;}
case 13:
{
                        pParser->OnClose( GetPooledString( yyvsp[-1].strID ) );
                    ;
    break;}
case 17:
{
                        pParser->OnAttribute( GetPooledString( yyvsp[-2].strID ), GetPooledString( yyvsp[0].strID ) );
                    ;
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

  yyerrstatus = 3;        /* Each real token shifted decrements this */

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


extern int yylineno;
extern char *yytext;
int yyerror(char *s) 
{
    assert( pParser );
    char errb[1024];
    sprintf( errb, "%s. Stopped at token (%s)", s, yytext );
    pParser->Error( errb, yylineno );
    return 0;
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

#else    /* ! __cplusplus */

#if __STDC__

#define YY_USE_PROTOS
#define YY_USE_CONST

#endif    /* __STDC__ */
#endif    /* ! __cplusplus */

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
#define YY_BUF_SIZE 65536

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
 *     if ( condition_holds )
 *        yyless( 5 );
 *    else
 *        do_something_else();
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

    char *yy_ch_buf;        /* input buffer */
    char *yy_buf_pos;        /* current position in input buffer */

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

static int yy_n_chars;        /* number of characters read into yy_ch_buf */


int yyleng;

/* Points to current character in buffer. */
static char *yy_c_buf_p = (char *) 0;
static int yy_init = 1;        /* whether we need to initialize */
static int yy_start = 0;    /* start state number */

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


#define YY_USES_REJECT

#define yywrap() 1
#define YY_SKIP_YYWRAP
typedef unsigned char YY_CHAR;
FILE *yyin = (FILE *) 0, *yyout = (FILE *) 0;
typedef int yy_state_type;
extern int yylineno;
int yylineno = 1;
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

#define YY_NUM_RULES 15
#define YY_END_OF_BUFFER 16
static yyconst short int yy_acclist[86] =
    {   0,
       16,   14,   15,   10,   14,   15,   10,   15,   14,   15,
       14,   15,   14,   15,   12,   14,   15,    7,   14,   15,
        4,   14,   15,    8,   14,   15,   13,   14,   15,   11,
       14,   15,   10,   11,   14,   15,   10,   11,   15,   11,
       14,   15,   11,   14,   15,   11,   14,   15,   11,   12,
       14,   15,    4,   11,   14,   15,    8,   11,   14,   15,
       11,   13,   14,   15,   10,    9,   12,    6,    5,   13,
       11,   10,   11,   11,    9,   11,   11,   12,    6,   11,
       11,   13,    1,    2,    3
    } ;

static yyconst short int yy_accept[72] =
    {   0,
        1,    1,    1,    1,    1,    2,    4,    7,    9,   11,
       13,   15,   18,   21,   24,   27,   30,   33,   37,   40,
       43,   46,   49,   53,   57,   61,   65,   66,   66,   67,
       68,   69,   69,   70,   70,   71,   72,   74,   75,   77,
       79,   81,   83,   83,   83,   83,   83,   83,   83,   83,
       83,   83,   83,   83,   83,   83,   83,   83,   84,   84,
       85,   85,   85,   85,   85,   85,   85,   85,   85,   86,
       86
    } ;

static yyconst int yy_ec[256] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    2,    3,
        1,    1,    2,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    2,    4,    5,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    6,    1,    7,    8,    8,    8,
        8,    8,    8,    8,    8,    8,    8,    1,    1,    9,
       10,   11,   12,    1,   15,   15,   16,   17,   18,   15,
       15,   15,   15,   15,   15,   19,   20,   15,   21,   22,
       15,   15,   15,   23,   15,   15,   15,   24,   25,   15,
       13,    1,   14,    1,   15,    1,   15,   15,   16,   17,

       18,   15,   15,   15,   15,   15,   15,   19,   20,   15,
       21,   22,   15,   15,   15,   23,   15,   15,   15,   24,
       25,   15,    1,    1,    1,    1,    1,    1,    1,    1,
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

static yyconst int yy_meta[26] =
    {   0,
        1,    2,    2,    1,    1,    3,    1,    4,    5,    1,
        6,    6,    1,    7,    8,    8,    8,    8,    8,    8,
        8,    8,    8,    8,    8
    } ;

static yyconst short int yy_base[80] =
    {   0,
        0,    0,   25,    0,  173,  174,   49,   51,  167,  163,
      158,  153,   51,  174,  174,    0,    0,   54,   57,   56,
      152,  148,  149,    0,    0,   58,   65,  149,  174,  145,
      174,   78,  174,  128,    0,    0,   67,   80,    0,  143,
        0,    0,  144,  125,  115,    0,  105,   96,  108,   90,
        0,   96,   76,   88,   88,   76,   83,  174,   70,  174,
       69,   84,   88,   90,   94,    0,   50,   51,  174,  174,
      103,  108,  116,  124,  132,  140,  147,  154,  162
    } ;

static yyconst short int yy_def[80] =
    {   0,
       70,    1,   70,    3,   70,   70,   70,   70,   71,   70,
       70,   70,   70,   70,   70,   72,   73,   73,   73,   74,
       73,   73,   73,   73,   73,   73,   70,   71,   70,   70,
       70,   70,   70,   70,   72,   73,   73,   74,   73,   73,
       73,   26,   70,   70,   70,   75,   70,   70,   75,   70,
       76,   70,   70,   76,   70,   70,   70,   70,   70,   70,
       70,   77,   78,   70,   78,   79,   79,   70,   70,    0,
       70,   70,   70,   70,   70,   70,   70,   70,   70
    } ;

static yyconst short int yy_nxt[200] =
    {   0,
        6,    7,    8,    6,    9,   10,   11,   12,   13,   14,
       15,    6,    6,    6,   16,   16,   16,   16,   16,   16,
       16,   16,   16,   16,   16,   17,   18,   19,   17,   20,
       21,   22,   23,   13,   24,   25,   17,   17,   17,   26,
       26,   26,   26,   26,   26,   26,   26,   26,   26,   26,
       27,   27,   27,   27,   32,   37,   37,   33,   37,   37,
       39,   69,   34,   68,   28,   42,   27,   27,   37,   37,
       62,   62,   42,   42,   42,   42,   42,   42,   42,   42,
       42,   42,   42,   43,   39,   62,   62,   61,   28,   64,
       64,   64,   64,   60,   44,   64,   64,   59,   58,   57,

       56,   55,   66,   28,   28,   28,   28,   28,   28,   28,
       28,   35,   53,   52,   51,   35,   36,   36,   36,   36,
       50,   36,   36,   36,   38,   38,   38,   38,   38,   38,
       38,   38,   49,   49,   48,   49,   49,   49,   49,   49,
       54,   54,   54,   54,   54,   47,   54,   54,   63,   46,
       40,   45,   30,   29,   63,   65,   40,   65,   41,   40,
       30,   65,   67,   67,   67,   67,   67,   67,   31,   67,
       30,   29,   70,    5,   70,   70,   70,   70,   70,   70,
       70,   70,   70,   70,   70,   70,   70,   70,   70,   70,
       70,   70,   70,   70,   70,   70,   70,   70,   70

    } ;

static yyconst short int yy_chk[200] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    3,    3,    3,    3,    3,
        3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
        3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
        7,    7,    8,    8,   13,   18,   18,   13,   19,   19,
       20,   68,   13,   67,   20,   26,   27,   27,   37,   37,
       61,   61,   26,   26,   26,   26,   26,   26,   26,   26,
       26,   26,   26,   32,   38,   62,   62,   59,   38,   63,
       63,   64,   64,   57,   32,   65,   65,   56,   55,   54,

       53,   52,   64,   71,   71,   71,   71,   71,   71,   71,
       71,   72,   50,   49,   48,   72,   73,   73,   73,   73,
       47,   73,   73,   73,   74,   74,   74,   74,   74,   74,
       74,   74,   75,   75,   45,   75,   75,   75,   75,   75,
       76,   76,   76,   76,   76,   44,   76,   76,   77,   43,
       40,   34,   30,   28,   77,   78,   23,   78,   22,   21,
       12,   78,   79,   79,   79,   79,   79,   79,   11,   79,
       10,    9,    5,   70,   70,   70,   70,   70,   70,   70,
       70,   70,   70,   70,   70,   70,   70,   70,   70,   70,
       70,   70,   70,   70,   70,   70,   70,   70,   70

    } ;

static yy_state_type yy_state_buf[YY_BUF_SIZE + 2], *yy_state_ptr;
static char *yy_full_match;
static int yy_lp;
#define REJECT \
{ \
*yy_cp = yy_hold_char; /* undo effects of setting up yytext */ \
yy_cp = yy_full_match; /* restore poss. backed-over text */ \
++yy_lp; \
goto find_rule; \
}
#define yymore() yymore_used_but_not_detected
#define YY_MORE_ADJ 0
char *yytext;
#define INITIAL 0
/*****************************************************************************/
/*    File:    xmlGrammar.l
/*    Desc:    .xml file FLEX rules
/*    Author:    Ruslan Shestopalyuk
/*    Date:    03.04.2003
/*****************************************************************************/
#include <math.h>
#define YY_INPUT(buf,result,max_size) {assert(pParser); result = pParser->yyInput(buf, max_size);}
#define YY_NEVER_INTERACTIVE 1
#define InsideTag 1


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
#define YY_READ_BUF_SIZE 65536
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
            yy_start = 1;    /* first start state */

        if ( ! yyin )
            yyin = stdin;

        if ( ! yyout )
            yyout = stdout;

        if ( ! yy_current_buffer )
            yy_current_buffer =
                yy_create_buffer( yyin, YY_BUF_SIZE );

        yy_load_buffer_state();
        }

    while ( 1 )        /* loops until end-of-file is reached */
        {
        yy_cp = yy_c_buf_p;

        /* Support of yytext. */
        *yy_cp = yy_hold_char;

        /* yy_bp points to the position in yy_ch_buf of the start of
         * the current run.
         */
        yy_bp = yy_cp;

        yy_current_state = yy_start;
        yy_state_ptr = yy_state_buf;
        *yy_state_ptr++ = yy_current_state;
yy_match:
        do
            {
            register YY_CHAR yy_c = yy_ec[YY_SC_TO_UI(*yy_cp)];
            while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
                {
                yy_current_state = (int) yy_def[yy_current_state];
                if ( yy_current_state >= 71 )
                    yy_c = yy_meta[(unsigned int) yy_c];
                }
            yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
            *yy_state_ptr++ = yy_current_state;
            ++yy_cp;
            }
        while ( yy_base[yy_current_state] != 174 );

yy_find_action:
        yy_current_state = *--yy_state_ptr;
        yy_lp = yy_accept[yy_current_state];
find_rule: /* we branch to this label when backing up */
        for ( ; ; ) /* until we find what rule we matched */
            {
            if ( yy_lp && yy_lp < yy_accept[yy_current_state + 1] )
                {
                yy_act = yy_acclist[yy_lp];
                    {
                    yy_full_match = yy_cp;
                    break;
                    }
                }
            --yy_cp;
            yy_current_state = *--yy_state_ptr;
            yy_lp = yy_accept[yy_current_state];
            }

        YY_DO_BEFORE_ACTION;

        if ( yy_act != YY_END_OF_BUFFER )
            {
            int yyl;
            for ( yyl = 0; yyl < yyleng; ++yyl )
                if ( yytext[yyl] == '\n' )
                    ++yylineno;
            }

do_action:    /* This label is used only to access EOF actions. */


        switch ( yy_act )
    { /* beginning of action switch */
case 1:
YY_RULE_SETUP
{}
    YY_BREAK
case 2:
YY_RULE_SETUP
{}
    YY_BREAK
case 3:
YY_RULE_SETUP
{}
    YY_BREAK
case 4:
YY_RULE_SETUP
{    return tAssignment;            }
    YY_BREAK
case 5:
YY_RULE_SETUP
{    return tLeftCloseBracket;    }
    YY_BREAK
case 6:
YY_RULE_SETUP
{    return tRightCloseBracket;    printf( "/>" );}
    YY_BREAK
case 7:
YY_RULE_SETUP
{    return tLeftBracket;        }
    YY_BREAK
case 8:
YY_RULE_SETUP
{    return tRightBracket;        }
    YY_BREAK
case 9:
YY_RULE_SETUP
{    
                            yylval.strID = CreatePooledString( yytext + 1, yyleng - 2 );
                            return tQuotedString;        
                        }
    YY_BREAK
case 10:
YY_RULE_SETUP
{    /*  skip blank symbols    */    }
    YY_BREAK
case 11:
YY_RULE_SETUP
{ 
                            yylval.strID = CreatePooledString( yytext, yyleng );
                            BEGIN(0); 
                            return tInsideTag; 
                        }
    YY_BREAK
case 12:
YY_RULE_SETUP
{    
                            yylval.strID = CreatePooledString( yytext, yyleng );
                            return tNumber;                    
                        }
    YY_BREAK
case 13:
YY_RULE_SETUP
{    
                            yylval.strID = CreatePooledString( yytext, yyleng );
                            return tID;                    
                        }
    YY_BREAK
case 14:
YY_RULE_SETUP
{    /*  skip anything else    */    }
    YY_BREAK
case 15:
YY_RULE_SETUP
ECHO;
    YY_BREAK
            case YY_STATE_EOF(INITIAL):
            case YY_STATE_EOF(InsideTag):
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
 *    EOB_ACT_LAST_MATCH -
 *    EOB_ACT_CONTINUE_SCAN - continue scanning from current position
 *    EOB_ACT_END_OF_FILE - end of file
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
    yy_state_ptr = yy_state_buf;
    *yy_state_ptr++ = yy_current_state;

    for ( yy_cp = yytext_ptr + YY_MORE_ADJ; yy_cp < yy_c_buf_p; ++yy_cp )
        {
        register YY_CHAR yy_c = (*yy_cp ? yy_ec[YY_SC_TO_UI(*yy_cp)] : 1);
        while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
            {
            yy_current_state = (int) yy_def[yy_current_state];
            if ( yy_current_state >= 71 )
                yy_c = yy_meta[(unsigned int) yy_c];
            }
        yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
        *yy_state_ptr++ = yy_current_state;
        }

    return yy_current_state;
    }


/* yy_try_NUL_trans - try to make a transition on the NUL character
 *
 * synopsis
 *    next_state = yy_try_NUL_trans( current_state );
 */

#ifdef YY_USE_PROTOS
static yy_state_type yy_try_NUL_trans( yy_state_type yy_current_state )
#else
static yy_state_type yy_try_NUL_trans( yy_current_state )
yy_state_type yy_current_state;
#endif
    {
    register int yy_is_jam;

    register YY_CHAR yy_c = 1;
    while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
        {
        yy_current_state = (int) yy_def[yy_current_state];
        if ( yy_current_state >= 71 )
            yy_c = yy_meta[(unsigned int) yy_c];
        }
    yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
    *yy_state_ptr++ = yy_current_state;
    yy_is_jam = (yy_current_state == 70);

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

    if ( c == '\n' )
        --yylineno;

    yytext_ptr = yy_bp;
    yy_hold_char = *yy_cp;
    yy_c_buf_p = yy_cp;
    }
#endif    /* ifndef YY_NO_UNPUT */


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

    c = *(unsigned char *) yy_c_buf_p;    /* cast for 8-bit char's */
    *yy_c_buf_p = '\0';    /* preserve yytext */
    yy_hold_char = *++yy_c_buf_p;

    if ( c == '\n' )
        ++yylineno;

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

    b->yy_buf_size = size - 2;    /* "- 2" to take care of EOB's */
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


int isatty( int )
{
    return 1;
}

void    _yyundo()
{
    yyless(0);
    BEGIN(InsideTag);
    yychar = yylex();
}

const char*    _getcurpos()
{
    return yytext;
}


END_NAMESPACE(xmlParser)

bool XMLParser::ParseBuffer( char* buffer )
{
    assert( buffer );
    Init();
    
    m_Buffer            = buffer;
    m_BufPtr            = buffer;
    xmlParser::pParser    = this;
    xmlParser::yyparse();
    return true;
} // XMLParser::ParseBuffer



