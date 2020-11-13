// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "Phonebook.h"
#include "Index.h"

#define MEMORY_NAME TEXT("PhoneBook")

static LPVOID lpvMem = NULL;
static HANDLE hMapObject = NULL;
static CRITICAL_SECTION cSection;

static int objectCount;
static std::vector<PhonebookRecord*> phoneBook;
static std::wstring filePath;
Index<wchar_t[20]>* telephoneIndex, * lastNameIndex, * firstNameIndex, * patronymicIndex, * streetIndex;
Index<unsigned int>* houseIndex, * housingIndex, * apartamentIndex;

std::wstring GetDirectory();
std::vector<PhonebookRecord> ReadFromFile(std::wstring filePath);
VOID WriteToFile(std::wstring filePath, std::vector<PhonebookRecord> vector);
std::vector<PhonebookRecord*> Intersection(std::vector<PhonebookRecord*> first, std::vector<PhonebookRecord*> second);
PhonebookRecord ParseLine(std::wstring line);
VOID CreateIndex(std::vector<PhonebookRecord*> phoneBook);

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        filePath = GetDirectory() + TEXT("\\db.txt");
        hMapObject = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MEMORY_NAME);
        if (hMapObject == NULL) {
            InitializeCriticalSection(&cSection);
            std::vector<PhonebookRecord> result = ReadFromFile(filePath);
            objectCount = result.size();
            hMapObject = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(PhonebookRecord) * objectCount + sizeof(objectCount) + sizeof(cSection), MEMORY_NAME);
            lpvMem = MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(PhonebookRecord) * objectCount + sizeof(objectCount) + sizeof(cSection));

            CopyMemory(lpvMem, &objectCount, sizeof(objectCount));
            CopyMemory((LPVOID)((::size_t)lpvMem + sizeof(objectCount)), &cSection, sizeof(cSection));
            for (int i = 0; i < objectCount; i++) {
                CopyMemory((LPVOID)((::size_t)lpvMem + i * sizeof(PhonebookRecord) + sizeof(objectCount) + sizeof(cSection)), &result[i], sizeof(PhonebookRecord));
                phoneBook.push_back((PhonebookRecord*)((::size_t)lpvMem + i * sizeof(PhonebookRecord) + sizeof(objectCount) + sizeof(cSection)));
            }
        }
        else {
            lpvMem = MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, 0);
            objectCount = *(int*)lpvMem;
            cSection = *(CRITICAL_SECTION*)((::size_t)lpvMem + sizeof(objectCount));
            for (int i = 0; i < objectCount; i++) {
                phoneBook.push_back((PhonebookRecord*)((::size_t)lpvMem + i * sizeof(PhonebookRecord) + sizeof(objectCount) + sizeof(cSection)));
            }
        }
        CreateIndex(phoneBook);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
    {
        std::vector<PhonebookRecord> vector;
        for (int i = 0; i < objectCount; i++) {
            vector.push_back(*phoneBook[i]);
        }

        UnmapViewOfFile(lpvMem);
        CloseHandle(hMapObject);

        hMapObject = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MEMORY_NAME);
        if (hMapObject == NULL) {
            WriteToFile(filePath, vector);
            CloseHandle(hMapObject);
        }
        break;
    }
    }
    return TRUE;
    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(lpReserved);
}

PHONEBOOKCORE_API std::vector<PhonebookRecord*> __cdecl GetPhonebook()
{
    return phoneBook;
}

PHONEBOOKCORE_API std::vector<PhonebookRecord*> __cdecl Search(PhonebookRecord searchParam)
{
    bool isEmpty = true;
    std::vector<PhonebookRecord*> result;
    if (!((std::wstring)searchParam.telephone).empty()) {
        result = Intersection(result, telephoneIndex->Search(telephoneIndex->root, searchParam.telephone));
        isEmpty &= false;
    }
    if (!((std::wstring)searchParam.lastName).empty()) {
        result = Intersection(result, lastNameIndex->Search(lastNameIndex->root, searchParam.lastName));
        isEmpty &= false;
    }
    if (!((std::wstring)searchParam.firstName).empty()) {
        result = Intersection(result, firstNameIndex->Search(firstNameIndex->root, searchParam.firstName));
        isEmpty &= false;
    }
    if (!((std::wstring)searchParam.patronymic).empty()) {
        result = Intersection(result, patronymicIndex->Search(patronymicIndex->root, searchParam.patronymic));
        isEmpty &= false;
    }
    if (!((std::wstring)searchParam.streetName).empty()) {
        result = Intersection(result, streetIndex->Search(streetIndex->root, searchParam.streetName));
        isEmpty &= false;
    }
    if (searchParam.houseNumber != 0) {
        result = Intersection(result, houseIndex->Search(houseIndex->root, searchParam.houseNumber));
        isEmpty &= false;
    }
    if (searchParam.housingNumber != 0) {
        result = Intersection(result, housingIndex->Search(housingIndex->root, searchParam.housingNumber));
        isEmpty &= false;
    }
    if (searchParam.apartamentNumber != 0) {
        result = Intersection(result, apartamentIndex->Search(apartamentIndex->root, searchParam.apartamentNumber));
        isEmpty &= false;
    }

    if (!isEmpty) {
        return result;
    }
    else {
        return phoneBook;
    }
}

PHONEBOOKCORE_API VOID __cdecl ChangeData(PhonebookRecord* changedItem, int subItem, std::wstring changedData)
{
    while (TRUE) {
        if (TryEnterCriticalSection(&cSection)) {
            switch (subItem) {
            case 0:
                ZeroMemory(changedItem->telephone, sizeof(changedItem->telephone));
                std::copy(std::begin(changedData), std::end(changedData), std::begin(changedItem->telephone));
                break;
            case 1:
                ZeroMemory(changedItem->lastName, sizeof(changedItem->lastName));
                std::copy(std::begin(changedData), std::end(changedData), std::begin(changedItem->lastName));
                break;
            case 2:
                ZeroMemory(changedItem->firstName, sizeof(changedItem->firstName));
                std::copy(std::begin(changedData), std::end(changedData), std::begin(changedItem->firstName));
                break;
            case 3:
                ZeroMemory(changedItem->patronymic, sizeof(changedItem->patronymic));
                std::copy(std::begin(changedData), std::end(changedData), std::begin(changedItem->patronymic));
                break;
            case 4:
                ZeroMemory(changedItem->streetName, sizeof(changedItem->streetName));
                std::copy(std::begin(changedData), std::end(changedData), std::begin(changedItem->streetName));
                break;
            case 5:
                try
                {
                    changedItem->houseNumber = std::stoi(changedData);
                }
                catch (...) {}
                break;
            case 6:
                try
                {
                    changedItem->housingNumber = std::stoi(changedData);
                }
                catch (...) {}
                break;
            case 7:
                try
                {
                    changedItem->apartamentNumber = std::stoi(changedData);
                }
                catch (...) {}
                break;
            }

            LeaveCriticalSection(&cSection);
            break;
        }
    }
}

std::wstring GetDirectory()
{
    TCHAR currentDirectory[256];
    GetCurrentDirectory(sizeof(currentDirectory), currentDirectory);
    return currentDirectory;
}

std::vector<PhonebookRecord> ReadFromFile(std::wstring filePath)
{
    std::vector<PhonebookRecord> result;
    std::wstring line;
    std::wifstream file(filePath);
    if (file.is_open()) {
        while (getline(file, line)) {
            result.push_back(ParseLine(line));
        }
    }
    file.close();
    return result;
}

VOID WriteToFile(std::wstring filePath, std::vector<PhonebookRecord> vector)
{
    std::wstring line;
    std::wofstream file(filePath);
    if (file.is_open()) {
        for (int i = 0; i < vector.size(); i++) {
            line = (std::wstring)vector[i].telephone + L"|" + (std::wstring)vector[i].lastName + L"|" + (std::wstring)vector[i].firstName + L"|" + (std::wstring)vector[i].patronymic + L"|" + (std::wstring)vector[i].streetName + L"|" + std::to_wstring(vector[i].houseNumber) + L"|" + std::to_wstring(vector[i].housingNumber) + L"|" + std::to_wstring(vector[i].apartamentNumber) + L"|";
            file << line << std::endl;
        }
    }
    file.close();
}

PhonebookRecord ParseLine(std::wstring line)
{
    PhonebookRecord newRecord;
    ZeroMemory(&newRecord, sizeof(newRecord));
    std::vector<std::wstring> tokens;

    int pos = 0;
    std::wstring token;
    while ((pos = line.find(L"|")) != std::string::npos) {
        tokens.push_back((token = line.substr(0, pos)) != L"" ? token : NULL);
        line.erase(0, pos + 1);
    }
    std::copy(std::begin(tokens[0]), std::end(tokens[0]), std::begin(newRecord.telephone));
    std::copy(std::begin(tokens[1]), std::end(tokens[1]), std::begin(newRecord.lastName));
    std::copy(std::begin(tokens[2]), std::end(tokens[2]), std::begin(newRecord.firstName));
    std::copy(std::begin(tokens[3]), std::end(tokens[3]), std::begin(newRecord.patronymic));
    std::copy(std::begin(tokens[4]), std::end(tokens[4]), std::begin(newRecord.streetName));
    newRecord.houseNumber = (unsigned int)std::stoi(tokens[5]);
    newRecord.housingNumber = (unsigned int)std::stoi(tokens[6]);
    newRecord.apartamentNumber = (unsigned int)std::stoi(tokens[7]);

    return newRecord;
}

VOID CreateIndex(std::vector<PhonebookRecord*> phoneBook)
{
    telephoneIndex = new Index<wchar_t[20]>(phoneBook, offsetof(PhonebookRecord, telephone));
    lastNameIndex = new Index<wchar_t[20]>(phoneBook, offsetof(PhonebookRecord, lastName));
    firstNameIndex = new Index<wchar_t[20]>(phoneBook, offsetof(PhonebookRecord, firstName));
    patronymicIndex = new Index<wchar_t[20]>(phoneBook, offsetof(PhonebookRecord, patronymic));
    streetIndex = new Index<wchar_t[20]>(phoneBook, offsetof(PhonebookRecord, streetName));
    houseIndex = new Index<unsigned int>(phoneBook, offsetof(PhonebookRecord, houseNumber));
    housingIndex = new Index<unsigned int>(phoneBook, offsetof(PhonebookRecord, housingNumber));
    apartamentIndex = new Index<unsigned int>(phoneBook, offsetof(PhonebookRecord, apartamentNumber));
}

std::vector<PhonebookRecord*> Intersection(std::vector<PhonebookRecord*> first, std::vector<PhonebookRecord*> second)
{
    std::vector<PhonebookRecord*> result;

    if (!first.empty()) {
        std::sort(first.begin(), first.end());
        std::sort(second.begin(), second.end());

        std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), back_inserter(result));
    }
    else {
        result = second;
    }

    return result;
}
