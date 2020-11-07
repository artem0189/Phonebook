// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "Phonebook.h"
#include "Index.h"

static std::vector<PhonebookRecord> _phoneBook;
Index<std::wstring> *telephoneIndex, *lastNameIndex, *firstNameIndex, *patronymicIndex, *streetIndex;
Index<unsigned int> *houseIndex, *housingIndex, *apartamentIndex;

VOID CreateIndex(std::vector<PhonebookRecord>* phoneBook);
VOID ReadFromFile(std::wstring filePath, std::vector<PhonebookRecord>* result);
PhonebookRecord ParseLine(std::wstring line);
VOID AddVector(std::vector<PhonebookRecord*>* first, std::vector<PhonebookRecord*> second);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        TCHAR currentDirectory[256];
        GetCurrentDirectory(sizeof(currentDirectory), currentDirectory);
        //ReadFromFile((std::wstring)currentDirectory + TEXT("\\db.txt"), &_phoneBook);
        ReadFromFile(TEXT("C:/Users/USER/Desktop/лб/осисп/4/Phonebook/PhonebookApp/PhonebookApp/db.txt"), &_phoneBook);
        CreateIndex(&_phoneBook);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

VOID CreateIndex(std::vector<PhonebookRecord>* phoneBook)
{
    telephoneIndex = new Index<std::wstring>(phoneBook, offsetof(PhonebookRecord, telephone));
    lastNameIndex = new Index<std::wstring>(phoneBook, offsetof(PhonebookRecord, lastName));
    firstNameIndex = new Index<std::wstring>(phoneBook, offsetof(PhonebookRecord, firstName));
    patronymicIndex = new Index<std::wstring>(phoneBook, offsetof(PhonebookRecord, patronymic));
    streetIndex = new Index<std::wstring>(phoneBook, offsetof(PhonebookRecord, streetName));
    houseIndex = new Index<unsigned int>(phoneBook, offsetof(PhonebookRecord, houseNumber));
    housingIndex = new Index<unsigned int>(phoneBook, offsetof(PhonebookRecord, housingNumber));
    apartamentIndex = new Index<unsigned int>(phoneBook, offsetof(PhonebookRecord, apartamentNumber));
}

VOID ReadFromFile(std::wstring filePath, std::vector<PhonebookRecord>* result)
{
    std::wstring line;
    std::wifstream file(filePath);
    if (file.is_open()) {
        while (getline(file, line)) {
            result->push_back(ParseLine(line));
        }
    }
}

PhonebookRecord ParseLine(std::wstring line)
{
    std::vector<std::wstring> tokens;

    int pos = 0;
    std::wstring token;
    while ((pos = line.find(L"|")) != std::wstring::npos) {
        tokens.push_back((token = line.substr(0, pos)) != L"" ? token : NULL);
        line.erase(0, pos + 1);
    }
    return { tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], (unsigned int)std::stoi(tokens[5]), (unsigned int)std::stoi(tokens[6]), (unsigned int)std::stoi(tokens[7]) };
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

PHONEBOOKCORE_API std::vector<PhonebookRecord> __cdecl GetPhonebook()
{
    return _phoneBook;
}

PHONEBOOKCORE_API std::vector<PhonebookRecord> __cdecl Search(PhonebookRecord searchParam)
{
    bool isEmpty = true;
    std::vector<PhonebookRecord*> result;
    if (!(searchParam.telephone.empty())) {
        result = Intersection(result, telephoneIndex->Search(telephoneIndex->root, searchParam.telephone));
        isEmpty &= false;
    }
    if (!(searchParam.lastName.empty())) {
        result = Intersection(result, lastNameIndex->Search(lastNameIndex->root, searchParam.lastName));
        isEmpty &= false;
    }
    if (!(searchParam.firstName.empty())) {
        result = Intersection(result, firstNameIndex->Search(firstNameIndex->root, searchParam.firstName));
        isEmpty &= false;
    }
    if (!(searchParam.patronymic.empty())) {
        result = Intersection(result, patronymicIndex->Search(patronymicIndex->root, searchParam.patronymic));
        isEmpty &= false;
    }
    if (!(searchParam.streetName.empty())) {
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
        std::vector<PhonebookRecord> t;
        for (int i = 0; i < result.size(); i++) {
            t.push_back(*(result[i]));
        }
        return t;
    }
    else {
        return _phoneBook;
    }
}

