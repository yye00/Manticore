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
  distribution = serial # This uses SolutionUserObject which doesn't work with ParallelMesh.
[]

[Variables]
  # x-velocity
  [./u]
    order = SECOND
    family = LAGRANGE

    [./InitialCondition]
      type = ConstantIC
      value = 0.0
    [../]
  [../]

  # y-velocity
  [./v]
    order = SECOND
    family = LAGRANGE

    [./InitialCondition]
      type = ConstantIC
      value = 0.0
    [../]
  [../]
  
  # Pressure
  [./p]
    order = FIRST
    family = LAGRANGE

    [./InitialCondition]
      type = ConstantIC
      value = 0 # This number is arbitrary for NS...
    [../]
  [../]
[]

[Kernels]
  [./ForchheimerMomentum]
    type = ForchheimerMomentum
    klinkenberg_factor=0.25
  [../]

  [./ForchheimerMass]
    type = ForchheimerMass
    klinkenberg_factor=0.25
  [../]

  [./forcing]
    type = UserForcingFunction
    function = '2*(0.25)+2*exp(-x*x)-4*x*x*exp(-x*x)'
    # Any object expecting a function name can also receive a ParsedFunction string
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    boundary = 'left'
    value = 1
  [../]
  [./right]
    type = DirichletBC
    boundary = 'right'
    value = 0.36787944117
  [../]
[]

[Materials]
  [./DarcyBlock]
    type = DarcyMaterial
    block = 0
    mat_permeability = '1 0 0  0 1 0  0 0 1'
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

