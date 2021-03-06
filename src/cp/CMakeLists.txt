include(../macros.cmake)

target_sources(neml PRIVATE 
      ${CMAKE_CURRENT_SOURCE_DIR}/crystallography.cxx
      ${CMAKE_CURRENT_SOURCE_DIR}/inelasticity.cxx
      ${CMAKE_CURRENT_SOURCE_DIR}/sliprules.cxx
      ${CMAKE_CURRENT_SOURCE_DIR}/slipharden.cxx
      ${CMAKE_CURRENT_SOURCE_DIR}/kinematics.cxx
      ${CMAKE_CURRENT_SOURCE_DIR}/singlecrystal.cxx
      ${CMAKE_CURRENT_SOURCE_DIR}/batch.cxx
      ${CMAKE_CURRENT_SOURCE_DIR}/polycrystal.cxx
      )

if (WRAP_PYTHON)
      set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MODULE_BASE}/cp)

      if (WIN32)
            set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MODULE_BASE}/cp)
      endif()

      pybind(crystallography)
      pybind(inelasticity)
      pybind(sliprules)
      pybind(slipharden)
      pybind(kinematics)
      pybind(singlecrystal)
      pybind(batch)
      pybind(polycrystal)
endif()
