// emacs edit mode for this file is -*- C++ -*-
// $Id: claptmpl.cc,v 1.1.1.1 1997-03-19 13:18:44 obachman Exp $
#include "mod2.h"

#ifdef HAVE_LIBFACTORY
#define SINGULAR 1
#include <singfactory.h>
#endif

#if defined(HAVE_LIBFACTORY) || defined(HAVE_FGLM)
#include <templates/list.cc>
#include <templates/dynamicArray.cc>
#endif

#ifdef HAVE_FGLM
#include "fglm.h"
#endif

#ifdef HAVE_LIBFACTORY
#include <templates/array.cc>
#include <templates/factor.cc>
#include <templates/functions.h>
#include <templates/matrix.cc>

template class Factor<CanonicalForm>;
template class List<CFFactor>;
template class ListItem<CFFactor>;
template class ListItem<CanonicalForm>;
template class ListIterator<CFFactor>;
template class List<CanonicalForm>;
template class ListIterator<CanonicalForm>;
template class Array<CanonicalForm>;
template class List<MapPair>;
template class ListItem<MapPair>;
template class ListIterator<MapPair>;
template class Matrix<CanonicalForm>;
template class SubMatrix<CanonicalForm>;

template List<CFFactor> Union ( const List<CFFactor>&, const List<CFFactor>& );

template CanonicalForm tmax ( const CanonicalForm&, const CanonicalForm& );
template CanonicalForm tmin ( const CanonicalForm&, const CanonicalForm& );

template int tmax ( const int&, const int& );
template int tmin ( const int&, const int& );

// place here your own template stuff, not instantiated by factory

#ifdef HAVE_LIBFAC_P
#include <tmpl_inst.h>
#include <class.cc>

template class List<int>;
template class ListIterator<int>;

template class List<IntList>;
template class ListIterator<IntList>;

template class Substitution<CanonicalForm>;
template class Array<Variable>;
template class Array<int>;
template class List<SForm>;
template class ListIterator<SForm>;
template class List<Variable>;
template class ListIterator<Variable> ;

template List<Variable> Union ( const List<Variable>&, const List<Variable>& );
template List<Variable> Difference ( const List<Variable>&, const List<Variable>& );

template List<CanonicalForm> Union ( const List<CanonicalForm>&, const List<CanonicalForm>& );
template List<CanonicalForm> Difference ( const List<CanonicalForm>&, const List<CanonicalForm>& );

// for charsets:
template class List<CFList>;
template class ListIterator<CFList>;

#endif
#endif

#ifdef HAVE_FGLM
// templates for fglm:

template class List<listElem>;
template class ListIterator<listElem>;

template class List<fglmElem>;
template class ListIterator<fglmElem>;

#endif
