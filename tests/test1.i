[GlobalParams]
  variable = pressure
  fluid_weight = '0 0 0'
  fluid_viscosity = 1
[]


[Mesh]
  # file = mug.e
  type = FileMesh
  file = ThreeBlocksLeftCenterRIght_SS_1Top_2Bottom.e
[]

[Variables]
  [./pressure]
    [./InitialCondition]
      type = RandomIC
      block = 'Left Center Right'
      min = 0
      max = 1
    [../]
  [../]
[]

[Kernels]
  [./klinkenberg]
    type = KlinkenbergPressure
    block = 'Left Center Right'
    klinkenberg_factor=0.01
  [../]

[]

[BCs]
  [./bottom]
    type = DirichletBC
    boundary = Bottom
    value = 15
  [../]
  [./top]
    type = DirichletBC
    boundary = Top
    value = 0
  [../]
[]

[Materials]
  [./DarcyBlock]
    type = DarcyMaterial
    block = 'Left Center Right'
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
[]

