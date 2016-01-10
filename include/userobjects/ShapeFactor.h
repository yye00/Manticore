#ifndef SHAPEFACTOR_H
#define SHAPEFACTOR_H

#include "GeneralUserObject.h"


class ShapeFactor;

template<>
InputParameters validParams<ShapeFactor>();

/**
 * The shape factor needed for matrix-fracture transfer
 */
class ShapeFactor : public GeneralUserObject
{
public:
  ShapeFactor(const InputParameters & parameters);

  void initialize();
  void execute();
  void finalize();

  /**
   * The actual shape factor
   * @param elem the _current_elem element from the geometry
   */
  Real Factor(const Elem * elem) const;

};

#endif // SHAPEFACTOR_H