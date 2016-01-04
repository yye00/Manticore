
#include "CompressibleOutflow.h"

template<>
InputParameters validParams<CompressibleOutflow>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredCoupledVar("u", "x-velocity");
  params.addCoupledVar("v", 0, "y-velocity"); // only required in 2D and 3D
  params.addCoupledVar("w", 0, "z-velocity"); // only required in 3D
  params.addRequiredCoupledVar("p", "pressure");
  params.addRequiredParam<UserObjectName>("gas_density_userobject", "We need the gas density since we are working with compressible");
  return params;
}

CompressibleOutflow::CompressibleOutflow(const InputParameters & parameters) :
  IntegratedBC(parameters),
  // The coupled variables
  _u_vel(coupledValue("u")),
  _v_vel(coupledValue("v")),
  _w_vel(coupledValue("w")),
  _p(coupledValue("p")),
  // Density from Richards
  _density(getUserObject<RichardsDensity>("gas_density_userobject"))
{}

Real
CompressibleOutflow::computeQpResidual()
{
  // The velocity vector
  RealVectorValue U(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]); 

  return _density.density(_p[_qp])*(_normals[_qp]*U)*_test[_i][_qp];
}

Real
CompressibleOutflow::computeQpJacobian()
{
   // The velocity vector
  RealVectorValue U(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]); 

  return _phi[_j][_qp]*_density.ddensity(_p[_qp])*(_normals[_qp]*U)*_test[_i][_qp];
}