
[Mesh]
  type = FileMesh
  file = inner_1_outer_10_3D.e
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
  [./klinkenberg]
    type = CompressibleKlinkenberg
    variable = pressure
    gravity_vector = '0 0 0'
    fluid_viscosity = 0.8
    klinkenberg_factor=1.0
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
  [./DarcyBlock]
    type = DarcyMaterial
    block = 2
    mat_permeability = '1.5 0 0  0 1.5 0  0 0 1.5'
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
  steps = 2

  [./Indicators]
    [./error]
      type = GradientJumpIndicator
      variable = pressure
    [../]
  [../]

  [./Markers]
    [./errorfrac]
      type = ErrorFractionMarker
      refine = 0.5
      coarsen = 1
      indicator = error
    [../]
  [../]
[]