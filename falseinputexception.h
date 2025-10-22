#ifndef FALSEINPUTEXCEPRION_H
#define FALSEINPUTEXCEPRION_H
#include <exception>
#include <string>

class FalseInputException: public std::exception{
    private:
    std::string message;
    public:
    FalseInputException(std::string message){this->message=message;};
    inline const char *what()const noexcept override{return this->message.c_str();};
};
#endif