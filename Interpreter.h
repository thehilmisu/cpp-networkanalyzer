#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <cstddef>

template <typename T>
class Interpreter 
{
public:
    virtual T interpret(const unsigned char* packet, std::size_t length) = 0;
    virtual ~Interpreter() = default;
};

#endif // INTERPRETER_H
