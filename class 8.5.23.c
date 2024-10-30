#include <advanlys.h>
#include <ansi_c.h>
#include <rs232.h>
#include <cvirte.h>		
#include <userint.h>
#include "class 8.5.23.h"

static int panelHandle;
char data[100];
unsigned long int ldrarray[10000];
unsigned long int timearray[10000];
int counter=0 ; 
int u; // how many times graph write points
double duty[10];
double sensor[10];
double  PolyF[100];
double  PolyEv[100];
double trash[9];
double error;
double Xpoly_arr[100];
double Ypoly_arr[100];


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "class 8.5.23.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK ExitFunc (int panel, int event, void *callbackData,
						  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

void CVICALLBACK ReactToArduino (int portNumber, int eventMask, void *callbackData)
{

	int bytesRead;
	static int lines = 0;
	int color;
	
	while (GetInQLen (6) > 0)
		{
		bytesRead = ComRdTerm (6, data, 100, '\n');
		if (bytesRead == 0) return;
		data[bytesRead - 1] = '\0';
		
		
		InsertTextBoxLine (panelHandle, PANEL_TEXTBOX, -1, data + 2);
		lines++;
		SetCtrlAttribute (panelHandle, PANEL_TEXTBOX, ATTR_FIRST_VISIBLE_LINE, lines);
		switch (data[0])//type
			
		{
			
			case '1':
				sscanf(data+2,"%lu,%d",&timearray[counter],&ldrarray[counter]);	
				counter++;
				u = 0;
				break;
				
			case '*':
				GetCtrlVal (panelHandle,PANEL_COLORNUM, &color);
				PlotXY (panelHandle, PANEL_GRAPH, timearray, ldrarray, counter, VAL_UNSIGNED_INTEGER, VAL_INTEGER, VAL_FAT_LINE, VAL_DOTTED_SOLID_CIRCLE, VAL_SOLID, 1, color);
				counter = 0;
				break;
				
			case '2':
				sscanf(data+2,"%lf,%lf",&sensor[u],&duty[u]);
				PlotPoint (panelHandle, PANEL_GRAPH_2, sensor[u], duty[u], VAL_EMPTY_SQUARE, VAL_RED);
				u++;
				break;
				
		}	
	}
	
}

int CVICALLBACK connectDISconnect (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	int button_state;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_TOGGLEBUTTON, &button_state);			
			if (button_state)
			{
				OpenComConfig (6, "", 230400, 0, 8, 1, 512, 512);
				InstallComCallback (6, LWRS_RXFLAG, 0, '\n', ReactToArduino, 0);
				FlushInQ (6);
			}
			else
			{
				CloseCom (6);
			}
			break;
	}
	return 0;
}

int CVICALLBACK SendText (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_STRING, data);
			ComWrt (6, data, strlen(data) + 1);
			
			

			break;
	}
	return 0;
}

int CVICALLBACK GREEN (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	int green=0;
	char data[10];
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle,PANEL_BINARYSWITCH,&green);
			sprintf(data,"1,%d",green);
			ComWrt (6, data, strlen(data));
			break;
	}
	return 0;
}


int CVICALLBACK BLUEfunc (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	int blue=0;
	char data[10];
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle,PANEL_BINARYSWITCH_2,&blue);
			sprintf(data,"2,%d",blue);
			ComWrt (6, data, strlen(data));

			break;
	}
	return 0;
}

int CVICALLBACK CLEARGRAPH (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			DeleteGraphPlot (panelHandle, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
			DeleteGraphPlot (panelHandle, PANEL_GRAPH_2, -1, VAL_IMMEDIATE_DRAW);
			break;
	}
	return 0;
}

int CVICALLBACK Poly (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	int q;
	double r;
	switch (event)
	{
		case EVENT_COMMIT:
				PolyFit (sensor, duty, 8, 4, trash, PolyF, &error);
				for(r=67,q=0;r<=767,q<100;r+=7,q++)
				{
					Xpoly_arr[q] = r;
				}
				PolyEv1D(Xpoly_arr,100,PolyF,6,Ypoly_arr);
			break;
	}
	return 0;
}

int CVICALLBACK Calc (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			PolyFit (sensor, duty, 8, 4, trash, PolyF, &error);
			GetCtrlVal(panelHandle, PANEL_NUMERIC_2,Xpoly_arr);
			PolyEv1D(Xpoly_arr,1,PolyF,6,Ypoly_arr);
			SetCtrlVal(panelHandle,PANEL_NUMERIC_3,Ypoly_arr[0]);
			

			break;
	}
	return 0;
}
