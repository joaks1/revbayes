/**
 * @file
 * This file contains the declaration of Ellipsis, which is
 * used to describe a dot dot dot formal (...), representing
 * a variable number of arguments. Ellipsis must always be
 * used as the last argument rule. It will match any number of
 * arguments, including 0. Types are not checked.
 *
 * @brief Declaration of Ellipsis 
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#ifndef Ellipsis_H
#define Ellipsis_H

#include "ArgumentRule.h"

#include <ostream>
#include <string>

class DAGNode;
class VectorString;

class Ellipsis : public ArgumentRule {

    public:
                                        Ellipsis( void );                                   //! No labels or types required, value arguments by default
                                        Ellipsis(const TypeSpec& typeSp);                   //! Some type specification needs to be met, value arguments by default

        // Basic utility functions
        Ellipsis*                       clone(void) const { return new Ellipsis(*this); }   //!< Clone object
        static const std::string&       getClassName(void);                                 //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                             //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                            //!< Get language type of the object
        void                            printValue(std::ostream& o) const;                  //!< Print value for user
    
};

#endif

