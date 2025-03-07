
#ifndef BOOST_MPL_ASSERT_HPP_INCLUDED
#define BOOST_MPL_ASSERT_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/boost/boost/boost/mpl/assert.hpp,v $
// $Date: 2004/09/15 15:29:45 $
// $Revision: 1.11 $

#include <boost/mpl/not.hpp>
#include <boost/mpl/aux_/value_wknd.hpp>
#include <boost/mpl/aux_/nested_type_wknd.hpp>
#include <boost/mpl/aux_/yes_no.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/aux_/adl_barrier.hpp>

#include <boost/mpl/aux_/config/nttp.hpp>
#include <boost/mpl/aux_/config/dtp.hpp>
#include <boost/mpl/aux_/config/gcc.hpp>
#include <boost/mpl/aux_/config/msvc.hpp>
#include <boost/mpl/aux_/config/workaround.hpp>

#include <boost/preprocessor/cat.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, >= 0x560) && BOOST_WORKAROUND(__BORLANDC__, < 0x600) \
    || (BOOST_MPL_CFG_GCC != 0)
#   define BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES
#endif

#if BOOST_WORKAROUND(__MWERKS__, < 0x3202) \
    || BOOST_WORKAROUND(__EDG_VERSION__, <= 238) \
    || BOOST_WORKAROUND(__BORLANDC__, < 0x600) \
    || BOOST_WORKAROUND(__DMC__, BOOST_TESTED_AT(0x840))
#   define BOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER
#endif


BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

struct failed {};

// agurt, 24/aug/04: MSVC 7.1 workaround here and below: return/accept 
// 'assert<false>' by reference; can't apply it unconditionally -- apparently it
// degrades quality of GCC diagnostics
#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
#   define AUX778076_ASSERT_ARG(x) x&
#else
#   define AUX778076_ASSERT_ARG(x) x
#endif

template< bool C >  struct assert        { typedef void* type; };
template<>          struct assert<false> { typedef AUX778076_ASSERT_ARG(assert) type; };

template< bool C >
int assertion_failed( typename assert<C>::type );

template <bool C>
struct assertion
{
    static int failed(assert<false>);
};

template <>
struct assertion<true>
{
    template <class T>
    static int failed(T);
};

struct assert_
{
#if !defined(BOOST_MPL_CFG_NO_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
    template< typename T1, typename T2 = na, typename T3 = na, typename T4 = na > struct types {};
#endif
    static assert_ const arg;
    enum relations { equal = 1, not_equal, greater, greater_equal, less, less_equal };
};


#if !defined(BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES)

bool operator==( failed, failed );
bool operator!=( failed, failed );
bool operator>( failed, failed );
bool operator>=( failed, failed );
bool operator<( failed, failed );
bool operator<=( failed, failed );

#if defined(__EDG_VERSION__)
template< bool (*)(failed, failed), long x, long y > struct assert_relation {};
#   define BOOST_MPL_AUX_ASSERT_RELATION(x, y, r) assert_relation<r,x,y>
#else
template< BOOST_MPL_AUX_NTTP_DECL(long, x), BOOST_MPL_AUX_NTTP_DECL(long, y), bool (*)(failed, failed) > 
struct assert_relation {};
#   define BOOST_MPL_AUX_ASSERT_RELATION(x, y, r) assert_relation<x,y,r>
#endif

#else // BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES

aux::weighted_tag<1>::type operator==( assert_, assert_ );
aux::weighted_tag<2>::type operator!=( assert_, assert_ );
aux::weighted_tag<3>::type operator>(  assert_, assert_ );
aux::weighted_tag<4>::type operator>=( assert_, assert_ );
aux::weighted_tag<5>::type operator<( assert_, assert_ );
aux::weighted_tag<6>::type operator<=( assert_, assert_ );

template< assert_::relations r, long x, long y > struct assert_relation {};

#endif 


#if !defined(BOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER)

template< bool > struct assert_arg_pred_impl { typedef int type; };
template<> struct assert_arg_pred_impl<true> { typedef void* type; };

template< typename P > struct assert_arg_pred
{
    typedef typename P::type p_type;
    typedef typename assert_arg_pred_impl< p_type::value >::type type;
};

template< typename P > struct assert_arg_pred_not
{
    typedef typename P::type p_type;
    enum { p = !p_type::value };
    typedef typename assert_arg_pred_impl<p>::type type;
};

template< typename Pred >
failed ************ (Pred::************ 
      assert_arg( void (*)(Pred), typename assert_arg_pred<Pred>::type )
    );

template< typename Pred >
failed ************ (boost::mpl::not_<Pred>::************ 
      assert_not_arg( void (*)(Pred), typename assert_arg_pred_not<Pred>::type )
    );

template< typename Pred >
AUX778076_ASSERT_ARG(assert<false>)
assert_arg( void (*)(Pred), typename assert_arg_pred_not<Pred>::type );

template< typename Pred >
AUX778076_ASSERT_ARG(assert<false>)
assert_not_arg( void (*)(Pred), typename assert_arg_pred<Pred>::type );


#else // BOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER
        
template< bool c, typename Pred > struct assert_arg_type_impl
{
    typedef failed      ************ Pred::* mwcw83_wknd;
    typedef mwcw83_wknd ************* type;
};

template< typename Pred > struct assert_arg_type_impl<true,Pred>
{
    typedef AUX778076_ASSERT_ARG(assert<false>) type;
};

template< typename Pred > struct assert_arg_type
    : assert_arg_type_impl< BOOST_MPL_AUX_VALUE_WKND(BOOST_MPL_AUX_NESTED_TYPE_WKND(Pred))::value, Pred >
{
};

template< typename Pred >
typename assert_arg_type<Pred>::type 
assert_arg(void (*)(Pred), int);

template< typename Pred >
typename assert_arg_type< boost::mpl::not_<Pred> >::type 
assert_not_arg(void (*)(Pred), int);

#   if !defined(BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES)
template< long x, long y, bool (*r)(failed, failed) >
typename assert_arg_type_impl< false,BOOST_MPL_AUX_ASSERT_RELATION(x,y,r) >::type
assert_rel_arg( BOOST_MPL_AUX_ASSERT_RELATION(x,y,r) );
#   else
template< assert_::relations r, long x, long y >
typename assert_arg_type_impl< false,assert_relation<r,x,y> >::type
assert_rel_arg( assert_relation<r,x,y> );
#   endif

#endif // BOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER

#undef AUX778076_ASSERT_ARG

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE


// BOOST_MPL_ASSERT((pred<x,...>))

#define BOOST_MPL_ASSERT(pred) \
enum { \
    BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
          boost::mpl::assertion_failed<false>( \
              boost::mpl::assert_arg( (void (*) pred)0, 1 ) \
            ) \
        ) \
}\
/**/

// BOOST_MPL_ASSERT_NOT((pred<x,...>))

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
#   define BOOST_MPL_ASSERT_NOT(pred) \
enum { \
    BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
          boost::mpl::assertion<false>::failed( \
              boost::mpl::assert_not_arg( (void (*) pred)0, 1 ) \
            ) \
        ) \
}\
/**/
#else
#   define BOOST_MPL_ASSERT_NOT(pred) \
enum { \
    BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
          boost::mpl::assertion_failed<false>( \
              boost::mpl::assert_not_arg( (void (*) pred)0, 1 ) \
            ) \
        ) \
}\
/**/
#endif

// BOOST_MPL_ASSERT_RELATION(x, ==|!=|<=|<|>=|>, y)

#if defined(BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES)

#   if !defined(BOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER)
#   define BOOST_MPL_ASSERT_RELATION(x, rel, y) \
enum { \
      BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
        boost::mpl::assertion_failed<(x rel y)>( \
            (boost::mpl::failed ************ ( boost::mpl::assert_relation< \
                  boost::mpl::assert_::relations( sizeof( \
                      boost::mpl::assert_::arg rel boost::mpl::assert_::arg \
                    ) ) \
                , x \
                , y \
                >::************)) 0 ) \
        ) \
} \
/**/
#   else
#   define BOOST_MPL_ASSERT_RELATION(x, rel, y) \
enum { \
      BOOST_PP_CAT(mpl_assert_rel,__LINE__) = sizeof(boost::mpl::assert_::arg rel boost::mpl::assert_::arg) \
    , BOOST_PP_CAT(mpl_assert_rel_value,__LINE__) = (x rel y) \
    , BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
        boost::mpl::assertion_failed<BOOST_PP_CAT(mpl_assert_rel_value,__LINE__)>( \
              boost::mpl::assert_rel_arg( boost::mpl::assert_relation< \
                  boost::mpl::assert_::relations(BOOST_PP_CAT(mpl_assert_rel,__LINE__)) \
                , x \
                , y \
                >() ) \
            ) \
        ) \
} \
/**/
#   endif

#else // !BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES

#   if defined(BOOST_MPL_CFG_ASSERT_BROKEN_POINTER_TO_POINTER_TO_MEMBER)
#   define BOOST_MPL_ASSERT_RELATION(x, rel, y) \
enum { \
    BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
        boost::mpl::assertion_failed<(x rel y)>( boost::mpl::assert_rel_arg( \
              boost::mpl::BOOST_MPL_AUX_ASSERT_RELATION(x,y,(&boost::mpl::operator rel))() \
            ) ) \
        ) \
}\
/**/
#   else
#   define BOOST_MPL_ASSERT_RELATION(x, rel, y) \
enum { \
    BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
        boost::mpl::assertion_failed<(x rel y)>( (boost::mpl::failed ************ ( \
            boost::mpl::BOOST_MPL_AUX_ASSERT_RELATION(x,y,(&boost::mpl::operator rel))::************))0 ) \
        ) \
}\
/**/
#   endif

#endif


// BOOST_MPL_ASSERT_MSG( (pred<x,...>::value), USER_PROVIDED_MESSAGE, (types<x,...>) ) 

#if BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3202))
#   define BOOST_MPL_ASSERT_MSG( c, msg, types_ ) \
    struct msg; \
    typedef struct BOOST_PP_CAT(msg,__LINE__) : boost::mpl::assert_ \
    { \
        using boost::mpl::assert_::types; \
        enum { msg }; \
        static boost::mpl::failed ************ (msg::************ assert_arg()) types_ \
        { return 0; } \
    } BOOST_PP_CAT(mpl_assert_arg,__LINE__); \
    enum { \
        BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
            boost::mpl::assertion<(c)>::failed( BOOST_PP_CAT(mpl_assert_arg,__LINE__)::assert_arg() ) \
            ) \
    }\
/**/
#else
#   define BOOST_MPL_ASSERT_MSG( c, msg, types_ ) \
    struct msg; \
    typedef struct BOOST_PP_CAT(msg,__LINE__) : boost::mpl::assert_ \
    { \
        static boost::mpl::failed ************ (msg::************ assert_arg()) types_ \
        { return 0; } \
    } BOOST_PP_CAT(mpl_assert_arg,__LINE__); \
    enum { \
        BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
            boost::mpl::assertion_failed<(c)>( BOOST_PP_CAT(mpl_assert_arg,__LINE__)::assert_arg() ) \
            ) \
    }\
/**/
#endif

#endif // BOOST_MPL_ASSERT_HPP_INCLUDED
