/**
 * @file
 * This file contains the declaration of Func_size, which
 * is used to count the number of elements in a vector.
 *
 * @brief Declaration of Func_size
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_size_H
#define Func_size_H

#include "RbFunction.h"
#include "Natural.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_size_name = "Size function";

template <typename valType>
class Func_size :  public RbFunction {
	
public:
	// Basic utility functions
	Func_size*                  clone(void) const;                                  //!< Clone the object
    static const std::string&   getClassName(void);                                 //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                             //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                            //!< Get language type of the object
	
	// Regular functions
    const ArgumentRules&        getArgumentRules(void) const;                       //!< Get argument rules
	const TypeSpec&             getReturnType(void) const;                          //!< Get type of return value

protected:
    const RbLanguageObject&     executeFunction(void);                              //!< Execute operation
    void                        setArgumentVariable(const std::string& name, const RbVariablePtr& var);

private:
    
    // Arguments
    RbVariablePtr               value;
    
    // function return value
    Natural                     retValue;
};

#endif

#include "Ellipsis.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "ValueRule.h"



/** Clone object */
template <typename valType>
Func_size<valType>* Func_size<valType>::clone( void ) const {
    
    return new Func_size( *this );
}


/** Execute function: We rely on operator overloading to provide the necessary functionality */
template <typename valType> 
const RbLanguageObject& Func_size<valType>::executeFunction( void ) {
    
    const valType& val = static_cast<valType&> ( value->getValue() ) ;
    retValue.setValue( val.size() ); 
    
    return retValue;
}


/** Get argument rules */
template <typename valType>
const ArgumentRules& Func_size<valType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) 
    {
        argumentRules.push_back( new ValueRule( "value", valType() .getTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
template <typename firstValType>
const std::string& Func_size<firstValType>::getClassName(void) { 
    
    static std::string rbClassName = "Func_size<" + firstValType().getType() + ">";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
template <typename firstValType>
const TypeSpec& Func_size<firstValType>::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
template <typename firstValType>
const TypeSpec& Func_size<firstValType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
template <typename firstValType>
const TypeSpec& Func_size<firstValType>::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Natural::getClassTypeSpec();
    
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
template <typename firstValType>
void Func_size<firstValType>::setArgumentVariable(std::string const &name, const RbVariablePtr& var) {
    
    if ( name == "value" ) {
        value = var;
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}


