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
  // Coupled gradients
  _grad_u_vel(coupledGradient("u")),
  _grad_v_vel(coupledGradient("v")),
  _grad_w_vel(coupledGradient("w")),
  _grad_p(coupledGradient("p")),
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
  // The mass continuity equation is:
  // nabla . (rho*U)* test=0
  // for compressible, rho is a function of pressure so 
  // rho*nabla.U*test + d(rho)/dp*nabla P*U*test = 0
  
  // The velocity vector
  RealVectorValue U(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
  // The divergence of velocity vector div(U)
  Real divU = _grad_u_vel[_qp](0) + _grad_v_vel[_qp](1) + _grad_w_vel[_qp](2) ;

  return ( _density.density(_p[_qp]) * divU 
         + _density.ddensity(_p[_qp]) * ( _grad_p[_qp]*U ) )* _test[_i][_qp];
}


Real ForchheimerMass::computeQpJacobian()
{
  // The velocity vector
  RealVectorValue U(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
  // The divergence of velocity vector div(U)
  Real divU = _grad_u_vel[_qp](0) + _grad_v_vel[_qp](1) + _grad_w_vel[_qp](2) ;
  
  // The derivative of the residual
  return ( _density.ddensity(_p[_qp])*_phi[_j][_qp]*divU
         + _density.ddensity(_p[_qp])*_grad_phi[_j][_qp]*U
         + _density.d2density(_p[_qp])*_phi[_j][_qp]*(_grad_p[_qp]*U) )*_test[_i][_qp];
}


Real ForchheimerMass::computeQpOffDiagJacobian(unsigned jvar)
{
  if (jvar == _u_vel_var_number)
    return ( _density.density(_p[_qp])*_grad_phi[_j][_qp](0)
           + _density.ddensity(_p[_qp])*_grad_p[_qp](0)*_phi[_j][_qp] )* _test[_i][_qp];

  else if (jvar == _v_vel_var_number)
    return ( _density.density(_p[_qp])*_grad_phi[_j][_qp](1)
           + _density.ddensity(_p[_qp])*_grad_p[_qp](1)*_phi[_j][_qp] )* _test[_i][_qp];

  else if (jvar == _w_vel_var_number)
    return ( _density.density(_p[_qp])*_grad_phi[_j][_qp](2)
           + _density.ddensity(_p[_qp])*_grad_p[_qp](2)*_phi[_j][_qp] )* _test[_i][_qp];
  else
    return 0;
}

