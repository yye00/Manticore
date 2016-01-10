

#ifndef FORCHHEIMERMOMENTUM_H
#define FORCHHEIMERMOMENTUM_H

#include "Kernel.h"

#include "RichardsDensity.h"

// Forward Declarations
class ForchheimerMomentum;

template<>
InputParameters validParams<ForchheimerMomentum>();

/**
 * This class computes the mass equation residual and Jacobian
 * contributions for the compressible forchheimer equation
 * equation.
 */
class ForchheimerMomentum : public Kernel
{
public:
  ForchheimerMomentum(const InputParameters & parameters);

  virtual ~ForchheimerMomentum(){}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Coupled gradients
  VariableGradient& _grad_p;

  // Coupled variables
  VariableValue& _u_vel;
  VariableValue& _v_vel;
  VariableValue& _w_vel;
  VariableValue& _p;

  // Variable numberings
  unsigned _u_vel_var_number;
  unsigned _v_vel_var_number;
  unsigned _w_vel_var_number;
  unsigned _p_var_number;

  // We will re-use the Richard's desntiy from the Richard's module
  const RichardsDensity & _density;

  // Forchheimer factor b
  Real _forchheimer_factor;

  // fluid dynamic viscosity
  Real _fluid_viscosity;  

  // Material permeability
  const MaterialProperty<RealTensorValue> &_permeability;

  // Gravity magnitude
  RealVectorValue _gravity_vector;

  // The component we are interested in
  unsigned _component;
};


#endif // FORCHHEIMERMOMENTUM_H