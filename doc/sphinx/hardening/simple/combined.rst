Combined isotropic/kinematic hardening
======================================

Overview
--------

This object simply combines an isotropic hardening model and a kinematic
hardening model to produce a combined hardening model suitable for
yield surfaces that use combined isotropic and kinematic hardening.

All it does in concatenate the isotropic hardening variable :math:`Q` and 
the kinematic backstress :math:`\mathbf{X}` into the "stress-like" hardening
vector

.. math::
   \mathbf{q}=\left[\begin{array}{cc} Q & \mathbf{X}\end{array}\right]

and likewise concatenates the "strain-like" history variables into a 
vector :math:`\bm{\alpha}`.

Parameters
----------

.. csv-table::
   :header: "Parameter", "Object type", "Description", "Default"
   :widths: 12, 30, 50, 8

   ``iso``, :cpp:class:`neml::IsotropicHardeningRule`, Isotropic hardening rule, No
   ``kin``, :cpp:class:`neml::KinematicHardeningRule`, Kinematic hardening rule, No

Class description
-----------------

.. doxygenclass:: neml::CombinedHardeningRule
   :members:
   :undoc-members:
