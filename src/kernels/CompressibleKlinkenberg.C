#include "CompressibleKlinkenberg.h"
#include "Material.h"

template<>
InputParameters validParams<CompressibleKlinkenberg>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<UserObjectName>("gas_density_userobject", "We need the gas density since we are working with compressible");
  params.addRequiredParam<RealVectorValue>("gravity_vector", "Gravity as a vector pointing downwards (usually measured in m^2/sec eg '0 0 -9.81'");
  params.addRequiredParam<Real>("fluid_viscosity", "Fluid dynamic viscosity (usually measured in Pa.s)");
  params.addRequiredParam<Real>("klinkenberg_factor", "Klinkenberg factor, i.e. factor b in the equation K_g=K_inf*(1+b/pressure)");
  params.addClassDescription("Klinkenberg Pressure.  nabla_i (k_ij/mu (nabla_j P - w_j)), where k_ij is the permeability tensor, mu is the fluid viscosity, P is the fluid pressure, and w_j is the fluid weight");
  return params;
}

CompressibleKlinkenberg::CompressibleKlinkenberg(const InputParameters & parameters) :
    Kernel(parameters),
    _density(getUserObject<RichardsDensity>("gas_density_userobject")),
    _gravity_vector(getParam<RealVectorValue>("gravity_vector")),
    _fluid_viscosity(getParam<Real>("fluid_viscosity")),
    _klinkenberg_factor(getParam<Real>("klinkenberg_factor")),
    _permeability(getMaterialProperty<RealTensorValue>("permeability"))
{
}

CompressibleKlinkenberg::~CompressibleKlinkenberg()
{
}

Real
CompressibleKlinkenberg::computeQpResidual()
{
  return _density.density(_u[_qp])/_fluid_viscosity*_grad_test[_i][_qp]*(_permeability[_qp]*(1+_klinkenberg_factor/_u[_qp])*(_grad_u[_qp] - _density.density(_u[_qp])*_gravity_vector));
}


Real
CompressibleKlinkenberg::computeQpJacobian()
{
  const Real density_term =  _phi[_j][_qp]*_density.ddensity(_u[_qp])/_fluid_viscosity*(1+_klinkenberg_factor/_u[_qp])*(_grad_test[_i][_qp]*(_permeability[_qp]*(_grad_u[_qp]- _density.density(_u[_qp])*_gravity_vector))) ;
  const Real perm_term = _density.density(_u[_qp])/_fluid_viscosity*_phi[_j][_qp]*(-1.0*_klinkenberg_factor/_u[_qp]/_u[_qp])*(_grad_test[_i][_qp]*(_permeability[_qp]*(_grad_u[_qp]- _density.density(_u[_qp])*_gravity_vector)));
  const Real press_term = _density.density(_u[_qp])/_fluid_viscosity*(1+_klinkenberg_factor/_u[_qp])*(_grad_test[_i][_qp]*(_permeability[_qp]*(_grad_phi[_j][_qp]- _phi[_j][_qp]*_density.ddensity(_u[_qp])*_gravity_vector)));

  return density_term+perm_term+press_term;           
}
