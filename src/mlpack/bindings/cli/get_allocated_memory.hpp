/**
 * @file bindings/cli/get_allocated_memory.hpp
 * @author Ryan Curtin
 *
 * If the parameter has a type that may need to be deleted, return the address
 * of that object.  Otherwise return NULL.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_BINDINGS_CMD_GET_ALLOCATED_MEMORY_HPP
#define MLPACK_BINDINGS_CMD_GET_ALLOCATED_MEMORY_HPP

#include <mlpack/core/util/param_data.hpp>

namespace mlpack {
namespace bindings {
namespace cmd {

template<typename T>
void* GetAllocatedMemory(
    util::ParamData& /* d */,
    const typename boost::disable_if<data::HasSerialize<T>>::type* = 0,
    const typename boost::disable_if<arma::is_arma_type<T>>::type* = 0)
{
  return NULL;
}

template<typename T>
void* GetAllocatedMemory(
    util::ParamData& /* d */,
    const typename boost::enable_if<arma::is_arma_type<T>>::type* = 0)
{
  return NULL;
}

template<typename T>
void* GetAllocatedMemory(
    util::ParamData& d,
    const typename boost::disable_if<arma::is_arma_type<T>>::type* = 0,
    const typename boost::enable_if<data::HasSerialize<T>>::type* = 0)
{
  // Here we have a model, which is a tuple, and we need the address of the
  // memory.
  typedef std::tuple<T*, std::string> TupleType;
  return std::get<0>(*boost::any_cast<TupleType>(&d.value));
}

template<typename T>
void GetAllocatedMemory(util::ParamData& d,
                        const void* /* input */,
                        void* output)
{
  *((void**) output) =
      GetAllocatedMemory<typename std::remove_pointer<T>::type>(d);
}

} // namespace cmd
} // namespace bindings
} // namespace mlpack

#endif
