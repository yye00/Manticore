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

#ifndef COMPRESSIBLEOUTFLOW_H
#define COMPRESSIBLEOUTFLOW_H

#include "IntegratedBC.h"


class CompressibleOutflow;

template<>
InputParameters validParams<CompressibleOutflow>();

/**
 * An IntegratedBC representing the "No BC" boundary condition for
 * heat conduction.
 *
 * The residual is simply -test*k*grad_u*normal... the term you get
 * from integration by parts.  This is a standard technique for
 * truncating longer domains when solving the convection/diffusion
 * equation.
 *
 * See also: Griffiths, David F. "The 'no boundary condition' outflow
 * boundary condition.", International Journal for Numerical Methods
 * in Fluids, vol. 24, no. 4, 1997, pp. 393-411.
 */
class CompressibleOutflow : public IntegratedBC
{
public:
  CompressibleOutflow(const InputParameters & parameters);

protected:
  /// This is called to integrate the residual across the boundary
  virtual Real computeQpResidual();

  /// Optional (but recommended!) to compute the derivative of the
  /// residual with respect to _this_ variable
  virtual Real computeQpJacobian();
};


#endif //COMPRESSIBLEOUTFLOW_H