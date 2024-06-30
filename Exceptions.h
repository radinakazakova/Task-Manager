#pragma once
#include <stdexcept>

class BusinessException : public std::logic_error {
public:
    BusinessException(const char* message) : std::logic_error(message) {}
};

class AuthException : public BusinessException {
public:
    AuthException(const char* message) : BusinessException(message) {}
};

class NotFoundException : public BusinessException {
public:
    NotFoundException(const char* message) : BusinessException(message) {}
};

class DuplicateException : public BusinessException {
public:
    DuplicateException(const char* message) : BusinessException(message) {}
};

class UnauthorizedException : public BusinessException {
public:
    UnauthorizedException(const char* message) : BusinessException(message) {}
};

class InvalidStateException : public BusinessException {
public:
    InvalidStateException(const char* message) : BusinessException(message) {}
};


class SystemException : public std::runtime_error {
public:
    SystemException(const char* message) : std::runtime_error(message) {}
};

class FileIOException : public SystemException {
public:
    FileIOException(const char* message) : SystemException(message) {}
};