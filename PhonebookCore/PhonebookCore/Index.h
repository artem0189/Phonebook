#pragma once

#include "Phonebook.h"
#include <sstream>

#define CMP_EQ(a, b) ((a) == (b))
#define CMP_LT(a, b) ((a) < (b))
#define CMP_GT(a, b) ((a) > (b))

struct Node 
{
    std::vector<PhonebookRecord*> data;
    Node* left;
    Node* right;
    Node* parent;
    Node(PhonebookRecord* value, Node* parent)
    {
        this->left = this->right = NULL;
        this->data.push_back(value);
        this->parent = parent;
    }
};

template <typename T>
std::wstring ConvertToWstring(T value)
{
    std::wostringstream wss;
    wss << value;
    return wss.str();
}

template <typename T>
class Index
{
public:
    Node* root = NULL;
	Index(std::vector<PhonebookRecord*> phoneBook, ::size_t offset)
    {
        this->offset = offset;
        for (int i = 0; i < phoneBook.size(); i++) {
            Insert(phoneBook[i]);
        }
    }
    void Insert(PhonebookRecord* value)
    {
        Node* tmp = NULL;
        if (root == NULL) {
            root = new Node(value, NULL);
            return;
        }

        tmp = root;
        std::wstring first = ConvertToWstring(*(T*)((::size_t)value + offset));
        std::wstring second = ConvertToWstring(*(T*)((::size_t)tmp->data[0] + offset));
        while (tmp) {
            if (CMP_GT(first, second)) {
                if (tmp->right) {
                    tmp = tmp->right;
                    continue;
                }
                else {
                    tmp->right = new Node(value, tmp);
                    return;
                }
            }
            else if (CMP_LT(first, second)) {
                if (tmp->left) {
                    tmp = tmp->left;
                    continue;
                }
                else {
                    tmp->left = new Node(value, tmp);
                    return;
                }
            }
            else if (CMP_EQ(first, second)) {
                tmp->data.push_back(value);
                return;
            }
            else {
                exit(2);
            }
        }
    }
	std::vector<PhonebookRecord*> Search(Node* root, T value)
    {
        std::wstring first = ConvertToWstring(*(T*)((::size_t)root->data[0] + offset));
        std::wstring second = ConvertToWstring(value);
        while (root) {
            if (CMP_GT(first, second)) {
                root = root->left;
                continue;
            }
            else if (CMP_LT(first, second)) {
                root = root->right;
                continue;
            }
            else {
                return root->data;
            }
        }
        return {};
    }
private:
    ::size_t offset;
};