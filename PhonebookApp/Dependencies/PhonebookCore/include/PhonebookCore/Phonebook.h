#pragma once

#include <string>
#include <vector>

#ifdef PHONEBOOKCORE_EXPORTS
#define PHONEBOOKCORE_API __declspec(dllexport)
#else
#define PHONEBOOKCORE_API __declspec(dllimport)
#endif

const std::vector<std::wstring> columnsName{ TEXT("Telephone"), TEXT("LastName"), TEXT("FirstName"), TEXT("Patronymic"), TEXT("Street"), TEXT("House"), TEXT("Housing"), TEXT("Apartament") };

struct PhonebookRecord
{
	std::wstring telephone;
	std::wstring lastName;
	std::wstring firstName;
	std::wstring patronymic;
	std::wstring streetName;
	unsigned int houseNumber;
	unsigned int housingNumber;
	unsigned int apartamentNumber;
};

PHONEBOOKCORE_API std::vector<PhonebookRecord> __cdecl GetPhonebook();

PHONEBOOKCORE_API std::vector<PhonebookRecord> __cdecl Search(PhonebookRecord searchParam);
