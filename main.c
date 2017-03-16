/***********************************************\
*                     main.c                    *
*           George Koskeridis (C) 2016          *
\***********************************************/


#include "common.h"
#include "main.h"
#include "letters.h"

#define STARTING_WIDTH  540
#define STARTING_HEIGHT 500

/*local globals*/
static struct window_data FKatelynMainW = {
    .title = TEXT("for Katelyn")
};

static BOOL running = FALSE;
static HINSTANCE g_hInst;
static POINT carpet_rect[] = {
    {.x = 10, .y = 10},
    {.x = 50, .y = 10},
    {.x = 50, .y = 80},
    {.x = 10, .y = 80}
};

/*globals*/
KatelynStruct katelyn = {
    .numOfPoints = {
        6, 6, 4, 8, 4, 4, 6
    },
    .lett_clr = {
        RGB_ORANGE, RGB_ORANGE,
        RGB_ORANGE, RGB_ORANGE,
        RGB_ORANGE, RGB_ORANGE,
        RGB_ORANGE
    }
};

int KatelynPenWidth[7];

BOOL letters_drawn = FALSE;

/******************************************
* Prototypes for functions with local scope *
 *******************************************/
struct backbuffer_data *NewBackbuffer(HWND hwnd);
BOOL DestroyBackbuffer(struct backbuffer_data *buf);
void DeleteKatelynStruct(void);
void InitKatelynStruct(void);
void UpdateState(void);
int NextCarpet_state(void);
void DrawToWindow(HWND hwnd, struct backbuffer_data *buf);
void DrawToBuffer(HWND hwnd, struct backbuffer_data *buf);
static inline void HandleMessages(void);
static LRESULT CALLBACK MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


struct backbuffer_data *NewBackbuffer(HWND hwnd)
{
    if (!hwnd)
        return (struct backbuffer_data*)NULL;

    HDC hdc = GetDC(hwnd);
    if (!hdc)
        return (struct backbuffer_data*)NULL;

    struct backbuffer_data *new = win_malloc(sizeof(struct backbuffer_data));
    if (new == NULL) {
        ReleaseDC(hwnd, hdc);
        return (struct backbuffer_data*)NULL;
    }

    GetClientRect(hwnd, &new->coords);

    new->hdc = CreateCompatibleDC(hdc);
    if (!(new->hdc)) {
        win_free(new);
        ReleaseDC(hwnd, hdc);
        return (struct backbuffer_data*)NULL;
    }
#ifdef WITH_GDIPLUSFLAT
    GdipCreateFromHDC(new->hdc, &new->gphdc);
#endif

    new->hBitmap = CreateCompatibleBitmap(hdc, new->coords.right, new->coords.bottom);
    if (!(new->hBitmap)) {
#ifdef WITH_GDIPLUSFLAT
        GdipDeleteGraphics(new->gphdc);
#else
        DeleteDC(new->hdc);
#endif
        win_free(new);
        ReleaseDC(hwnd, hdc);
        return (struct backbuffer_data*)NULL;
    }
    HGDIOBJ temp = SelectObject(new->hdc, new->hBitmap);
    DeleteObject(temp);
    ReleaseDC(hwnd, hdc);
    return new;
}

BOOL DestroyBackbuffer(struct backbuffer_data *buf)
{
    BOOL retvalue = FALSE;

    if (buf != NULL) {
        retvalue = ((buf->hBitmap != NULL) ? DeleteObject((HBITMAP)buf->hBitmap) : TRUE) ? TRUE : FALSE;
#ifdef WITH_GDIPLUSFLAT
        GdipDeleteGraphics(buf->gphdc);
#else
        retvalue = ((buf->hdc != NULL) ? DeleteDC(buf->hdc) : TRUE) ? TRUE : FALSE;
#endif
        win_free(buf);
    }
    return retvalue;
}

void DeleteKatelynStruct(void)
{
    for (int i = K; i <= N; i++)
        if (katelyn.letter[i])
            win_free(katelyn.letter[i]);
}

void InitKatelynStruct(void)
{
    for (int i = K; i <= N; i++) {
        katelyn.letter[i] = win_malloc(sizeof(POINT)*katelyn.numOfPoints[i]);
        SecureZeroMemory(katelyn.letter[i], sizeof(POINT)*katelyn.numOfPoints[i]);
    }
}

void UpdateState(void)
{
    static int curr_state = -1;

    switch (curr_state) {
    case -1:
        curr_state = NextCarpet_state();
        break;
    case K:
        curr_state = NextLetterK_state();
        break;
    case A:
        curr_state = NextLetterA_state();
        break;
    case T:
        curr_state = NextLetterT_state();
        break;
    case E:
        curr_state = NextLetterE_state();
        break;
    case L:
        curr_state = NextLetterL_state();
        break;
    case Y:
        curr_state = NextLetterY_state();
        break;
    case N:
        curr_state = NextLetterN_state();
        break;
    case N + 1:
        letters_drawn = TRUE;
        curr_state = K;
        break;
    }
}

int NextCarpet_state(void)
{
    int delta = 10;
    int max_thres_x = 30, max_thres_y = 50;
    int retvalue = -1;

    if ((carpet_rect[2].x <= FKatelynMainW.width - max_thres_x) ||
        (carpet_rect[2].y <= FKatelynMainW.height - max_thres_y)) {
        if (carpet_rect[2].x <= FKatelynMainW.width - max_thres_x) {
            carpet_rect[2].x += delta;
        }
        if (carpet_rect[2].y <= FKatelynMainW.height - max_thres_y) {
            carpet_rect[2].y += delta;
        }
    } else if ((carpet_rect[1].x <= FKatelynMainW.width - max_thres_x) ||
               (carpet_rect[3].y <= FKatelynMainW.height - max_thres_y)) {
        if (carpet_rect[1].x <= FKatelynMainW.width - max_thres_x) {
            carpet_rect[1].x += delta;
        }
        if (carpet_rect[3].y <= FKatelynMainW.height - max_thres_y) {
            carpet_rect[3].y += delta;
        }
    } else {
        retvalue++;
    }

    return retvalue;
}

void DrawToWindow(HWND hwnd, struct backbuffer_data *buf)
{
    HDC hdc = GetDC(hwnd);

    if (!hdc) return;

    BitBlt(hdc, 0, 0, buf->coords.right, buf->coords.bottom, buf->hdc, 0, 0, SRCCOPY);
    ReleaseDC(hwnd, hdc);
}

void DrawToBuffer(HWND hwnd, struct backbuffer_data *buf)
{
    if (buf == NULL) return;
    HGDIOBJ prev;
    //background color
    FillRect(buf->hdc, &buf->coords, (HBRUSH)GetStockObject(DKGRAY_BRUSH));
    //carpet
    LOGBRUSH boundary_pen_attr = {BS_SOLID, RGB_WHITE, NULL};
    HPEN boundary_pen = ExtCreatePen(PS_COSMETIC | PS_NULL | PS_ENDCAP_ROUND | PS_JOIN_BEVEL,
                                     0, &boundary_pen_attr, 0, NULL);
    SelectObject(buf->hdc, boundary_pen);

    HBRUSH background_brush;
    if (!letters_drawn) {
        background_brush = CreateSolidBrush(RGB_PURPLE);
    } else {
        static int counter = 0, timeout = 600;
        int hatch_styles[6] = {HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS,
                               HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL};
        static int curr_style = 0;

        counter++;
        if (counter >= timeout) {
            curr_style = ((curr_style > 5) ? 0 : curr_style + 1);
            counter = 0;
        }
        background_brush = CreateHatchBrush(hatch_styles[curr_style], RGB_PURPLE);
    }
    prev = SelectObject(buf->hdc, background_brush);
    Polygon(buf->hdc, carpet_rect, 4);

    SelectObject(buf->hdc, prev);

    DeleteObject((HGDIOBJ)background_brush);
    DeleteObject((HGDIOBJ)boundary_pen);
    //letters
    for (int i = K; i <= N; i++) {
        //we're looping through half of the points to draw a line that connects both
        for (int j = 0; j < katelyn.numOfPoints[i] - 1; j+=2) { //double step
            HPEN pe = CreatePen(PS_SOLID, KatelynPenWidth[i], katelyn.lett_clr[i]);
            prev = SelectObject(buf->hdc, pe);
            MoveToEx(buf->hdc, katelyn.letter[i][j].x, katelyn.letter[i][j].y, NULL);
            LineTo(buf->hdc, katelyn.letter[i][j + 1].x, katelyn.letter[i][j + 1].y);
            SelectObject(buf->hdc, prev);
            DeleteObject((HGDIOBJ)pe);
        }
    }

}

static inline void HandleMessages(void)
{
    MSG msg;

    if (PeekMessage(&msg, FKatelynMainW.hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

static LRESULT CALLBACK MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //static struct backbuffer_data *buf;

    switch (msg) {
    case WM_CREATE:
        //buf = (struct backbuffer_data *)((LPCREATESTRUCT)lParam)->lpCreateParams;
        break;
    case WM_PAINT:
    {
        HDC hdc;
        PAINTSTRUCT ps;

        if ((hdc = BeginPaint(hwnd, &ps))) {
            //DrawToBuffer(hwnd, buf);
            EndPaint(hwnd, &ps);
        }
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        }
        switch (HIWORD(wParam)) {
        }
        break;
    case WM_CLOSE:
    case WM_DESTROY:
        running = FALSE;
        DestroyWindow(hwnd);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    RECT wrkspace_px;
    LOGBRUSH bckr_data;
#ifdef WITH_GDIPLUSFLAT
    ULONG_PTR gdiplus_token;
    GdiplusStartupInput gdiplus_options = {1, NULL, FALSE, TRUE};
#endif
    struct backbuffer_data *backbuff = NULL;
    const int TICKS_PER_SECOND = 20;
    const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
    const int MAX_FRAMESKIP = 10;
    DWORD next_game_tick = GetTickCount();
    int loops;

#if defined(_DEBUG) && defined(_MSC_VER)
    //Create the console for debugging
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    //https://en.wikipedia.org/wiki/Device_file#DOS.2C_OS.2F2.2C_and_Windows
    freopen("CON", "w", stdout);
#endif

#ifdef WITH_GDIPLUSFLAT
    GdiplusStartup(&gdiplus_token, &gdiplus_options, NULL);
#endif

    InitKatelynStruct();

    FKatelynMainW.width = STARTING_WIDTH;
    FKatelynMainW.height = STARTING_HEIGHT;
    //Get the working desktop area (width x height) in pixels.
    //The taskbar is not included in this area, so it's not always
    //the resolution of the monitor that shows the program.
    SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&wrkspace_px, SPIF_SENDCHANGE);
    wrkspace_px.right = (wrkspace_px.right - FKatelynMainW.width)/2;
    wrkspace_px.bottom = (wrkspace_px.bottom - FKatelynMainW.height)/2;

    g_hInst = hInstance;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.lpfnWndProc = MainWindowProcedure;
    wc.hInstance = hInstance;
    wc.hIconSm = wc.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCEW(IDI_FORKATELYN32_ICON),
                                             IMAGE_ICON, GetSystemMetrics(SM_CXSMICON),
                                             GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = TEXT("forKatelyn");

    GetObject((HGDIOBJ)wc.hbrBackground, sizeof(LOGBRUSH), (LPVOID)&bckr_data);
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, TEXT("Main Window class registration Failed!"), TEXT("Something happened!"), MB_ICONERROR | MB_OK);
        return 1;
    }

    FKatelynMainW.hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, wc.lpszClassName, FKatelynMainW.title,
                          WS_BORDER | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                          wrkspace_px.right, wrkspace_px.bottom,
                          FKatelynMainW.width, FKatelynMainW.height, NULL, NULL, hInstance, (LPVOID)&backbuff);

    if (!FKatelynMainW.hwnd) {
        MessageBox(NULL, TEXT("Window creation failed!"), TEXT("Something happened!"), MB_ICONERROR | MB_OK);
        return 1;
    }
    //InitCommonControls();
    
    ShowWindow(FKatelynMainW.hwnd, nCmdShow);
    UpdateWindow(FKatelynMainW.hwnd);
    running = TRUE;
    backbuff = NewBackbuffer(FKatelynMainW.hwnd);

    while (running) {
        DrawToBuffer(FKatelynMainW.hwnd, backbuff);
        DrawToWindow(FKatelynMainW.hwnd, backbuff);
        loops = 0;

        while (GetTickCount() > next_game_tick && loops < MAX_FRAMESKIP) {
            HandleMessages();
            UpdateState();
            next_game_tick += SKIP_TICKS;
            loops++;
        }
    }
    DeleteKatelynStruct();

    if (!DestroyBackbuffer(backbuff)) {
        MessageBox(NULL, TEXT("Failed deleting backbuffer!"), TEXT("Something happened!"), MB_ICONERROR | MB_OK);
    }
#ifdef WITH_GDIPLUSFLAT
    GdiplusShutdown(gdiplus_token);
#endif

#if defined(_DEBUG) && defined(_MSC_VER)
    FreeConsole();
#endif
    return 0;
}
