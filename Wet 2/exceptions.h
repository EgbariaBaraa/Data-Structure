//
// Created by Shadi Diab on 27/12/2021.
//

#ifndef WET2_EXCEPTIONS_H
#define WET2_EXCEPTIONS_H

#include <stdexcept>


class AlreadyExist :public std::exception{};

class DoNotExist :public std::exception{};

class InvalidInput :public std::exception{};

class AllocatonError :public std::exception{};


#endif //WET2_EXCEPTIONS_H
