#pragma once

#include "ATen/Tensor.h"
#include "ATen/Scalar.h"
#include "ATen/SparseTensorRef.h"

namespace at {

inline Tensor Tensor::toType(const Type & t) const {
  if(type().ID() ==t.ID())
    return *this;
  return t.copy(*this);
}

inline Tensor & Tensor::copy_(const Tensor & src) {
  resize_(src.sizes());
  type().copy(src,*this);
  return *this;
}

inline Tensor Tensor::toType(ScalarType t) const {
  return toType(type().toScalarType(t));
}

inline Tensor Tensor::toBackend(Backend b) const {
  return toType(type().toBackend(b));
}


// all static inline to allow for inlining of the non-dynamic part of dispatch
${tensor_method_definitions}

template<typename T>
inline T* Tensor::data() const {
  runtime_error("data() cast to unexpected type.");
}
#define DEFINE_CAST(T,name,_) \
template<> \
inline T* Tensor::data() const { \
  AT_ASSERT(type().scalarType() == ScalarType::name, \
    "expected scalar type % s but found %s", #name, \
    at::toString(type().scalarType())); \
  return static_cast<T*>(this->data_ptr()); \
} \
inline T* Tensor::to##name##Data() const { return data<T>(); }

AT_FORALL_SCALAR_TYPES(DEFINE_CAST)
#undef DEFINE_CAST

} //namespace at
