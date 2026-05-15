#ifndef COUNTER_HPP
#define COUNTER_HPP

#include <cstddef>

template <typename Derived>
class counter {
private:
    inline static size_t m_count = 0;

public:
    counter() {
        ++m_count;
    }

    counter(const counter&) {
        ++m_count;
    }

    counter(counter&&) noexcept {
        ++m_count;
    }

    ~counter() {
        --m_count;
    }

    counter& operator=(const counter&) = default;
    counter& operator=(counter&&) = default;

    static size_t count() {
        return m_count;
    }

protected:
    counter& operator=(const counter& other) const = delete;
};

#endif