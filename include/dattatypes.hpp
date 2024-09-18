
#ifndef __DATTATYPES_BASE__HPP
#define __DATTATYPES_BASE__HPP

#include <type_traits>
#include <stdexcept>


namespace Dattatypes {

// Primary template for checking if a type is an instantiation of a template
template <typename, template <typename> class>
struct is_templated : std::false_type {};
// Specialization: True if the type is an instantiation of the given template
template <typename T, template <typename> class Template>
struct is_templated<Template<T>, Template> : std::true_type {};


} // namespace Dattatypes

#endif
