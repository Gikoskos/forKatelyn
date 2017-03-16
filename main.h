/***********************************************\
*                     main.h                    *
*           George Koskeridis (C) 2016          *
\***********************************************/

#ifndef __MAIN_H
#define __MAIN_H

struct window_data {
    HWND hwnd; //window handle
    LPTSTR title;
    int width, height;
};

struct backbuffer_data {
    HBITMAP hBitmap;
#ifdef WITH_GDIPLUSFLAT
    GpGraphics *gphdc;
#endif
    HDC hdc;
    RECT coords;
};


typedef struct _KatelynStruct {
    POINT *letter[7];
    UINT numOfPoints[7];
    COLORREF lett_clr[7];
} KatelynStruct;

enum {
    K = 0,
    A,
    T,
    E,
    L,
    Y,
    N
};

extern KatelynStruct katelyn;

extern int KatelynPenWidth[7];

extern BOOL letters_drawn;

#endif //__MAIN_H
