#pragma once

#include "Phonebook.h"

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
class Index
{
public:
    Node* root = NULL;
	Index(std::vector<PhonebookRecord>* phoneBook, ::size_t offset)
    {
        this->offset = offset;
        for (int i = 0; i < phoneBook->size(); i++) {
            Insert(&((*phoneBook)[i]));
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
        while (tmp) {
            if (CMP_GT(*(T*)((::size_t)value + offset), *(T*)((::size_t)tmp->data[0] + offset))) {
                if (tmp->right) {
                    tmp = tmp->right;
                    continue;
                }
                else {
                    tmp->right = new Node(value, tmp);
                    return;
                }
            }
            else if (CMP_LT(*(T*)((::size_t)value + offset), *(T*)((::size_t)tmp->data[0] + offset))) {
                if (tmp->left) {
                    tmp = tmp->left;
                    continue;
                }
                else {
                    tmp->left = new Node(value, tmp);
                    return;
                }
            }
            else if (CMP_EQ(*(T*)((::size_t)value + offset), *(T*)((::size_t)tmp->data[0] + offset))) {
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
        while (root) {
            if (CMP_GT(*(T*)((::size_t)root->data[0] + offset), value)) {
                root = root->left;
                continue;
            }
            else if (CMP_LT(*(T*)((::size_t)root->data[0] + offset), value)) {
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