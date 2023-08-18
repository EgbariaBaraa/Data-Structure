//
// Created by Shadi Diab on 03/12/2021.
//

#ifndef WET1_EXCEPTIONS_H
#define WET1_EXCEPTIONS_H
#include <stdexcept>

class AlreadyExist : public std::exception{};

class DoNotExist : public std::exception{};

class InvalidInput : public std::exception{};

#endif //WET1_EXCEPTIONS_H
