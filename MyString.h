#pragma once
#include <iostream>

//credit Angeld55 github

namespace StringHelperFunctions
{
    unsigned int getNextPowerOfTwo(unsigned int n);
}

class MyString
{
private:
    char* data = nullptr;
    size_t length = 0;
    size_t capacity = 15;

    void resize(size_t lengthToFit);
public:

    MyString();
	explicit MyString(size_t capacity);
    MyString(const char* str);
    MyString(const MyString& other);
    MyString& operator=(const MyString& other);
    ~MyString();

    size_t getLength() const;
    size_t getCapacity() const;
    const char* c_str() const;

    MyString& operator+=(const MyString& other);
    MyString& operator+=(char ch);

    char& operator[](size_t index);
    char operator[](size_t index) const;

    // this is not in the standard
    operator bool() const;

    friend MyString operator+(const MyString& lhs, const MyString& rhs);
    friend std::istream& operator>>(std::istream& is, MyString& str);

private:
    void copyFrom(const MyString& other);
    void free();
};

std::ostream& operator<<(std::ostream& os, const MyString& str);

bool operator<(const MyString& lhs, const MyString& rhs);
bool operator<=(const MyString& lhs, const MyString& rhs);
bool operator>=(const MyString& lhs, const MyString& rhs);
bool operator>(const MyString& lhs, const MyString& rhs);
bool operator==(const MyString& lhs, const MyString& rhs);
bool operator!=(const MyString& lhs, const MyString& rhs);