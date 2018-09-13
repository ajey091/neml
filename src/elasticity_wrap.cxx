#include "pyhelp.h" // include first to avoid annoying redef warning

#include "elasticity.h"

#include "nemlerror.h"

#include "pybind11/pybind11.h"
#include "pybind11/numpy.h"
#include "pybind11/stl.h"

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

namespace neml {

PYBIND11_MODULE(elasticity, m) {
  m.doc() = "Elastic models.";

  py::class_<Modulus, NEMLObject, std::shared_ptr<Modulus>>(m, "Modulus")
      .def(py::init<double>(), py::arg("x"))
      .def(py::init<std::shared_ptr<Interpolate>>(), py::arg("x"))
      .def("modulus", &Modulus::modulus, "Modulus as a function of temperature.")
      ;

  py::class_<ShearModulus, Modulus, std::shared_ptr<ShearModulus>>(m, "ShearModulus")
      .def(py::init<double>(), py::arg("mu"))
      .def(py::init<std::shared_ptr<Interpolate>>(), py::arg("mu"))
      ;

  py::class_<BulkModulus, Modulus, std::shared_ptr<BulkModulus>>(m, "BulkModulus")
      .def(py::init<double>(), py::arg("K"))
      .def(py::init<std::shared_ptr<Interpolate>>(), py::arg("K"))
      ;

  py::class_<YoungsModulus, Modulus, std::shared_ptr<YoungsModulus>>(m, "YoungsModulus")
      .def(py::init<double>(), py::arg("E"))
      .def(py::init<std::shared_ptr<Interpolate>>(), py::arg("E"))
      ;

  py::class_<PoissonsRatio, Modulus, std::shared_ptr<PoissonsRatio>>(m, "PoissonsRatio")
      .def(py::init<double>(), py::arg("nu"))
      .def(py::init<std::shared_ptr<Interpolate>>(), py::arg("nu"))
      ;

  py::class_<LinearElasticModel, NEMLObject, std::shared_ptr<LinearElasticModel>>(m, "LinearElasticModel")
      .def("C",
           [](const LinearElasticModel & m, double T) -> py::array_t<double>
           {
            auto C = alloc_mat<double>(6,6);
            int ier = m.C(T, arr2ptr<double>(C));
            py_error(ier);
            return C;
           }, "Return stiffness elasticity matrix.")

      .def("S",
           [](const LinearElasticModel & m, double T) -> py::array_t<double>
           {
            auto S = alloc_mat<double>(6,6);
            int ier = m.S(T, arr2ptr<double>(S));
            py_error(ier);
            return S;
           }, "Return compliance elasticity matrix.")
      .def("E", &LinearElasticModel::E, "Young's modulus as a function of temperature.")
      .def("nu", &LinearElasticModel::nu, "Poisson's ratio as a function of temperature.")
      .def("G", &LinearElasticModel::G, "Shear modulus as a function of temperature.")
      .def("K", &LinearElasticModel::K, "Bulk modulus as a function of temperature.")
      .def_property_readonly("valid", &LinearElasticModel::valid, "Good or dummy model.")
      ;

  py::class_<IsotropicLinearElasticModel, LinearElasticModel, std::shared_ptr<IsotropicLinearElasticModel>>(m, "IsotropicLinearElasticModel")
      .def(py::init<std::shared_ptr<ShearModulus>, std::shared_ptr<BulkModulus>>(),
           py::arg("shear_modulus"), py::arg("bulk_modulus"))
      .def(py::init<std::shared_ptr<YoungsModulus>, std::shared_ptr<PoissonsRatio>>(),
           py::arg("youngs_modulus"), py::arg("poissons_ratio"))
      ;
  
  py::class_<BlankElasticModel, LinearElasticModel, std::shared_ptr<BlankElasticModel>>(m, "BlankElasticModel")
      .def(py::init<>())
      ;
}

}
