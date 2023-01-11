/*
** GP142lib.h by Neil McKenzie
** 
** (C) 1994 Neil McKenzie.  All rights reserved.
** Parts (C) 1994, 1995 Corin Anderson.
** RCS $Id: gp142lib.h,v 1.11 2000/07/17 22:34:25 boren Exp $
** Includes changes made for port to WIN32 by Shuichi Koga 
** All additions ifdefed by WIN32 -- brd
*/

/* include prototypes for API functions */
#include "GP142.H"

#if !(defined(MACINTOSH) ^ defined(WINDOWS) ^ defined(X11R6))
#error Either MACINTOSH or WINDOWS or X11R6 must be #defined.
#endif

/*************************************************** Include files ***/
#ifdef MACINTOSH
#include <Events.h>
#include <TextEdit.h>
#include <Windows.h>
#include <Controls.h>
#include <Dialogs.h>
#include <Menus.h>
#include <QuickDraw.h>
#include <console.h>
#include <Timer.h>
#include <Fonts.h>
#include <ToolUtils.h>
#include <Memory.h>
#ifdef __MWERKS__
#include <SIOUX.h>
#endif
#include <QDOffscreen.h>
#elif defined(WINDOWS)
#include <windows.h>
#elif defined(X11R6)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <sys/time.h>
#include <signal.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

/**************************************************** type declarations ***/
#ifdef MACINTOSH
#define MAX_EVENTS 10
#elif defined(WINDOWS)
#define MAX_EVENTS 100
#elif defined(X11R6)
#define MAX_EVENTS 100
#endif

/* Shuichi Modified this to handle linking against C++ libraries */
#ifdef __cplusplus 
extern "C"{ 
#endif  

enum {
    obj_triangle,
    obj_rectangle,
    obj_oval,
    obj_line,
    obj_pixel,
    obj_text
};

typedef struct {
    int obj_type;
    int color;
    int width;
    union {
        struct {
            GP142_point p1;
            GP142_point p2;
            GP142_point p3;
        } triangle;
        struct {
            GP142_point upper_left;
            GP142_point lower_right;
        } rectangle;
        struct {
            GP142_point upper_left;
            GP142_point lower_right;
        } oval;
        struct {
            GP142_point upper_left;
            GP142_point lower_right;
        } line;
        struct {
            GP142_point p;
        } pixel;
        struct {
            GP142_point p;
            int ptsize;                 
#ifdef MACINTOSH
            char *the_text;
#elif defined(WINDOWS)
            HANDLE hText;
#elif defined(X11R6)
            char *the_text;
#endif
        } text;
    } g;
} Graphics;

#ifdef __cplusplus 
}
#endif  

/********************************************* Mac specific stuff ***/
#ifdef MACINTOSH
#define Desk_ID         200
#define Run_ID          201
#define kGetsDialogID   128
#define kGetsEditTextID 3

/* Change this macro to 0 if your compiler is old.  You'll know
   you need to change this macro if the compiler complains that
   it can't find, say, qd.thePort. */
#define USES_NEW_HEADERS 1


/*void timerFlagProc(void);*/
void static ToolboxInit(void);
void static DoMenu(long);
void static DoMouseClick(EventRecord *, CWindowPtr);
static void HandleMessage(EventRecord *event);
void static refresh(CWindowPtr);
static void GP_clear(void);
void static Draw_Graphic_Item(Graphics *);
#endif

/****************************************** Windows specific stuff ***/
#ifdef WINDOWS

#define BKGND_COLOR     WHITE

enum {
    IDM_ANIMATE = 1,
    IDM_HALT,
    IDM_ADVANCE_ONE,
    IDM_LOGGING,
    IDM_RECORD_SCRIPT,
    IDM_PLAY_SCRIPT,
    IDM_STEP_SCRIPT,
    IDM_QUIT,
    IDM_ABOUT};

enum {
    kOkayID = 101,
    kEditID, 
    kStaticID};

#ifdef GP142_open
#undef GP142_open
#endif

#ifdef WIN32
long FAR PASCAL EXPORT DrawWndProc(HWND, UINT, WPARAM, LPARAM);
long FAR PASCAL EXPORT TextWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK getsDialogProc(HWND hDlg, UINT wMessage, WPARAM wParam, LONG lParam);
#else
long FAR PASCAL EXPORT DrawWndProc(HWND, WORD, WORD, LONG);
long FAR PASCAL EXPORT TextWndProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL EXPORT getsDialogProc(HWND, WORD, WORD, LONG);
#endif
int FAR PASCAL LibMain(HINSTANCE, WORD, WORD, LPSTR);

/* these constants are only of interest in Windows */
#define PERIODIC_TASK_WAIT_COUNT    100
#define MAX_CONSOLE_LINES           25
#define DLI_OKAY                    
#define DLI_EDIT                    
#define DLI_STATIC                  


static void DrawString(char far *s);
int FAR EXPORT WINprintf(const char fmt[], ... );
static void GP_clear(void);
void FAR Draw_Graphic_Item(Graphics *);
int far_strlen(char far *);

/* Dan 2/3/00 */
static void RedrawTextWnd(HWND hWnd);

#endif

/****************************************** X11R6 specific stuff ***/

#ifdef X11R6
typedef struct sFontDB {
        XFontStruct* thefont;
        int point_size;
        struct sFontDB* next;
        } FontDB;
typedef struct sRGBColor {
        unsigned short red;
        unsigned short green;
        unsigned short blue;
        } RGBColor;
void static Draw_Graphic_Item(Graphics *);
void Process_event(XEvent);
#endif

/**************************************************** Common stuff *****/
static void YieldToSystem(void);
static void GP_clear(void);
#define GP142_NONE  -1

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


/**************************************************** Script prototypes ****/
static void SaveToScript(int a, GP142_event_t e);
static int PlayFromScript(void);
static void CloseScript(void);

#define kScriptName "GP142.SCR"


