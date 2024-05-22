#include <windows.h>
#include <commdlg.h>
#include <stdio.h>

int main() {
    OPENFILENAME ofn;
    char file_name[100];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(file_name);
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;

    if(GetOpenFileName(&ofn)) {
        printf("Selected file: %s\n", file_name);
    } else {
        printf("No file selected.\n");
    }

    return 0;
}
