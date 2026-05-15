#ifndef LESS_THAN_COMPARABLE_HPP
#define LESS_THAN_COMPARABLE_HPP

template <typename Derived>
class less_than_comparable {
private:
    const Derived& derived() const {
        return static_cast<const Derived&>(*this);
    }

public:
    friend bool operator>(const Derived& a, const Derived& b) {
        return b < a;
    }

    friend bool operator<=(const Derived& a, const Derived& b) {
        return !(b < a);
    }

    friend bool operator>=(const Derived& a, const Derived& b) {
        return !(a < b);
    }

    friend bool operator==(const Derived& a, const Derived& b) {
        return !(a < b) && !(b < a);
    }

    friend bool operator!=(const Derived& a, const Derived& b) {
        return (a < b) || (b < a);
    }

protected:
    less_than_comparable() = default;
    ~less_than_comparable() = default;
};

#endif