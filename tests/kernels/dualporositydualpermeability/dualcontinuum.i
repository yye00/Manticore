[GlobalParams]
  gravity_vector = '0 0 0'
  fluid_viscosity = 1
  klinkenberg_factor = 0.0
  gas_density_userobject = DensityConstBulk
[]


[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  xmin = 0.0
  xmax = 15.0
  ymin = 0.0
  ymax = 15.0
  distribution = serial # This uses SolutionUserObject which doesn't work with ParallelMesh.
[]

[Variables]
  [./matrix_pressure]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = ConstantIC
      value = 1.0
    [../]
  [../]

  [./fracture_pressure]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
      type = ConstantIC
      value = 1.0
    [../]
  [../]
[]

[UserObjects]
  [./DensityConstBulk]
    type = RichardsDensityConstBulk
    dens0 = 1.0
    bulk_mod = 100000000
  [../]

  [./IrregShapeFactor]
    type = ShapeFactor
  [../]
[]

[Kernels]
  [./matrix_klinkenberg]
    type = CompressibleKlinkenberg
    variable = matrix_pressure
  [../]

  [./matrix_transfer]
    type = MatrixFractureTransfer
    variable = matrix_pressure
    p_primary = matrix_pressure
    p_secondary = fracture_pressure
    shape_factor_userobject = IrregShapeFactor
    gas_density_userobject = DensityConstBulk
  [../]

  [./fracture_klinkenberg]
    type = CompressibleKlinkenberg
    variable = fracture_pressure
  [../]  

  [./facture_transfer]
    type = MatrixFractureTransfer
    variable = fracture_pressure
    p_primary = fracture_pressure
    p_secondary = matrix_pressure
    shape_factor_userobject = IrregShapeFactor
    gas_density_userobject = DensityConstBulk
  [../]  

[]

[BCs]
  [./left_matrix]
    type = DirichletBC
    boundary = 'left'
    value = 1
    variable = matrix_pressure
  [../]
  [./right_matrix]
    type = DirichletBC
    boundary = 'right'
    value = -149
    variable = matrix_pressure
  [../]
  [./left_fracture]
    type = DirichletBC
    boundary = 'left'
    value = 1.33750000000000
    variable = fracture_pressure
  [../]
  [./right_fracture]
    type = DirichletBC
    boundary = 'right'
    value = -148.662500000000
    variable = fracture_pressure
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
