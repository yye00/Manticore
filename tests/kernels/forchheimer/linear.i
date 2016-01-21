[GlobalParams]
  gravity_vector = '0 0 0'
  fluid_viscosity = 1
[]


[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 25
  ny = 25
  xmin = 0.0
  xmax = 1.0
  ymin = 0.0
  ymax = 1.0
  elem_type = QUAD9
  distribution = serial # This uses SolutionUserObject which doesn't work with ParallelMesh.
[]

[UserObjects]
  [./DensityConstBulk]
    type = RichardsDensityConstBulk
    dens0 = 1.0
    bulk_mod = 1000000
  [../]
[]


[Variables]
  # x-velocity
  [./u]
    order = SECOND
    family = LAGRANGE

    [./InitialCondition]
      type = ConstantIC
      value = 1.0
    [../]
  [../]

  # y-velocity
  [./v]
    order = SECOND
    family = LAGRANGE

    [./InitialCondition]
      type = ConstantIC
      value = 1.0
    [../]
  [../]
  
  # Pressure
  [./p]
    order = FIRST
    family = LAGRANGE

    [./InitialCondition]
      type = ConstantIC
      value = 3.0
    [../]
  [../]
[]

[Kernels]

  # x forchheimer momentum, space
  [./x_ForchheimerMomentum]
    type = ForchheimerMomentum
    variable = u
    u = u
    v = v
    p = p
    component = 0
    gas_density_userobject = DensityConstBulk
    forchheimer_factor = 1
  [../]

  # y forchheimer momentum, space
  [./y_ForchheimerMomentum]
    type = ForchheimerMomentum
    variable = v
    u = u
    v = v
    p = p
    component = 1
    gas_density_userobject = DensityConstBulk
    forchheimer_factor = 1
  [../]  

  # mass
  [./ForchheimerMass]
    type = ForchheimerMass
    variable = p
    u = u
    v = v
    p = p
    gas_density_userobject = DensityConstBulk
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    boundary = 'left'
    value = 1
    variable = p
  [../]
  [./right]
    type = DirichletBC
    boundary = 'right'
    value = -29
    variable = p
  [../]
[]

[Materials]
  [./DarcyBlock]
    type = DarcyMaterial
    block = 0
    mat_permeability = '1 0 0  0 1 0  0 0 1'
  [../]
[]

[Preconditioning]
  [./SMP_PJFNK]
    type = SMP
    full = true

    # Preconditioned JFNK (default)
    solve_type = 'PJFNK'
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

