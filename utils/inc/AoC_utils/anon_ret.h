#pragma once

#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/cat.hpp>

#include <utility>

#define AOC_DETAILS_FOREACH_VAARG(MACRO, ...) \
    BOOST_PP_SEQ_FOR_EACH(MACRO, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__) )

#define AOC_DETAILS_FOREACH_I_VAARG(MACRO, ...) \
    BOOST_PP_SEQ_FOR_EACH_I(MACRO, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__) )

#define AOC_DETAILS_ANON_RET_EXPAND_TYPES(r, data, elem) \
    using BOOST_PP_CAT( elem, _t ) = decltype(elem);

#define AOC_DETAILS_ANON_RET_EXPAND_MEMBERS(r, data, elem) \
    BOOST_PP_CAT( elem, _t ) elem;

#define AOC_DETAILS_ANON_RET_EXPAND_MOVE(r, data, i, elem) \
    BOOST_PP_COMMA_IF(i) std::move( elem )

#define AOC_RETURN_ANONYMOUS_STRUCT(...)                          \
    AOC_DETAILS_FOREACH_VAARG(AOC_DETAILS_ANON_RET_EXPAND_TYPES, __VA_ARGS__)     \
    struct {                                                  \
      AOC_DETAILS_FOREACH_VAARG(AOC_DETAILS_ANON_RET_EXPAND_MEMBERS, __VA_ARGS__) \
    } aoc_anonymous_result = {                                \
      AOC_DETAILS_FOREACH_I_VAARG(AOC_DETAILS_ANON_RET_EXPAND_MOVE, __VA_ARGS__)  \
    };                                                        \
    return aoc_anonymous_result;
