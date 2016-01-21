[GlobalParams]
  variable = pressure
[]


[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
  xmin = 0.0
  xmax = 1.0
  ymin = 0.0
  ymax = 1.0
  distribution = serial # This uses SolutionUserObject which doesn't work with ParallelMesh.
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
    dens0 = 0.7
    bulk_mod = 1.4
  [../]
[]

[Kernels]
  [./klinkenberg]
    type = CompressibleKlinkenberg
    variable = pressure
    gravity_vector = '0 0 0'
    fluid_viscosity = 1
    klinkenberg_factor=0.25
    gas_density_userobject = DensityConstBulk
  [../]

  [./forcing]
    type = UserForcingFunction
    #function = '2*(0.25)+2*exp(-x*x)-4*x*x*exp(-x*x)'
    # the function from pressure
    function ='-2.8*(0.714285714285714*x*exp(-x*x)+x)*x*(0.25*exp(x*x)+1)*exp(-x*x+0.714285714285714*exp(-x*x))+0.70*x*x*exp(0.714285714285714*exp(-x*x))+1.4*(0.25*exp(x*x)+1)*exp(-x*x+0.714285714285714*exp(-x*x))'
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

