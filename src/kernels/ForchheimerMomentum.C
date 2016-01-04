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
  params.addRequiredParam<UserObjectName>("gas_density_userobject", "We need the gas density since we are working with compressible");
  params.addRequiredParam<Real>("forchheimer_factor", "Beta, the forchheimer factor");
  params.addRequiredParam<Real>("fluid_viscosity", "Fluid dynamic viscosity (usually measured in Pa.s)");  
  params.addRequiredParam<Real>("gravity", "Direction of the gravity vector");  
  params.addRequiredParam<unsigned>("component", "0,1,2 depending on if we are solving the x,y,z component of the momentum equation");
  return params;
}


ForchheimerMomentum::ForchheimerMomentum(const InputParameters & parameters) :
  Kernel(parameters),
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
  _gravity(getParam<Real>("gravity")),
  // the id of the component we are working with 0,1,2
  _component(getParam<unsigned>("component"))
{
}


Real ForchheimerMomentum::computeQpResidual()
{
  // Gravity is alwayz the z direction
  Real z = _q_point[_qp](2);
  // -[P+rho(P)*g*z]*div(psi) term
  Real pressure_term = -(_p[_qp]+_density.density(_p[_qp])*_gravity*z)*_grad_test[_i][_qp](_component);

  // it is easier to use the inverse of permeability
  // but we have to get the correct permeability
  // for every component
  const Real k_inv = 1.0/((_permeability[_qp])(_component,_component));

  Real vel_mag = std::sqrt(_u_vel[_qp]*_u_vel[_qp]+_v_vel[_qp]*_v_vel[_qp]+_w_vel[_qp]*_w_vel[_qp]);

  Real viscosity_term = _fluid_viscosity*k_inv*_u[_qp]*_test[_i][_qp];
  Real forchheimer_term = _forchheimer_factor*_density.density(_p[_qp])*vel_mag*_u[_qp]*_test[_i][_qp];

  return pressure_term+viscosity_term+forchheimer_term;
}


Real ForchheimerMomentum::computeQpJacobian()
{
  // it is easier to use the inverse of permeability
  // but we have to get the correct permeability
  // for every component
  const Real k_inv = 1.0/((_permeability[_qp])(_component,_component));

  Real vel_mag = std::sqrt(_u_vel[_qp]*_u_vel[_qp]+_v_vel[_qp]*_v_vel[_qp]+_w_vel[_qp]*_w_vel[_qp]);


  Real viscosity_term = _fluid_viscosity*k_inv*_phi[_j][_qp];
  Real forchheimer_term = _forchheimer_factor*_density.density(_p[_qp])*(_phi[_j][_qp]*_u[_qp]+vel_mag*_phi[_j][_qp])*_test[_i][_qp];  

  return viscosity_term+forchheimer_term;
}


Real ForchheimerMomentum::computeQpOffDiagJacobian(unsigned jvar)
{
  // it is easier to use the inverse of permeability
  // but we have to get the correct permeability
  // for every component
  const Real k_inv = 1.0/((_permeability[_qp])(_component,_component));
  
  Real vel_mag = std::sqrt(_u_vel[_qp]*_u_vel[_qp]+_v_vel[_qp]*_v_vel[_qp]+_w_vel[_qp]*_w_vel[_qp]);
  RealVectorValue U(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);

  if (jvar == _u_vel_var_number)
    return -_density.density(_p[_qp])*_phi[_j][_qp] * _grad_test[_i][_qp](0);

  else if (jvar == _v_vel_var_number)
    return -_density.density(_p[_qp])*_phi[_j][_qp] * _grad_test[_i][_qp](1);

  else if (jvar == _w_vel_var_number)
    return -_density.density(_p[_qp])*_phi[_j][_qp] * _grad_test[_i][_qp](2);

  else if (jvar == _p_var_number)
    return -_phi[_j][_qp]*_grad_test[_i][_qp](_component)-_density.ddensity(_p[_qp])+_forchheimer_factor*_density.density(_p[_qp])*vel_mag*U(_component);

  else
    return 0;
  return 0;
}

