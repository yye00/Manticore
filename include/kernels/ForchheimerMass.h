

#ifndef FORCHHEIMERMASS_H
#define FORCHHEIMERMASS_H

#include "Kernel.h"

#include "RichardsDensity.h"

// Forward Declarations
class ForchheimerMass;

template<>
InputParameters validParams<ForchheimerMass>();

/**
 * This class computes the mass equation residual and Jacobian
 * contributions for the compressible forchheimer mass
 * equation.
 */
class ForchheimerMass : public Kernel
{
public:
  ForchheimerMass(const InputParameters & parameters);

  virtual ~ForchheimerMass(){}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Coupled gradients
  const VariableGradient & _grad_u_vel;
  const VariableGradient & _grad_v_vel;
  const VariableGradient & _grad_w_vel;
  const VariableGradient & _grad_p;

  // Coupled variables
  const VariableValue & _u_vel;
  const VariableValue & _v_vel;
  const VariableValue & _w_vel;
  const VariableValue & _p;

  // We will re-use the Richard's desntiy from the Richard's module
  const RichardsDensity & _density;

  // Variable numberings
  unsigned _u_vel_var_number;
  unsigned _v_vel_var_number;
  unsigned _w_vel_var_number;
  unsigned _p_var_number;
};


#endif // FORCHHEIMERMASS_H
