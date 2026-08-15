#define WIN32_LEAN_AND_MEAN //define to cut out needless subsystems.

#include <stdio.h>
#include <windows.h>
#include <shellapi.h>

int mainwindowwidth = 250;
int mainwindowheight = 125;


HWND mainwindowhandle; //declare window handle
HWND edithandle; //declares edit handle, since each "field" of a window needs its own handle. This is for the text box.

WNDCLASS windowclass = {0}; //creates windowclass struct to later be registered as a class and sets all values to 0
WNDPROC editoriginalwndproc; //used to store pointer to EDIT classes wndproc

MSG msg; //initialize a struct of type MSG to hold the message data

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); //func declarations
LRESULT CALLBACK EditSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
char *ConvertSpaces(char *buffer, size_t buffersize);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){ //standard winmain function

    windowclass.lpfnWndProc = WindowProc; //pointer to function that is called on msg
    windowclass.hInstance = hInstance; //imports handle to instance that windows creates for us so kindly
    windowclass.lpszClassName = "HotkeyBoxClass"; //names the class
    windowclass.hCursor = LoadCursor(NULL, IDC_ARROW); //sets cursor to win default
    
    RegisterClass(&windowclass); //pushes struct info to windows to create class
    RegisterHotKey(NULL, 1, MOD_SHIFT, 'M'); //registers our hotkey, NULL for the window handle, 1 for the id, MOD_SHIFT for the modifier key, and 'M' for the key to be pressed.

    //creates window handle for our Specific window, lots of args see winapi docs for info
    mainwindowhandle = CreateWindowEx(0, windowclass.lpszClassName,"MC WikiSearch", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, mainwindowwidth, mainwindowheight, NULL, NULL, hInstance, NULL);
    edithandle = CreateWindowEx(0, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, mainwindowwidth - 35, 25, mainwindowhandle, NULL, hInstance, NULL); //actually creates the text box using the built in windows class EDIT.
    
    editoriginalwndproc = (WNDPROC)SetWindowLongPtr(edithandle, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc); //gets pointer to original wndproc (returned by SetWindowLongPtr) and replaces it with EditSubclassProc.




    
    while(GetMessage(&msg, NULL, 0, 0)){ //calls getmessage and passes in a pointer to msg struct, NULL to capture any window, 0 for both filters.
        
        if (msg.message == WM_HOTKEY){ //checks for hotkey pressed messages

            ShowWindow(mainwindowhandle, SW_SHOW); //spawns (shows) window on hotkey detected    
        } 

        TranslateMessage(&msg); //translates message used for text entry later
        DispatchMessage(&msg); //dispatches message (calls WindowProc)
     }
    
     return 0;

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){ //func that is called when msg is received. Only case here is for quitting, call postquitmessage to correctly close the while loop.
    switch(msg){
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK EditSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    char buffer[256];

    if(msg == WM_KEYDOWN && wParam == VK_RETURN){
        //code for wiki
       GetWindowText(hwnd, buffer, sizeof(buffer) - 1);
       //MessageBox(NULL, buffer, "Debug", MB_OK);
        char url[256];
       sprintf(url, "https://minecraft.wiki/w/%s", ConvertSpaces(buffer, sizeof(buffer) - 1));

       ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
       SetWindowText(hwnd, "");
       ShowWindow(hwnd, SW_HIDE);

       //MessageBox(NULL, url, "Debug", MB_OK);

        return 0; //needed because when wndproc is called it returns 
    }
    
    
    return CallWindowProc(editoriginalwndproc, hwnd, msg, wParam, lParam); //winapi function to get pointer to the handle's windowproc
}

    char *ConvertSpaces(char *buffer, size_t buffersize){
        for(int i = 1; i < buffersize; i++){
            if(buffer[i] == ' '){
                buffer[i] = '_';
            }
        }
        return buffer;
    }
