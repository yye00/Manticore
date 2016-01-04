#ifndef SHAPEFACTOR_H
#define SHAPEFACTOR_H

#include "GeneralUserObject.h"

#include "libmesh/elem.h"


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
  Real Factor(libMesh::Elem elem) const = 0;

};

#endif // SHAPEFACTOR_H