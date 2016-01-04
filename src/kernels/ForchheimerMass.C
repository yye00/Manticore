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
  params.addRequiredParam<UserObjectName>("gas_density_userobject", "We need the gas density since we are working with compressible");
  return params;
}



ForchheimerMass::ForchheimerMass(const InputParameters & parameters) :
  Kernel(parameters),
  // Coupled variables
  _u_vel(coupledValue("u")),
  _v_vel(coupledValue("v")),
  _w_vel(coupledValue("w")),
  _p(coupledValue("p")),
  // Density from Richards
  _density(getUserObject<RichardsDensity>("gas_density_userobject")),
  // Variable numberings
  _u_vel_var_number(coupled("u")),
  _v_vel_var_number(coupled("v")),
  _w_vel_var_number(coupled("w")),
  _p_var_number(coupled("p"))
{
}


Real ForchheimerMass::computeQpResidual()
{
  // div(rho*u) * q
  // integrate by parts it becomes
  // integral_bc_rho*U.n*q - integral_vol_rho*U*gradient(q)
  return -(   _u_vel[_qp]*_grad_test[_i][_qp](0)
            + _v_vel[_qp]*_grad_test[_i][_qp](1)
            + _w_vel[_qp]*_grad_test[_i][_qp](2) )*_density.density(_p[_qp]);
}


Real ForchheimerMass::computeQpJacobian()
{
  // Derivative wrt to p is zero
  return -(   _u_vel[_qp]*_grad_test[_i][_qp](0)
            + _v_vel[_qp]*_grad_test[_i][_qp](1)
            + _w_vel[_qp]*_grad_test[_i][_qp](2) )*_phi[_j][_qp]*_density.ddensity(_p[_qp]);
}


Real ForchheimerMass::computeQpOffDiagJacobian(unsigned jvar)
{
  if (jvar == _u_vel_var_number)
    return -_density.density(_p[_qp])*_phi[_j][_qp] * _grad_test[_i][_qp](0);

  else if (jvar == _v_vel_var_number)
    return -_density.density(_p[_qp])*_phi[_j][_qp] * _grad_test[_i][_qp](1);

  else if (jvar == _w_vel_var_number)
    return -_density.density(_p[_qp])*_phi[_j][_qp] * _grad_test[_i][_qp](2);
  else
    return 0;
}

