
[Mesh]
  type = FileMesh
  file = one_fracture.e
[]

[Variables]
  [./pressure]
    [./InitialCondition]
      type = RandomIC
      min = 0
      max = 1
    [../]
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
    klinkenberg_factor=0.0
    gas_density_userobject = DensityConstBulk
    block = 1
  [../]

  [./klinkenberg_fracture]
    type = CompressibleKlinkenberg
    variable = pressure
    gravity_vector = '0 0 0'
    fluid_viscosity = 1.0
    klinkenberg_factor=0.0
    gas_density_userobject = DensityConstBulk
    block = 2
  [../]  
[]

[BCs]
  [./inner]
    variable = pressure
    type = DirichletBC
    boundary = 'inner'
    value = 10
  [../]
  [./outer]
    variable = pressure
    type = DirichletBC
    boundary = 'outer'
    value = 100
  [../]
[]

[Materials]
  [./DarcyBlock_left]
    type = DarcyMaterial
    block = 1
    mat_permeability = '1 0 0  0 1 0  0 0 1'
  [../]
[]

[Materials]
  [./DarcyBlock_right]
    type = DarcyMaterial
    block = 2
    mat_permeability = '10000 0 0  0 10000 0  0 0 10000'
  [../]
[]


[Executioner]
  # Preconditioned JFNK (default)
  type = Steady
  solve_type = PJFNK
[]

[Outputs]
  file_base = out
  exodus = true
  xda = true
[]

[Adaptivity]
  marker = errorfrac
  steps = 4

  [./Indicators]
    [./error]
      type = GradientJumpIndicator
      variable = pressure
    [../]
  [../]

  [./Markers]
    [./errorfrac]
      type = ErrorFractionMarker
      refine = 0.9
      coarsen = 0.1
      indicator = error
    [../]
  [../]
[]