#ifndef GP142_H
#define GP142_H

#define WINDOWS /* This line must define MACINTOSH or WINDOWS or X11R6 */

/************************************************************************
** GP142.H
**
** Dear 142 Students,
** This is the header file for the GP142 graphics library.  This file is
** divided into two sections:
**    * Compiler directives -- You should ignore this section; it's arcane.
**    * API Functions       -- Read this section.
**
** Extra note to Macintosh users:
**    * GP142 requires a minimum of system 6.0.3 and to be run on a machine
**      built AFTER a Mac Plus.
**    * For your program to animate while in the background, select
**      "Set Project Type..." from the Project menu, click and hold
**      "SIZE flags", and select "Background NULL events", then click OK.
**
*************************************************************************
**
** GP142 Application Programmer's Interface (API)
** Copyright (C) 1994 Neil McKenzie, Corin Anderson, Casey Anderson
** University of Washington, Seattle WA
** December, 1994  v1.90
** January,  1995  v2.00
** August,   1995  v2.02
** October,  1995  v2.10
** RCS $Id: gp142.h,v 1.11 2000/07/17 22:34:11 boren Exp $
*/

/************************************************************************
                      COMPILER DIRECTIVES -- IGNORE
************************************************************************/
#include <stdio.h>

/* Shuichi Modified this to handle linking against C++ */
#ifdef __cplusplus 
extern "C"{ 
#endif  

/* The constant WIN32 is automatically defined for you by the compiler. */
/* When utilizing WIN32, _export doesn't exist as a valid parameter anymore */
#if defined(WIN32)
  #define WINDOWS
  #define EXPORT
#elif defined(WINDOWS)
  #define EXPORT _export
#endif

/* verify that system defines are appropriate */
#if !(defined(MACINTOSH) ^ defined(WINDOWS) ^ defined(X11R6))
  #error In GP142.H, you must #define one of MACINTOSH or WINDOWS or X11R6.
#endif

#ifdef WINDOWS
#include <windows.h>
//#define EMPTY HANDLE,HANDLE,LPSTR,int
#endif

#ifdef MACINTOSH
#include <stdlib.h>
#define EMPTY void
#define FAR
#endif

#ifdef X11R6
#define EMPTY void
#define FAR
#define TRUE 1
#define FALSE 0
#endif


/**************************************************************************
                           API FUNCTIONS
***************************************************************************/
/*
    Very important: scanf() is not compatible with the GP142 library.
    Do not use scanf() in these programs.  You should use GP142_gets() 
    together with sscanf() instead.
*/

/*
** Window Size: max X and Y coordinates in the GP142 Graphics Window.
**
** NOTE NOTE NOTE:
**   On the PC version, the graphics windows size is variable.
**   However, for portability to the Mac, which has a fixed size
**   window, you should not resize it.
*/
#ifdef WINDOWS
#define GP142_XMAX              350
#define GP142_YMAX              280
#endif

#ifdef MACINTOSH
#define GP142_XMAX              300
#define GP142_YMAX              250
#endif

#ifdef X11R6
#define GP142_XMAX              300
#define GP142_YMAX              250
#endif



/*
** color palette
*/
#define MAX_COLORS      24

#define BLACK           0
#define WHITE           1
#define RED             2
#define GREEN           3
#define BLUE            4
#define YELLOW          5
#define MAGENTA         6
#define CYAN            7
#define PURPLE          8
#define NAVY_BLUE       9
#define DUSTY_PLUM      10
#define ICE_BLUE        11
#define TURQUOISE       12
#define ORANGE          13
#define BROWN           14
#define PINK            15
#define CHALK           16
#define GOLD            17
#define PEACH           18
#define FOREST_GREEN    19
#define SEA_GREEN       20
#define OLIVE           21
#define MED_GRAY        22
#define LT_GRAY         23

/*
**  NOTE: the FAR type modifier on all the functions may seem strange;
**  Windows compilers need it.  Please leave it in, even if you're only
**  using Think C on the Macintosh.
*/

/* Possible return values of GP142 API functions */
#define GP142_SUCCESS   1
#define GP142_FAILURE   0

extern int FAR GP142_open();        /* Initialize the GP142 library;  */
                                    /* Call this at the top of main() */

                                    /* (Pretend EMPTY == void above. You */
                                    /* really don't want to know why     */
                                    /* it's not void; c.f. "COMPILER     */
                                    /* DIRECTIVES" if you must.)         */

extern int FAR
GP142_close(void);                  /* Close the library.  Call this at */
                                    /* the end of main() */

extern int FAR
GP142_await_event(                  /* Fetch the next event */
    int *mouse_x,                   /* where to put mouse's x coordinate */
    int *mouse_y,                   /* where to put mouse's y coordinate */
    char *key_pressed);             /* where to put character just entered */

/*
** Possible return values
** from GP142_await_event()
*/
#define GP142_MOUSE     1           /* mouse click: mouse_y/mouse_y set */
#define GP142_KBD       2           /* keyboard event: key_pressed set */
#define GP142_PERIODIC  3           /* periodic event (neither set) */
#define GP142_QUIT      4           /* user says "Quit" */
#define GP142_RIGHTCLICK 5
extern int FAR
GP142_clear(void);                  /* clear window */

extern int FAR
GP142_flush(void);  /* flushes the window by executing all drawing requests
                       that have been issued.  The screen is flushed
                       when GP142_await_event is called, so you don't need 
                       to use this function unless you want to synchronize
                       your display (perhaps when debugging) */

extern int FAR
GP142_logging(                      /* switch logging on or off             */
    int);                           /* Possible values:                     */
#define LOG_OFF 0                   /*      LOG_OFF == no logging;          */
#define LOG_ON  1                   /*      LOG_ON == yes                   */

extern int FAR
GP142_animate(                      /* controls animation/GP142_PERIODIC events */
    int);                           /* Possible values:                     */
#define ANI_HALT        0           /*     == Stop animation                */
#define ANI_SINGLE_STEP 1           /*     == Advance one frame             */
#define ANI_RUN         2           /*     == Start animation               */
#define ANI_QUIT        5           /*     == Quit program                  */

extern int FAR
GP142_gets(                         /* Display a dialog box, and ask user */
                                    /* for a string */
    const char prompt[],            /* Prompt string */
    char result[]);                 /* Result string */

/******************************************************************************
        API functions, using pairs of ints to specify coordinates
*******************************************************************************/
extern int FAR
GP142_pixelXY(                  /* draw a pixel in the graphics window */
    int color,                  /* color */
    int x, int y);              /* coordinates */

extern int FAR
GP142_lineXY(                   /* draw a line segment on the graphics window */
    int color,                  /* color */
    int x1, int y1,             /* x, y coordinates of starting point */
    int x2, int y2,             /* x, y coordinates of ending point */
    int thickness);             /* line width */

extern int FAR
GP142_rectangleXY(              /* draw a rectangle on the graphics window */
    int color,                  /* color */
    int x1, int y1,             /* coordinates of one corner */
    int x2, int y2,             /* coordinates of opposite corner */
    int thickness);             /* line width; 0 => fill */

extern int FAR
GP142_triangleXY(               /* draw a triangle on the graphics window */
    int color,                  /* color */
    int x1, int y1,             /* coordinates of first corner point */
    int x2, int y2,             /* coordinates of second corner point */
    int x3, int y3,             /* coordinates of third corner point */
    int thickness);             /* line width; 0 => fill */

extern int FAR
GP142_ovalXY(                   /* draw an oval on the graphics window */
    int color,                  /* color */
    int x1, int y1,             /* coordinates of one corner */
    int x2, int y2,             /* coordinates of opposite corner */
    int thickness);             /* line width; 0 => fill */

extern int FAR
GP142_circleXY(                 /* draw a filled circle on the graphics window */
    int color,                  /* color */
    int x, int y,               /* coordinates of center */
	int radius, int with);                /* radius */

/*
 * This function provides formated printout to the graphics window,
 * essentially just like printf.  Newlines do advance text to the next
 * line.
 */
extern int FAR
GP142_printfXY(                 /* printf onto the graphics window */
    int color,                  /* text color                       */
    int x, int y,               /* coords of left end of text       */
    int point_size,             /* text size                        */
    const char fmt[],           /* the printf format string         */
    ... );                      /* list of values to print          */

extern int FAR                  /* don't use this; GP142_printfXY is better */
GP142_textXY(                   /* draw a text string to graphics window */
    int color,                  /* what color */
    int x, int y,               /* where */
    int point_size,             /* how big */
    const char[]);              /* what */

extern int FAR                  /* return height/width text would take  */
GP142_getDimensions(            /* if drawn to the graphics window      */
    int point_size,             /* how big                              */
    int *height,                /* return height here                   */
    int *width,                 /* return width here                    */
    const char fmt[],           /* the printf format string             */
    ... );                      /* list of values to print              */
    
extern int FAR                  /* as above, but takes a plain string,  */
GP142_getDimensions2 (          /* rather than a printf list.           */
    int point_size, 
    int *height, 
    int *width, 
    const char string[]);


/************* API, using structs to specify coordinates ****************/
/*  You will not need these functions until after we cover structs.     */

typedef struct {
    int x;
    int y;
} GP142_point;

typedef struct {
    int x, y;
    char c;
} GP142_event_t;

extern int FAR
GP142_await_eventP(             /* Fetch the next event */
    GP142_event_t *event);      /* pointer to event record */

extern int FAR
GP142_pixelP(                   /* draw a pixel in the graphics window  */
    int color,                  /* color */
    GP142_point p);             /* Point to draw */

extern int FAR
GP142_lineP(                    /* draw a line segment on the graphics window */
    int color,                  /* color */
    GP142_point p1,             /* starting point */
    GP142_point p2,             /* ending point */
    int thickness);             /* line width */

extern int FAR
GP142_rectangleP(               /* draw a rectangle on the graphics window */
    int color,                  /* color */
    GP142_point p1,             /* one corner point */
    GP142_point p2,             /* opposite corner point */
    int thickness);             /* line width; 0 => fill */

extern int FAR
GP142_triangleP(                /* draw a triangle on the graphics window */
    int color,                  /* color */
    GP142_point p1,             /* first corner point */
    GP142_point p2,             /* second corner point */
    GP142_point p3,             /* third corner point */
    int thickness);             /* line width; 0 => fill */

extern int FAR
GP142_ovalP(                    /* draw an oval on the graphics window */
    int color,                  /* color */
    GP142_point p1,             /* one corner point */
    GP142_point p2,             /* opposite corner point */
    int thickness);             /* line width; 0 => fill */

extern int FAR
GP142_circleP(                  /* draw a circle on the graphics window */
    int color,                  /* color */
    GP142_point p,              /* center point */
	int radius, int with);                /* radius */

extern int FAR
GP142_printfP(                  /* printf onto the graphics window  */
    int color,                  /* text color                       */
    GP142_point p,              /* x, y coords of left end of text  */
    int size,                   /* text size                        */
    const char fmt[],           /* the printf format string         */
    ... );                      /* list of values to print          */

extern int FAR                  /* don't use this; GP142_printfP is better */
GP142_textP(                    /* draw a text string to graphics window */
    int color,                  /* color */
    GP142_point,                /* Point to start text */
    int size,                   /* point size */
    const char []);             /* string of text in double quotes  */

extern void FAR                 /* has an effect in Windows only    */
GP142_show_text(                /* show or hide the text window     */
    int Showit);                /* nonzero = show, zero = hide      */


int pil(int a, int b);

/************************************************************************
                  MORE COMPILER DIRECTIVES -- IGNORE
************************************************************************/
extern int FAR
GP142_undo(void);               /* obsolete function: do not use  */
#ifdef WINDOWS
extern int FAR EXPORT WINprintf(const char [], ...);

#define printf WINprintf

/*
// Shuichi was here.
// This works for C, but not C++ very well, so I'm going to change 
// it to something cleaner. See GP142Display.cpp for more details.
*/
#ifndef __cplusplus
/*
** #defines for main(dummy), GP142_open(), and EMPTY are a cheap trick to hide
** the fact that windows version must pass 4 parameters through from main()
** to GP142_open(). And breaks under C++ (Shuichi)
*/
/*
#define main(dummy)  PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
#define GP142_open() GP142_open(hInstance, hPrevInstance, lpszCmdLine, nCmdShow)
*/
#endif
#endif

/* Shuichi was here. */
#ifdef __cplusplus 
}
#endif  

#endif /* GP142.H */
