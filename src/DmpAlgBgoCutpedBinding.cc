#include <boost/python.hpp>
#include "DmpAlgBgoCutped.h"

BOOST_PYTHON_MODULE(libDmpAlgBgoCutped){
  using namespace boost::python;

  class_<DmpAlgBgoCutped,boost::noncopyable,bases<DmpVAlg> >("DmpAlgBgoCutped",init<>());
}
