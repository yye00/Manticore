/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "CompressibleOutflow.h"

template<>
InputParameters validParams<CompressibleOutflow>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<UserObjectName>("density", "User object that defines the fluid density");
  return params;
}

HeatConductionOutflow::CompressibleOutflow(const InputParameters & parameters) :
  IntegratedBC(parameters),
  _density(&getUserObjectByName<RichardsDensity>(getParam<UserObjectName>("density")))
{}

Real
CompressibleOutflow::computeQpResidual()
{
  return -_test[_i][_qp]*density[_qp]*_grad_u[_qp]*_normals[_qp];
}

Real
CompressibleOutflow::computeQpJacobian()
{
  // Derivative of the residual with respect to "u"
  return -_test[_i][_qp]*density[_qp]*_grad_phi[_j][_qp]*_normals[_qp];
}