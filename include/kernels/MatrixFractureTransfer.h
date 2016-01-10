

#ifndef MATRIXFRACTURETRANSFER_H
#define MATRIXFRACTURETRANSFER_H

#include "Kernel.h"

#include "ShapeFactor.h"
#include "RichardsDensity.h"

// Forward Declarations
class MatrixFractureTransfer;

template<>
InputParameters validParams<MatrixFractureTransfer>();

/**
 * This class contains the kernel terms for mass transfer
 * between the matrix and fracture continua
 */
class MatrixFractureTransfer : public Kernel
{
public:
  MatrixFractureTransfer(const InputParameters & parameters);

  virtual ~MatrixFractureTransfer(){}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Coupled variables
  VariableValue& _p_primary;
  VariableValue& _p_secondary;

  // The shape factor
  const ShapeFactor & _shape_factor;

  // We will re-use the Richard's desntiy from the Richard's module
  const RichardsDensity & _density;

  // fluid dynamic viscosity
  Real _fluid_viscosity;  

  // Material permeability
  const MaterialProperty<RealTensorValue> &_permeability;
};


#endif // MATRIXFRACTURETRANSFER_H