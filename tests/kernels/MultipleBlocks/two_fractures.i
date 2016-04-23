[Mesh]
  type = FileMesh
  file = two_fractures.e
[]

[Variables]
  [./pressure]
    block = 'matrix fractures'
  [../]
[]

[UserObjects]
  [./DensityConstBulk]
    type = RichardsDensityConstBulk
    dens0 = 1.0
    bulk_mod = 10000000
  [../]
[]

[Kernels]
  [./klinkenberg_matrix]
    type = CompressibleKlinkenberg
    variable = pressure
    gravity_vector = '0 0 0'
    fluid_viscosity = 1.0
    klinkenberg_factor = 0.0
    gas_density_userobject = DensityConstBulk
    block = matrix
  [../]
  [./klinkenberg_fracture]
    type = CompressibleKlinkenberg
    variable = pressure
    gravity_vector = '0 0 0'
    fluid_viscosity = 1.0
    klinkenberg_factor = 0.0
    gas_density_userobject = DensityConstBulk
    block = fractures
  [../]
[]

[BCs]
  [./inner]
    variable = pressure
    type = DirichletBC
    boundary = inner
    value = 10
  [../]
  [./outer]
    variable = pressure
    type = DirichletBC
    boundary = outer
    value = 100
  [../]
[]

[Materials]
  [./matrix]
    type = DarcyMaterial
    block = matrix
    mat_permeability = '1 0 0  0 1 0  0 0 1'
  [../]
  [./fracture]
    type = DarcyMaterial
    block = fractures
    mat_permeability = '10000 0 0  0 10000 0  0 0 10000'
  [../]
[]

[Preconditioning]
  [./myprecond]
    type = SMP
    off_diag_row = pressure
    off_diag_column = pressure
    solve_type = PJFNK
  [../]
[]

[Executioner]
  # Preconditioned JFNK (default)
  type = Steady
  l_max_its = 20000
  solve_type = JFNK
  nl_abs_tol = 1e-6
  nl_rel_step_tol = 1e-8
  nl_abs_step_tol = 1e-6
[]

[Outputs]
  file_base = out
  exodus = true
  xda = true
[]

[ICs]
  [./constant]
    min = 10
    max = 100
    variable = pressure
    type = RandomIC
    block = 'matrix fractures'
  [../]
[]

