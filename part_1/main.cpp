#include <iostream>
#include <string>
#include <windows.h>

#define RECORDS_FILEPATH "./record_list"

using namespace std;

struct record {
    unsigned char index;
    FILETIME created_at;
    char data[80];
    unsigned char access_count;
};

enum program_mode {
    PM_CREATE_FILE,
    PM_EDIT_FILE,
    PM_EXIT
};

enum edit_file_mode {
    EFM_PRINT_RECORD,
    EFM_DELETE_RECORD,
    EFM_EDIT_RECORD,
    EFM_EXIT
};

string format_systemtime(SYSTEMTIME &st)
{
    return to_string(st.wDay) + "." + to_string(st.wMonth) + "." + to_string(st.wYear) + " "
        + to_string(st.wHour) + ":" + to_string(st.wMinute) + ":" + to_string(st.wSecond);
}

void create_file()
{
    unsigned short file_size;
    unsigned char file_records_count;

    SYSTEMTIME system_time;
    FILETIME created_at;

    FILE *file = fopen(RECORDS_FILEPATH, "w");

    cout << "Enter number of records: ";
    scanf("%hhu", &file_records_count);

    //number of non-empty records
    unsigned char b = '0';
    fwrite(&b, sizeof(char), 1, file);

    //placeholder for file size
    fwrite(&file_size, sizeof(short), 1, file);

    //records' placeholders
    for (unsigned char i = 0; i < (int)file_records_count; i++)
    {
        GetSystemTime(&system_time);
        SystemTimeToFileTime(&system_time, &created_at);

        struct record r =
        {
            i,
            created_at,
            "",
            0
        };

        fwrite(&r, sizeof(struct record), 1, file);
    }

    //actual file size
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);

    fseek(file, sizeof(char), SEEK_SET);
    fwrite(&file_size, sizeof(short), 1, file);

    fclose(file);
}

bool find_record(record &r, unsigned char index) {
    FILE *file = fopen(RECORDS_FILEPATH, "r+");
    SYSTEMTIME local_time;

    bool is_exist = false;

    fseek(file, sizeof(char) + sizeof(short), SEEK_SET);

    while (!is_exist && fread(&r, sizeof(record), 1, file))
    {
        if (r.index == index) {
            is_exist = true;
        }
    }

    fclose(file);

    return is_exist;
}

void print_record(record &r) {
    SYSTEMTIME created_at;
    FileTimeToSystemTime(&(r.created_at), &created_at);

    cout << "Record:" << endl;

    if ((string)r.data == "")
    {
        cout << "No data was found" << endl;
    }
    else
    {
        cout << "Data: " << r.data << endl;
    }

    cout << "Created at: " << format_systemtime(created_at) << endl;
    cout << "Number of modifications: " << (int)r.access_count << endl;
}

void delete_record(record &r) {
    FILE *file = fopen(RECORDS_FILEPATH, "r+");
    SYSTEMTIME system_time;
    FILETIME created_at;

    unsigned char index = r.index;

    if ((string)r.data != "") {
        unsigned char b = '0';

        // change number of non empty records
        fseek(file, 0, SEEK_SET);
        fread(&b, sizeof(char), 1, file);

        b -= 1;
        fseek(file, 0, SEEK_SET);
        fwrite(&b, sizeof(char), 1, file);
    }

    // as a 'deletion' of record was chosen to place new empty record on it's place 
    fseek(file, sizeof(char) + sizeof(short) + (int)index * sizeof(struct record), SEEK_SET);

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &created_at);

    struct record empty =
    {
        index,
        created_at,
        "",
        0
    };

    fwrite(&empty, sizeof(struct record), 1, file);
    fclose(file);
}

void edit_record(record &r) {
    FILE *file = fopen(RECORDS_FILEPATH, "r+");
    char cont[80];

    cout << "Enter new record data: ";

    cin.getline(cont, sizeof(cont));
    cin.getline(cont, sizeof(cont));
    cin.clear();
    cin.ignore(INT_MAX, '\n');

    if ((string)cont != "")
    {
        unsigned char b = '0';

        //change number of non empty records
        fseek(file, 0, SEEK_SET);
        fread(&b, sizeof(char), 1, file);

        b += 1;
        fseek(file, 0, SEEK_SET);
        fwrite(&b, sizeof(char), 1, file);
    }

    strncpy(r.data, cont, sizeof(r.data));
    r.access_count++;

    fseek(file, sizeof(char) + sizeof(short) + (int)r.index * sizeof(struct record), SEEK_SET);
    fwrite(&r, sizeof(struct record), 1, file);

    fclose(file);
}

void edit_file_menu() {
    int efm_mode;
    bool is_exit = false;

    do
    {
        cout << "Edit file menu. Choose operation:" << endl
            << "0 - show record" << endl
            << "1 - delete record" << endl
            << "2 - edit record" << endl
            << "3 - return" << endl;
        cin >> efm_mode;

        if (efm_mode >= EFM_PRINT_RECORD && efm_mode <= EFM_EDIT_RECORD)
        {
            record r;
            unsigned char index;

            cout << "Enter index of record: ";
            scanf("%hhu", &index);

            bool is_exist = find_record(r, index);

            if (is_exist)
            {
                switch (efm_mode)
                {
                    case EFM_PRINT_RECORD:
                    {
                        print_record(r);
                        break;
                    }
                    case EFM_DELETE_RECORD:
                    {
                        delete_record(r);
                        break;
                    }
                    case EFM_EDIT_RECORD:
                    {
                        edit_record(r);
                        break;
                    }
                }
            }
            else
            {
                cout << "Record with specified index doesn't exist." << endl;
            }
        }
        else if (efm_mode == EFM_EXIT)
        {
            is_exit = true;
        }
        else
        {
            cout << "Invalid operation." << endl;
        }
    }
    while (!is_exit);
}

int main()
{
    int pm_mode;
    bool is_exit = false;

    do
    {
        cout << "Main menu. Choose operation:" << endl
            << "0 - create new file" << endl
            << "1 - edit file" << endl
            << "2 - close program" << endl;
        cin >> pm_mode;

        switch (pm_mode)
        {
            case PM_EXIT:
            {
                is_exit = true;
                break;
            }
            case PM_CREATE_FILE:
            {
                create_file();
                break;
            }
            case PM_EDIT_FILE:
            {
                edit_file_menu();
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
