/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_simplex, which is used to create
 * simplex objects.
 *
 * @brief Declaration and implementation of Func_simplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_simplex_H
#define Func_simplex_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename valType>
class Func_simplex :  public RbFunction {

    public:
        // Basic utility functions
        Func_simplex<valType>*      clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        bool                        throws(void) const;                                         //!< One variant needs to throw

    protected:
        void                        clearArguments(void);                               //!< Clear the arguments of this class
        const RbLanguageObject&     executeFunction(void);                                      //!< Execute function
        void                        setArgumentVariable(const std::string& name, const RbVariablePtr& var);

    private:
    
        // memberfunction return value
        Simplex                     s;
    
        // arguments
        std::vector<RbVariablePtr>  values;
};

#endif


#include "Ellipsis.h"
#include "Integer.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "Simplex.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"



/** Clear the arguments. We empty the list of elements to print. Then give the call back to the base class. */
template <typename valType>
void Func_simplex<valType>::clearArguments(void) {
    // just empty the elements list, the super smart pointers will take care of the rest
    values.clear();
    
}


/** Clone the object */
template <typename valType>
Func_simplex<valType>* Func_simplex<valType>::clone( void ) const {
    return new Func_simplex<valType>( *this );
}


/** Execute function: Simplex <- ( Integer ) */
template <>
const RbLanguageObject& Func_simplex<Integer>::executeFunction( void ) {

    int size = static_cast<const Integer&>( values[0]->getValue() ).getValue();

    if ( size < 2 )
        throw RbException( "Simplex size must be at least 2" );

    s = Simplex( size );

    return s;
}


/** Execute function: Simplex <- ( VectorRealPos ) */
template <>
const RbLanguageObject& Func_simplex<VectorRealPos>::executeFunction( void ) {

    const VectorRealPos& tempVec = static_cast<VectorRealPos&>( values[0]->getValue() );

    s.setValue( tempVec );

    return s;
}


/** Execute function: Simplex <- ( RealPos, RealPos, ... ) */
template <>
const RbLanguageObject& Func_simplex<RealPos>::executeFunction( void ) {

    VectorReal  tempVec;
    for ( size_t i = 0; i < values.size(); i++ )
        tempVec.push_back( static_cast<const RealPos&>( values[i]->getValue() ) );

    // Normalization is done by the Simplex constructor
    s.setValue( tempVec );
    return s;
}


/** Get argument rules for general case */
template <typename valType>
const ArgumentRules& Func_simplex<valType>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "", valType().getTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get argument rules for: Simplex <- ( RealPos, RealPos, ... ) */
template <>
const ArgumentRules& Func_simplex<RealPos>::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) 
        {
        argumentRules.push_back( new ValueRule( "", RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ValueRule( "", RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new Ellipsis (     RealPos::getClassTypeSpec() ) );
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class name of object */
template <typename firstValType>
const std::string& Func_simplex<firstValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func_simplex<" + firstValType().getType() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType>
const TypeSpec& Func_simplex<firstValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType>
const TypeSpec& Func_simplex<firstValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename firstValType>
const TypeSpec& Func_simplex<firstValType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Simplex::getClassTypeSpec();
    
    return returnTypeSpec;
}


/** Function does not throw in general case */
template <typename valType>
bool Func_simplex<valType>::throws( void ) const {

    return false;
}


/** Function does throw occasionally in Simplex <- ( Integer ) case */
template <>
bool Func_simplex<Integer>::throws( void ) const {

    return true;
}


/** We catch here the setting of the argument variables to store our parameters. */
template <typename valType>
void Func_simplex<valType>::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
    if ( name == "" ) {
        values.push_back( var );
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

