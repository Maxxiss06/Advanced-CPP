#include <iostream>
#include <type_traits>

namespace typelist {


template<typename T, typename U>
inline constexpr bool is_same_v = std::is_same<T, U>::value;


template<typename... Types>
struct TypeList {
    static constexpr size_t size = sizeof...(Types);
};


template<typename TL, size_t Index, size_t Current = 0>
struct AtHelper;

template<typename Head, typename... Tail, size_t Current>
struct AtHelper<TypeList<Head, Tail...>, Current, Current> {
    using type = Head;
};

template<typename Head, typename... Tail, size_t Index, size_t Current>
struct AtHelper<TypeList<Head, Tail...>, Index, Current> {
    using type = typename AtHelper<TypeList<Tail...>, Index, Current + 1>::type;
};

template<typename TL, size_t Index>
struct At {
    using type = typename AtHelper<TL, Index, 0>::type;
};


template<typename TL>
struct Size;

template<typename... Types>
struct Size<TypeList<Types...>> {
    static constexpr size_t value = sizeof...(Types);
};


template<typename TL, typename T>
struct Contains;

template<typename Head, typename... Tail, typename T>
struct Contains<TypeList<Head, Tail...>, T> {
    static constexpr bool value = is_same_v<Head, T> || Contains<TypeList<Tail...>, T>::value;
};

template<typename T>
struct Contains<TypeList<>, T> {
    static constexpr bool value = false;
};


template<typename TL, typename T, size_t Current = 0>
struct IndexOfHelper;

template<typename Head, typename... Tail, typename T, size_t Current>
struct IndexOfHelper<TypeList<Head, Tail...>, T, Current> {
    static constexpr size_t value = is_same_v<Head, T> 
        ? Current 
        : IndexOfHelper<TypeList<Tail...>, T, Current + 1>::value;
};

template<typename T, size_t Current>
struct IndexOfHelper<TypeList<>, T, Current> {
    static constexpr size_t value = static_cast<size_t>(-1);
};

template<typename TL, typename T>
struct IndexOf {
    static constexpr size_t value = IndexOfHelper<TL, T, 0>::value;
    static_assert(value != static_cast<size_t>(-1), "Type not found in TypeList");
};


template<typename TL, typename T>
struct PushBack;

template<typename... Types, typename T>
struct PushBack<TypeList<Types...>, T> {
    using type = TypeList<Types..., T>;
};


template<typename TL, typename T>
struct PushFront;

template<typename... Types, typename T>
struct PushFront<TypeList<Types...>, T> {
    using type = TypeList<T, Types...>;
};

}

// Тесты
using namespace typelist;

struct TypeA {};
struct TypeB {};
struct TypeC {};
struct TypeD {};

int main() {
    using List = TypeList<TypeA, TypeB, TypeC>;
    
    static_assert(Size<List>::value == 3, "Size failed");
    std::cout << "Size: " << Size<List>::value << std::endl;
    
    static_assert(std::is_same<At<List, 0>::type, TypeA>::value, "At<0> failed");
    static_assert(std::is_same<At<List, 1>::type, TypeB>::value, "At<1> failed");
    static_assert(std::is_same<At<List, 2>::type, TypeC>::value, "At<2> failed");
    std::cout << "At works" << std::endl;
    
    static_assert(Contains<List, TypeA>::value == true, "Contains positive failed");
    static_assert(Contains<List, TypeD>::value == false, "Contains negative failed");
    std::cout << "Contains works" << std::endl;
    
    static_assert(IndexOf<List, TypeA>::value == 0, "IndexOf A failed");
    static_assert(IndexOf<List, TypeB>::value == 1, "IndexOf B failed");
    static_assert(IndexOf<List, TypeC>::value == 2, "IndexOf C failed");
    std::cout << "IndexOf works" << std::endl;
    
    using ListBack = PushBack<List, TypeD>::type;
    static_assert(Size<ListBack>::value == 4, "PushBack size failed");
    static_assert(std::is_same<At<ListBack, 3>::type, TypeD>::value, "PushBack element failed");
    std::cout << "PushBack works" << std::endl;
    
    using ListFront = PushFront<List, TypeD>::type;
    static_assert(Size<ListFront>::value == 4, "PushFront size failed");
    static_assert(std::is_same<At<ListFront, 0>::type, TypeD>::value, "PushFront element failed");
    std::cout << "PushFront works" << std::endl;
    
    std::cout << "All done" << std::endl;
    return 0;
}