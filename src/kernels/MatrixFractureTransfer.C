#include "MatrixFractureTransfer.h"

template<>
InputParameters validParams<MatrixFractureTransfer>()
{
  InputParameters params = validParams<Kernel>();
  // Coupled variables
  params.addRequiredCoupledVar("p_primary", "The primary pressure in the continuum");
  params.addRequiredCoupledVar("p_secondary", "The primary pressure in the continuum");  
  // other parameters
  params.addRequiredParam<UserObjectName>("shape_factor_userobject", "The geometric shape factor used to calculate the transfer term");
  params.addRequiredParam<UserObjectName>("gas_density_userobject", "We need the gas density since we are working with compressible");
  params.addRequiredParam<Real>("fluid_viscosity", "Fluid dynamic viscosity (usually measured in Pa.s)");  
  return params;
}


MatrixFractureTransfer::MatrixFractureTransfer(const InputParameters & parameters) :
  Kernel(parameters),
  // Coupled gradients
  _p_primary(coupledValue("p_primary")),
  _p_secondary(coupledValue("p_secondary")),  
  // Density from Richards
  _shape_factor(getUserObject<ShapeFactor>("shape_factor_userobject")),  
  // Density from Richards
  _density(getUserObject<RichardsDensity>("gas_density_userobject")),
  // the fluid viscosity
  _fluid_viscosity(getParam<Real>("fluid_viscosity")),  
  // permeability is from the Richard's material
  _permeability(getMaterialProperty<RealTensorValue>("permeability"))
{
}


Real MatrixFractureTransfer::computeQpResidual()
{

  // Get the shape factor
  Real sigma = _shape_factor.Factor(_current_elem);

  // We use the geometric mean
  Real perm = std::pow(_permeability[_qp](0,0)*_permeability[_qp](1,1)*_permeability[_qp](2,2),1/3.0);

  // sigma * k_avg * density(primary_pressure) /viscosity * (primary_pressure - secondary_pressure)
  return sigma*perm/_fluid_viscosity*_density.density(_p_primary[_qp])*(_p_primary[_qp]-_p_secondary[_qp])*_test[_i][_qp];
}


Real MatrixFractureTransfer::computeQpJacobian()
{
  // Get the shape factor
  Real sigma = _shape_factor.Factor(_current_elem);

  // We use the geometric mean
  Real perm = std::pow(_permeability[_qp](0,0)*_permeability[_qp](1,1)*_permeability[_qp](2,2),1/3.0);

  return  sigma*perm/_fluid_viscosity*_phi[_j][_qp]*_density.ddensity(_p_primary[_qp])*(_p_primary[_qp]-_p_secondary[_qp])*_test[_i][_qp]
         + sigma*perm/_fluid_viscosity*_density.density(_p_primary[_qp])*_phi[_j][_qp]*_test[_i][_qp];
}


Real MatrixFractureTransfer::computeQpOffDiagJacobian(unsigned jvar)
{
  // Get the shape factor
  Real sigma = _shape_factor.Factor(_current_elem);

  // We use the geometric mean
  Real perm = std::pow(_permeability[_qp](0,0)*_permeability[_qp](1,1)*_permeability[_qp](2,2),1/3.0);

  // Off-diagonal means we are computing derivative against the
  // secondary pressure
  return -sigma*perm/_fluid_viscosity*_density.density(_p_primary[_qp])*_phi[_j][_qp]*_test[_i][_qp]; 
}

