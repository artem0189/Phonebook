#pragma once

#include <vector>

#ifdef PHONEBOOKCORE_EXPORTS
#define PHONEBOOKCORE_API __declspec(dllexport)
#else
#define PHONEBOOKCORE_API __declspec(dllimport)
#endif

const std::vector<const wchar_t*> columnsName{ L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8" };

struct PhonebookRecord
{
	const wchar_t* telephone;
	const wchar_t* lastName;
	const wchar_t* firstName;
	const wchar_t* patronymic;
	const wchar_t* streetName;
	unsigned int houseNumber;
	unsigned int housingNumber;
	unsigned int apartamentNumber;
};

#ifdef __cplusplus   
extern "C" {
#endif

	PHONEBOOKCORE_API void __cdecl GetPhonebook();

	PHONEBOOKCORE_API void __cdecl Search(PhonebookRecord searchParam);

#ifdef __cplusplus
}
#endif