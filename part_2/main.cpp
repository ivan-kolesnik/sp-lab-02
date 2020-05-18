#include <iostream>
#include <string>
#include <windows.h>
#include <strsafe.h>
#include <tchar.h>

#define BUF_SIZE MAX_PATH

using namespace std;

string format_systemtime(SYSTEMTIME &st)
{
    return to_string(st.wDay) + "." + to_string(st.wMonth) + "." + to_string(st.wYear) + " "
        + to_string(st.wHour) + ":" + to_string(st.wMinute) + ":" + to_string(st.wSecond);
}

void on_error() {
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0,
        NULL
    );

    wprintf(L"error code %d: %s\n", dw, lpMsgBuf);
}

void change_directory() {
    string new_path;

    cout << "Change directory to: ";
    getline(cin, new_path);
    getline(cin, new_path);

    wstring stemp = wstring(new_path.begin(), new_path.end());
    LPCWSTR destination = stemp.c_str();

    if (!SetCurrentDirectory(destination))
    {
        on_error();
    }
}

void list_files(TCHAR path[BUF_SIZE]) {
    HANDLE h_find = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA found_file_data;
    LARGE_INTEGER file_size;
    SYSTEMTIME st_creation_time;
    FILETIME ft_creation_time;

    StringCchCat(path, MAX_PATH, TEXT("\\*"));
    h_find = FindFirstFile(path, &found_file_data);

    if (h_find != INVALID_HANDLE_VALUE)
    {
        _tprintf(TEXT("\t%-30s%-10s%-10s%s\n"), L"NAME", L"TYPE", L"SIZE (b)", L"CREATION_TIME");

        do
        {
            if (found_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                _tprintf(TEXT("\t%-30s%-10s\n"), found_file_data.cFileName, L"<DIR>");
            }
            else
            {
                file_size.LowPart = found_file_data.nFileSizeLow;
                file_size.HighPart = found_file_data.nFileSizeHigh;

                _tprintf(TEXT("\t%-30s%-10s%-10ld"), found_file_data.cFileName, L"<FILE>", file_size.QuadPart);

                FileTimeToLocalFileTime(&found_file_data.ftCreationTime, &ft_creation_time);
                FileTimeToSystemTime(&ft_creation_time, &st_creation_time);

                cout << format_systemtime(st_creation_time) << endl;
            }
        }
        while (FindNextFile(h_find, &found_file_data) != 0);

        if (GetLastError() != ERROR_NO_MORE_FILES)
        {
            on_error();
        }

        FindClose(h_find);
    }
    else
    {
        on_error();
    }
}

void copy_file() {
    string new_path;

    cout << "Copy file (path): ";
    getline(cin, new_path);
    getline(cin, new_path);

    wstring stemp1 = wstring(new_path.begin(), new_path.end());
    LPCWSTR source = stemp1.c_str();

    cout << " to: ";
    getline(cin, new_path);

    wstring stemp = wstring(new_path.begin(), new_path.end());
    LPCWSTR destination = stemp.c_str();

    if (CopyFile(source, destination, 1) == 0)
    {
        on_error();
    }
}

void create_directory() {
    string new_path;

    cout << "Directory (path): ";
    getline(cin, new_path);
    getline(cin, new_path);

    wstring stemp = wstring(new_path.begin(), new_path.end());
    LPCWSTR destination = stemp.c_str();

    if (CreateDirectoryW(destination, NULL) == 0)
    {
        if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
            cout << "Specified directory already exists." << endl;
        }
        else
        {
            cout << "One or more intermediate directories do not exist" << endl;
        }
    }
}

void delete_file_or_empty_directory() {
    string new_path;

    cout << "Path: ";
    getline(cin, new_path);
    getline(cin, new_path);

    wstring stemp = wstring(new_path.begin(), new_path.end());
    LPCWSTR destination = stemp.c_str();

    DWORD ftyp = GetFileAttributesW(destination);

    if (ftyp == INVALID_FILE_ATTRIBUTES)
    {
        on_error();
    }
    else if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    {
        if (RemoveDirectoryW(destination) == 0)
        {
            on_error();
        }
    }
    else
    {
        if (DeleteFileW(destination) == 0)
        {
            if (GetLastError() == ERROR_FILE_NOT_FOUND)
            {
                cout << "File wasn't found.\n";
            }
            else
            {
                cout << "File is inaccessible.\n";
            }
        }
    }
}

void current_directory(TCHAR* path) {
    TCHAR buf[BUF_SIZE];
    DWORD dwRet = GetCurrentDirectory(BUF_SIZE, buf);

    if (dwRet == 0)
    {
        cout << "GetCurrentDirectory failed (" << GetLastError() << ")" << endl;
        return;
    }

    if (dwRet > BUF_SIZE)
    {
        cout << "Buffer too small; need " << dwRet << " characters" << endl;
        return;
    }

    for (int i = 0; i < BUF_SIZE; ++i)
    {
        path[i] = buf[i];
    }
}

void extended_file_info() {
    string new_path;
    HANDLE h_find = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA found_file_data;
    LARGE_INTEGER file_size;
    SYSTEMTIME st_creation_time;
    FILETIME ft_creation_time;

    cout << "File name (path): ";
    getline(cin, new_path);
    getline(cin, new_path);

    wstring stemp = wstring(new_path.begin(), new_path.end());
    LPCWSTR destination = stemp.c_str();

    h_find = FindFirstFileW(destination, &found_file_data);

    if (h_find != INVALID_HANDLE_VALUE)
    {
        file_size.LowPart = found_file_data.nFileSizeLow;
        file_size.HighPart = found_file_data.nFileSizeHigh;
        _tprintf(TEXT("Name: %s\n Size: %ld bytes\n"), found_file_data.cFileName, file_size.QuadPart);

        FileTimeToLocalFileTime(&found_file_data.ftCreationTime, &ft_creation_time);
        FileTimeToSystemTime(&ft_creation_time, &st_creation_time);
        cout << "Creation Time: " << format_systemtime(st_creation_time) << endl;

        FileTimeToLocalFileTime(&found_file_data.ftLastAccessTime, &ft_creation_time);
        FileTimeToSystemTime(&ft_creation_time, &st_creation_time);
        cout << "Last Access Time: " << format_systemtime(st_creation_time) << endl;

        FileTimeToLocalFileTime(&found_file_data.ftLastWriteTime, &ft_creation_time);
        FileTimeToSystemTime(&ft_creation_time, &st_creation_time);
        cout << "Last Write Time: " << format_systemtime(st_creation_time) << endl;

        cout << "File Attributes: " << found_file_data.dwFileAttributes << endl;
    }
    else
    {
        on_error();
    }
}

int main()
{
    enum modes {
        CHANGE_DIR,
        LIST_FILES,
        COPY_FILE,
        CREATE_DIR,
        DELETE_FILE_OR_EMPTY_DIR,
        EXTENDED_FILE_INFO,
        EXIT
    };

    int mode;
    bool is_exit = false;

    TCHAR path[BUF_SIZE];

    cout << "Welcome to the File Manager!" << endl << endl;

    do
    {
        current_directory(path);

        _tprintf(TEXT("Current directory: %s\n"), path);
        cout << "Options:" << endl
            << "0 - change directory" << endl
            << "1 - list files in current directory" << endl
            << "2 - copy file" << endl
            << "3 - create directory" << endl
            << "4 - delete file (empty directory)" << endl
            << "5 - extended file info" << endl
            << "6 - close program" << endl;
        cin >> mode;

        switch (mode)
        {
            case CHANGE_DIR:
            {
                change_directory();
                break;
            }
            case LIST_FILES:
            {
                list_files(path);
                break;
            }
            case COPY_FILE:
            {
                copy_file();
                break;
            }
            case CREATE_DIR:
            {
                create_directory();
                break;
            }
            case DELETE_FILE_OR_EMPTY_DIR:
            {
                delete_file_or_empty_directory();
                break;
            }
            case EXTENDED_FILE_INFO:
            {
                extended_file_info();
                break;
            }
            case EXIT:
            {
                is_exit = true;
                break;
            }
            default:
            {
                cout << "Invalid operation." << endl;
                break;
            }  
        }
    }
    while (!is_exit);
}
