#include "types.h"

std::string ListNode::toString() const
{
    std::string result = "(";
    for (const ASTNodePtr &elem : _elements)
    {
        result += elem->toString();
        // Add space if not the last element
        if (&elem != &_elements.back())
            result += " ";
    }
    result += ")";
    return result;
}

std::string AtomNode::toString() const
{
    return _token.image;
}
