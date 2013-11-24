#ifndef MPL_H
#define MPL_H

namespace mpl {

template <typename T> constexpr T max(T t, T u) {
    return t > u ? t : u;
}

template <typename T, typename... TT> constexpr T max(T t, TT... ts) {
    return max(t, max(ts...));
}

template <typename T>
constexpr int index_of() { return -1; }

template <typename T, typename Head, typename... Tail>
constexpr int index_of() {
    return std::is_same<T, Head>() ? 0 :
        index_of<T, Tail...>() == -1 ? -1 : 1 + index_of<T, Tail...>();
}

template <typename T, typename... Types>
constexpr bool contains() {
    return index_of<T, Types...>() != -1;
}

template <typename Head>
constexpr bool has_duplicates() {
    return false;
}

template <typename Head, typename Next, typename... Tail>
constexpr bool has_duplicates() {
    return contains<Head, Next, Tail...>() ? true : has_duplicates<Next, Tail...>();
}

template <typename T>
constexpr bool all_of(T t) {
    return t;
}

template <typename Head, typename... Tail>
constexpr bool all_of(Head head, Tail... tail) {
    return head && all_of<Tail...>(tail...);
}

template <typename T>
constexpr bool any_of(T t) {
    return t;
}

template <typename Head, typename... Tail>
constexpr bool any_of(Head head, Tail... tail) {
    return head || any_of<Tail...>(tail...);
}

template <typename Head, typename... Tail>
struct first {
    using value = Head;
};

template <class T>
constexpr bool is_move_or_copy_constructible() {
    return std::is_nothrow_copy_constructible<T>() || std::is_nothrow_move_constructible<T>();
}

}

#endif
