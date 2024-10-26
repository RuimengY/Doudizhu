#include "Card.h"
#include <stdexcept>

Card::Card(const std::string &str) : strRepresentation(str)
{
}

int Card::getValue() const
{
    return value;
}

std::string Card::getString() const
{
    return strRepresentation;
}
