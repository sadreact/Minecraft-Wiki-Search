#define WIN32_LEAN_AND_MEAN //define to cut out needless subsystems.

#include <stdio.h>
#include <windows.h>
#include <shellapi.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); //func declaration

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){ //standard winmain function
    
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "HotkeyBoxClass";
    
    
    
    RegisterHotKey(NULL, 1, MOD_SHIFT, 'M'); //registers our hotkey, NULL for the window handle, 1 for the id, MOD_SHIFT for the modifier key, and 'm' for the key to be pressed.

    MSG msg; //initialize a struct of type MSG to hold the message data.
    while(GetMessage(&msg, NULL, 0, 0)){ //calls getmessage and passes in a pointer to msg struct, NULL to capture any window, 0 for both filters.
        if (msg.message == WM_HOTKEY){ //checks for hotkey pressed messages

            
            
            
            
            
            //MessageBox(NULL, "Hotkey pressed.", "Test", MB_OK); //spawns a message box, NULL to capture all windows, message text, title text, and gives an ok button

        } 
        TranslateMessage(&msg);
        DispatchMessage(&msg);

     }

    return 0;

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    switch(msg){
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}