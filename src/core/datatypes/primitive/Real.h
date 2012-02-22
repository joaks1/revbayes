/**
 * @file
 * This file contains the declaration of Real, which is the
 * primitive RevBayes type for real numbers.
 *
 * @brief Declaration of Real
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#ifndef Real_H
#define Real_H

#include "RbLanguageObject.h"
#include <ostream>
#include <string>

class RbBoolean;

/**
 * Real is the class used to hold a real value. Internally, the real is represented by
 * a double.
 *
 * @note Some functions are virtual because RealPos is derived from Real
 */

class Real : public RbLanguageObject {

    public:

                                    Real(void);                                                             //!< Default constructor (0.0)
                                    Real(const double v);                                                   //!< Construct from double
                                    Real(const int v);                                                      //!< Construct from int 
                                    Real(const unsigned int v);                                             //!< Construct from unsigned int 
                                    Real(const bool v);                                                     //!< Construct from bool
                                    Real(const Real& x);                                                    //!< Copy constructor
    
        // Overloaded operators
                                    operator double(void) const { return value; }                           //!< Type conversion to double for convenience
        
        // Basic utility functions
        virtual Real*               clone(void) const;                                                      //!< Clone object
        virtual RbObject*           convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        static const std::string&   getClassName(void);                                                     //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                 //!< Get class type spec
        virtual const TypeSpec&     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual double&             getValueReference(void) { return value; }                               //!< Get value reference
        virtual bool                isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?
        virtual void                printValue(std::ostream& o) const;                                      //!< Print value (for user)

        // Getters and setters
        virtual void                setValue(double x) { value = x; }                                       //!< Set value
        virtual double              getValue(void) const { return value; }                                  //!< Get value

	protected:
        double                      value;                                                                  //!< Value member
    
};

        // Operators defined outside of the class
        Real                        operator+ (const Real& A);                                              //!< Unary operator + 
        Real                        operator- (const Real& A);                                              //!< Unary operator - 
        RbBoolean                     operator! (const Real& A);                                              //!< Unary operator !

#endif

