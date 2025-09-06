#ifndef ISORTER_HPP
#define ISORTER_HPP

#include <vector>

template <typename T>
class ISorter {
public:
    virtual ~ISorter() = default;
    virtual void sort(std::vector<T>& data) = 0;
};

#endif // ISORTER_HPP
