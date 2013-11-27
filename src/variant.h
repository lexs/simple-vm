#ifndef VARIANT_H
#define VARIANT_H

#include <cstdint>
#include <cassert>

#include <new>
#include <type_traits>
#include <utility>

#include "mpl.h"

template <typename... Types>
class Variant {
private:
    using Which = int;
    using First = typename mpl::first<Types...>::value;

    template <Which which, typename... TT>
    struct Initializer;

    template <typename R, typename T, typename Variant, typename Visitor>
    static R caller(const Variant& variant, Visitor& visitor) {
        return visitor(variant.template get<T>());
    }
public:
    static_assert(!mpl::any_of(std::is_void<Types>()...), "void type is not allowed");
    static_assert(!mpl::any_of(std::is_reference<Types>()...), "reference types are not allowed");
    static_assert(mpl::all_of(mpl::is_move_or_copy_constructible<Types>()...), "unmovable and uncopyable types are not allowed");
    static_assert(!mpl::has_duplicates<Types...>(), "duplicate types are not allowed");

    Variant() {
        construct(0, First());
    }

    template <typename T,
        typename = typename std::enable_if<!std::is_same<Variant, typename std::remove_reference<T>::type>::value>::type>
    Variant(T&& value) {
        Initializer<0, Types...>::initialize(*this, std::forward<T>(value));
    }

    Variant(const Variant& variant) {
        variant.visit(Constructor(*this, variant));
    }

    Variant(Variant&& variant) {
        variant.visit(MoveConstructor(*this, variant));
    }

    ~Variant() {
        destroy();
    }

    Variant& operator=(const Variant& variant) {
        if (this != &variant) {
            variant.visit(Assigner(*this, variant));
        }

        return *this;
    }

    Variant& operator=(Variant&& variant) {
        if (this != &variant) {
            variant.visit(MoveAssigner(*this, variant));
        }

        return *this;
    }

    int which_type() const {
        return which;
    }

    template <typename T> const T& get() const {
        static_assert(mpl::contains<T, Types...>(), "Type is not part of variant");
        return reinterpret_cast<const T&>(storage);
    }

    template <typename T> T& get() {
        static_assert(mpl::contains<T, Types...>(), "Type is not part of variant");
        return reinterpret_cast<T&>(storage);
    }

    template <typename Visitor> auto visit(Visitor&& visitor) const -> decltype(visitor(get<First>())) {
        using R = decltype(visit(visitor));
        static_assert(mpl::all_of(std::is_same<typename std::result_of<Visitor(Types&)>::type, R>()...),
            "all visitor methods must have the same return type");
        typedef R (*Callback)(const Variant& variant, Visitor& visitor);

        static Callback callers[] {
            &caller<R, Types, Variant, Visitor>...
        };

        assert(which >= 0 && which < COUNT);
        return callers[which](*this, visitor);
    }
private:
    static constexpr std::size_t SIZE = mpl::max(sizeof(Types)...);
    static constexpr std::size_t ALIGN = mpl::max(alignof(Types)...);
    static constexpr std::size_t COUNT = sizeof...(Types);

    using AlignedStorage = typename std::aligned_storage<SIZE, ALIGN>::type;
    Which which;
    AlignedStorage storage; //alignas(ALIGN) char storage[SIZE];

    template <typename T>
    void construct(Which which, T&& t) {
        typedef typename std::remove_reference<T>::type type;
        this->which = which;
        new(&storage) type(std::forward<T>(t));
    }

    void destroy() {
        if (which != -1) {
            visit(Destroyer());
        }
    }

    template <Which which, typename T, typename... TT>
    struct Initializer<which, T, TT...> : public Initializer<which + 1, TT...> {
        using Initializer<which + 1, TT...>::initialize;

        static void initialize(Variant& v, T&& value) {
            v.construct(which, std::forward<T>(value));
        }

        static void initialize(Variant& v, const T& value) {
            v.construct(which, value);
        }
    };


    template <Which which> struct Initializer<which> {
        static void initialize();
    };

    struct Constructor {
        Constructor(Variant& self, const Variant& other) : self(self), other(other) {}

        template <typename T> void operator()(const T& value) const {
            self.construct(other.which, value);
        }
    private:
        Variant& self;
        const Variant& other;
    };


    struct MoveConstructor {
        MoveConstructor(Variant& self, const Variant& other) : self(self), other(other) {}

        template <typename T> void operator()(T& value) const {
            self.construct(other.which, std::move(value));
        }
    private:
        Variant& self;
        const Variant& other;
    };

    struct Assigner {
        Assigner(Variant& self, const Variant& other) : self(self), other(other) {}

        template <typename T> void operator()(const T& value) const {
            if (self.which == other.which) {
                self.get<T>() = value;
            } else {
                self.destroy();
                self.construct(other.which, value);
            }
        }
    private:
        Variant& self;
        const Variant& other;
    };

    struct MoveAssigner {
        MoveAssigner(Variant& self, const Variant& other) : self(self), other(other) {}

        template <typename T> void operator()(T& value) const {
            if (self.which == other.which) {
                self.get<typename std::remove_const<T>::type>() = std::move(value);
            } else {
                self.destroy();
                self.construct(other.which, std::move(value));
            }
        }
    private:
        Variant& self;
        const Variant& other;
    };

    struct Destroyer {
        template <typename T> void operator()(T& value) const {
            value.~T();
        }
    };
};

#endif
