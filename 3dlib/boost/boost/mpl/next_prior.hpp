
#ifndef BOOST_MPL_NEXT_PRIOR_HPP_INCLUDED
#define BOOST_MPL_NEXT_PRIOR_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /cvsroot/boost/boost/boost/mpl/next_prior.hpp,v $
// $Date: 2004/09/02 15:40:41 $
// $Revision: 1.2 $

#include <boost/mpl/aux_/na_spec.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>

namespace boost { namespace mpl {

template<
      typename BOOST_MPL_AUX_NA_PARAM(T)
    >
struct next
{
    typedef typename T::next type;
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1,next,(T))
};

template<
      typename BOOST_MPL_AUX_NA_PARAM(T)
    >
struct prior
{
    typedef typename T::prior type;
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1,prior,(T))
};

BOOST_MPL_AUX_NA_SPEC(1, next)
BOOST_MPL_AUX_NA_SPEC(1, prior)

}}

#endif // BOOST_MPL_NEXT_PRIOR_HPP_INCLUDED
