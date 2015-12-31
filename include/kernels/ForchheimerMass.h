

#ifndef FORCHHEIMERMASS_H
#define FORCHHEIMERMASS_H

#include "Kernel.h"

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

  // Coupled variables
  VariableValue& _u_vel;
  VariableValue& _v_vel;
  VariableValue& _w_vel;
  VariableValue& _p;

  // We will re-use the Richard's desntiy from the Richard's module
  const RichardsDensity & _density;
};


#endif // FORCHHEIMERMASS_H