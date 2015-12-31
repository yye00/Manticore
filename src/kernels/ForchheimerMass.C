#include "ForchheimerMass.h"

template<>
InputParameters validParams<ForchheimerMass>()
{
  InputParameters params = validParams<Kernel>();

  // Coupled variables
  params.addRequiredCoupledVar("u", "x-velocity");
  params.addCoupledVar("v", 0, "y-velocity"); // only required in 2D and 3D
  params.addCoupledVar("w", 0, "z-velocity"); // only required in 3D
  params.addRequiredCoupledVar("p", "pressure");
  params.addRequiredParam<RichardsDensity>("gas_density_userobject", "We need the gas density since we are working with compressible");

  return params;
}



ForchheimerMass::ForchheimerMass(const InputParameters & parameters) :
  Kernel(parameters),

  // Coupled variables
  _u_vel(coupledValue("u")),
  _v_vel(coupledValue("v")),
  _w_vel(coupledValue("w")),
  _p(coupledValue("p")),
{
}



Real ForchheimerMass::computeQpResidual()
{
  // div(rho*u) * q
  // integrate by parts it becomes
  // integral_bc_rho*U.n*q - integral_vol_rho*U*gradient(q)
  return -(_u_vel[_qp](0) + _v_vel[_qp](1) + _w_vel[_qp](2)) * _grad_test[_i][_qp];
}




Real ForchheimerMass::computeQpJacobian()
{
  // Derivative wrt to p is zero
  return 0;
}




Real ForchheimerMass::computeQpOffDiagJacobian(unsigned jvar)
{
  if (jvar == _u_vel_var_number)
    return -_grad_phi[_j][_qp](0) * _test[_i][_qp];

  else if (jvar == _v_vel_var_number)
    return -_grad_phi[_j][_qp](1) * _test[_i][_qp];

  else if (jvar == _w_vel_var_number)
    return -_grad_phi[_j][_qp](2) * _test[_i][_qp];
  else
    return 0;
}
Status API Training Shop Blog About Pricing
