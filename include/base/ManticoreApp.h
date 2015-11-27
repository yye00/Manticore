#ifndef MANTICOREAPP_H
#define MANTICOREAPP_H

#include "MooseApp.h"

class ManticoreApp;

template<>
InputParameters validParams<ManticoreApp>();

class ManticoreApp : public MooseApp
{
public:
  ManticoreApp(InputParameters parameters);
  virtual ~ManticoreApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* MANTICOREAPP_H */
