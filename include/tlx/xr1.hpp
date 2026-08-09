//
// Created by muham on 24.07.2026.
//

#ifndef TLX_XR1_HPP
#define TLX_XR1_HPP

namespace tlx {
    /**
     * @brief Removes reference qualifiers from a type.
     *
     * This struct provides the member typedef `type` which is the same as `Ty`
     * with any reference qualifiers (`&` or `&&`) removed.
     *
     * @tparam Ty The type to remove references from.
     */
    template<class Ty>
    struct RemoveReference {
        using type = Ty;
    };
    template<class Ty>
    struct RemoveReference<Ty&> {
        using type = Ty;
    };
    template<class Ty>
    struct RemoveReference<Ty&&> {
        using type = Ty;
    };
    template<class Ty>
    using remove_reference_t = RemoveReference<Ty>::type;

    /**
     * @brief Extracts the underlying type of enumeration.
     *
     * This trait provides the underlying integral type of scoped or unscoped
     * enumeration. It is a no-op (empty) for non-enum types.
     *
     * @tparam Ty The type to extract the underlying type from.
     */
    template<class Ty, bool = __is_enum(Ty)>
    struct UnderlyingType {
        using type = __underlying_type(Ty);
    };
    template<class Ty>
    struct UnderlyingType<Ty, false> {};
    template<class Ty>
    using underlying_type_t = UnderlyingType<Ty>::type;

    /**
     * @brief Compile-time check whether a type is a reference (lvalue or rvalue).
     */
    template<class>
    constexpr bool is_reference = false;
    template<class Ty>
    constexpr bool is_reference<Ty&> = true;
    template<class Ty>
    constexpr bool is_reference<Ty&&> = true;

    template<typename T>
    struct restrict {
        using type = T* __restrict;
    };

    template<typename T>
    using restrict_t = restrict<T>::type;
} //namespace tlx

#endif //TLX_XR1_HPP