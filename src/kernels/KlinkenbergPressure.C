
#include "KlinkenbergPressure.h"
#include "Material.h"

template<>
InputParameters validParams<KlinkenbergPressure>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<RealVectorValue>("fluid_weight", "Fluid weight (gravity*density) as a vector pointing downwards (usually measured in kg.m^-2.s^-2 = Pa/m).  Eg '0 0 -10000'");
  params.addRequiredParam<Real>("fluid_viscosity", "Fluid dynamic viscosity (usually measured in Pa.s)");
  params.addRequiredParam<Real>("klinkenberg_factor", "Klinkenberg factor, i.e. factor b in the equation K_g=K_inf*(1+b/pressure)");  
  params.addClassDescription("Klinkenberg Pressure.  nabla_i (k_ij/mu (nabla_j P - w_j)), where k_ij is the permeability tensor, mu is the fluid viscosity, P is the fluid pressure, and w_j is the fluid weight");
  return params;
}

KlinkenbergPressure::KlinkenbergPressure(const InputParameters & parameters) :
    Kernel(parameters),
    _fluid_weight(getParam<RealVectorValue>("fluid_weight")),
    _fluid_viscosity(getParam<Real>("fluid_viscosity")),
    _klinkenberg_factor(getParam<Real>("klinkenberg_factor")),
    _permeability(getMaterialProperty<RealTensorValue>("permeability"))
{
}

KlinkenbergPressure::~KlinkenbergPressure()
{
}

Real
KlinkenbergPressure::computeQpResidual()
{
  return _grad_test[_i][_qp]*(_permeability[_qp]*(1+_klinkenberg_factor/_u[_qp])*(_grad_u[_qp] - _fluid_weight))/_fluid_viscosity;
}


Real
KlinkenbergPressure::computeQpJacobian()
{

  //return _grad_test[_i][_qp]*(_permeability[_qp]*_grad_phi[_j][_qp])/_fluid_viscosity;
  //return (_grad_test[_i][_qp]*(_permeability[_qp]*_grad_phi[_j][_qp]))/_fluid_viscosity*(1+_klinkenberg_factor/_u[_qp]);

  return  -1.0*_grad_test[_i][_qp]*(_permeability[_qp]*_grad_u[_qp])/_fluid_viscosity*(_klinkenberg_factor/_u[_qp]/_u[_qp])*_phi[_j][_qp]
             +(_grad_test[_i][_qp]*(_permeability[_qp]*_grad_phi[_j][_qp]))/_fluid_viscosity*(1+_klinkenberg_factor/_u[_qp])
             +(_grad_test[_i][_qp]*(_permeability[_qp]*_fluid_weight))/_fluid_viscosity*(_klinkenberg_factor/_u[_qp]/_u[_qp])*_phi[_j][_qp];
             
}
