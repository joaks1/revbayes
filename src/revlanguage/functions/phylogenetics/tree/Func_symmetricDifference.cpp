//
//  Func_symmetricDifference.cpp
//  RevBayesCore
//
//  Created by Bastien Boussau on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_symmetricDifference.h"
#include "ModelVector.h"
#include "RlTimeTree.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "SymmetricDifferenceStatistic.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_symmetricDifference::Func_symmetricDifference( void ) : TypedFunction<RealPos>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_symmetricDifference* Func_symmetricDifference::clone( void ) const {
    
    return new Func_symmetricDifference( *this );
}


RevBayesCore::TypedFunction< double >* Func_symmetricDifference::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau2 = static_cast<const TimeTree&>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::SymmetricDifferenceStatistic* f = new RevBayesCore::SymmetricDifferenceStatistic( tau, tau2 );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_symmetricDifference::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "tree1", Tree::getClassTypeSpec(), "The first tree." , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "tree2", Tree::getClassTypeSpec(), "The second tree.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_symmetricDifference::getClassType(void)
{
    
    static std::string revType = "Func_symmetricDifference";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_symmetricDifference::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_symmetricDifference::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "symDiff";
    
    return f_name;
}


const TypeSpec& Func_symmetricDifference::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
