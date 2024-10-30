/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: ExitFunc */
#define  PANEL_TEXTBOX                    2       /* control type: textBox, callback function: (none) */
#define  PANEL_TOGGLEBUTTON               3       /* control type: textButton, callback function: connectDISconnect */
#define  PANEL_NUMERIC                    4       /* control type: numeric, callback function: (none) */
#define  PANEL_LED                        5       /* control type: LED, callback function: (none) */
#define  PANEL_NUMERICTANK                6       /* control type: scale, callback function: (none) */
#define  PANEL_STRING                     7       /* control type: string, callback function: SendText */
#define  PANEL_BINARYSWITCH_2             8       /* control type: binary, callback function: BLUEfunc */
#define  PANEL_BINARYSWITCH               9       /* control type: binary, callback function: GREEN */
#define  PANEL_COLORNUM                   10      /* control type: color, callback function: (none) */
#define  PANEL_GRAPH_2                    11      /* control type: graph, callback function: (none) */
#define  PANEL_GRAPH                      12      /* control type: graph, callback function: (none) */
#define  PANEL_COMMANDBUTTON              13      /* control type: command, callback function: CLEARGRAPH */
#define  PANEL_NUMERIC_2                  14      /* control type: numeric, callback function: (none) */
#define  PANEL_COMMANDBUTTON_2            15      /* control type: command, callback function: Poly */
#define  PANEL_NUMERIC_3                  16      /* control type: numeric, callback function: (none) */
#define  PANEL_COMMANDBUTTON_3            17      /* control type: command, callback function: Calc */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK BLUEfunc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Calc(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CLEARGRAPH(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK connectDISconnect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ExitFunc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GREEN(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Poly(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SendText(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif