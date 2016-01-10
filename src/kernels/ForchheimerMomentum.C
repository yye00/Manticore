#include "ForchheimerMomentum.h"

template<>
InputParameters validParams<ForchheimerMomentum>()
{
  InputParameters params = validParams<Kernel>();
  // Coupled variables
  params.addRequiredCoupledVar("u", "x-velocity");
  params.addCoupledVar("v", 0, "y-velocity"); // only required in 2D and 3D
  params.addCoupledVar("w", 0, "z-velocity"); // only required in 3D
  params.addRequiredCoupledVar("p", "pressure");
  // other parameters
  params.addRequiredParam<UserObjectName>("gas_density_userobject", "We need the gas density since we are working with compressible");
  params.addRequiredParam<Real>("forchheimer_factor", "Beta, the forchheimer factor");
  params.addRequiredParam<Real>("fluid_viscosity", "Fluid dynamic viscosity (usually measured in Pa.s)");  
  params.addRequiredParam<RealVectorValue>("gravity_vector", "Gravity as a vector pointing downwards (usually measured in m^2/sec eg '0 0 -9.81'");
  params.addRequiredParam<unsigned>("component", "0,1,2 depending on if we are solving the x,y,z component of the momentum equation");
  return params;
}


ForchheimerMomentum::ForchheimerMomentum(const InputParameters & parameters) :
  Kernel(parameters),
  // Coupled gradients
  _grad_p(coupledGradient("p")),
  // Coupled variables
  _u_vel(coupledValue("u")),
  _v_vel(coupledValue("v")),
  _w_vel(coupledValue("w")),
  _p(coupledValue("p")),
  // Variable numberings
  _u_vel_var_number(coupled("u")),
  _v_vel_var_number(coupled("v")),
  _w_vel_var_number(coupled("w")),
  _p_var_number(coupled("p")),  
  // Density from Richards
  _density(getUserObject<RichardsDensity>("gas_density_userobject")),
  // forchheimer factor i.e. beta
  _forchheimer_factor(getParam<Real>("forchheimer_factor")),
  // the fluid viscosity
  _fluid_viscosity(getParam<Real>("fluid_viscosity")),  
  // permeability is from the Richard's material
  _permeability(getMaterialProperty<RealTensorValue>("permeability")),
  // Gravity is direction based
  _gravity_vector(getParam<RealVectorValue>("gravity_vector")),
  // the id of the component we are working with 0,1,2
  _component(getParam<unsigned>("component"))
{
}


Real ForchheimerMomentum::computeQpResidual()
{
  // Gradient[P+rho(P)*g*z]*psi term
  Real pressure_term = (_grad_p[_qp](_component) + _density.density(_p[_qp])*_gravity_vector(_component) ) *_test[_i][_qp];

  // It is easier to use the inverse of permeability
  // but we have to get the correct permeability
  // for every component
  const Real k_inv = 1.0/(_permeability[_qp](_component,_component));
  // The velocity magnitude
  Real vel_mag = std::sqrt(_u_vel[_qp]*_u_vel[_qp]+_v_vel[_qp]*_v_vel[_qp]+_w_vel[_qp]*_w_vel[_qp]);

  // Viscosity term is mu*K^-1 * U
  Real viscosity_term = _fluid_viscosity*k_inv*_u[_qp]*_test[_i][_qp];

  // Forchheimer term is rho * beta * magnitude(U) * U
  // where U=vector[u v w]
  Real forchheimer_term = _density.density(_p[_qp])*_forchheimer_factor*vel_mag*_u[_qp]*_test[_i][_qp];

  return pressure_term+viscosity_term+forchheimer_term;
}


Real ForchheimerMomentum::computeQpJacobian()
{
  // it is easier to use the inverse of permeability
  // but we have to get the correct permeability
  // for every component
  const Real k_inv = 1.0/((_permeability[_qp])(_component,_component));
  Real vel_mag = std::sqrt(_u_vel[_qp]*_u_vel[_qp]+_v_vel[_qp]*_v_vel[_qp]+_w_vel[_qp]*_w_vel[_qp]);

  // Viscosity term is mu*K^-1 * U
  // therefore the jacobian term would be:
  // mu*K^-1 * phi[j]
  Real viscosity_term = _fluid_viscosity*k_inv*_phi[_j][_qp] * _test[_i][_qp];
  
  // Forchheimer term is rho * beta * magnitude(U) * U
  // where U=vector[u v w]
  // therefore the jacobian term is:
  // rho*beta*d(magnitude(U))/du*phi[j]*U + rho*beta*magnitude(U)*phi[j]
  // since d(magnitude(U))/du = d(sqrt(u^2+v^2+w^2))/du = u/sqrt(u^2+v^2+w^2)
  Real forchheimer_term = _density.density(_p[_qp])*_forchheimer_factor*
         ( _phi[_j][_qp]*(_u[_qp]/vel_mag) *_u[_qp] 
        +  vel_mag*_phi[_j][_qp] )*_test[_i][_qp];

  return viscosity_term+forchheimer_term;
}


Real ForchheimerMomentum::computeQpOffDiagJacobian(unsigned jvar)
{

  Real vel_mag = std::sqrt(_u_vel[_qp]*_u_vel[_qp]+_v_vel[_qp]*_v_vel[_qp]+_w_vel[_qp]*_w_vel[_qp]);
  RealVectorValue U(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);

  if (jvar == _u_vel_var_number)
  {
    return _density.density(_p[_qp])*_forchheimer_factor*
           _phi[_j][_qp]*(_u_vel[_qp]/vel_mag) *_u[_qp] *_test[_i][_qp];
  }

  else if (jvar == _v_vel_var_number)
  {
    return _density.density(_p[_qp])*_forchheimer_factor*
           _phi[_j][_qp]*(_v_vel[_qp]/vel_mag) *_u[_qp] *_test[_i][_qp];
  }

  else if (jvar == _w_vel_var_number)
  {
    return _density.density(_p[_qp])*_forchheimer_factor*
           _phi[_j][_qp]*(_w_vel[_qp]/vel_mag) *_u[_qp] *_test[_i][_qp];
  }

  else if (jvar == _p_var_number)
  {
    return ( _grad_phi[_j][_qp](_component) 
           + _density.ddensity(_p[_qp])*_phi[_j][_qp]*_gravity_vector(_component)
           + _density.ddensity(_p[_qp])*_phi[_j][_qp]
           *_forchheimer_factor*vel_mag*U(_component) )*_test[_i][_qp];

  }

  else
    return 0;

}

