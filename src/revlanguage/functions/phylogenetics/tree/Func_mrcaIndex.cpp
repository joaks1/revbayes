//
//  Func_mrcaIndex.cpp
//  RevLanguage
//
//  Created by Michael Landis on 8/19/14.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Clade.h"
#include "Func_mrcaIndex.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlClade.h"
#include "RlTimeTree.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "MrcaIndexStatistic.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_mrcaIndex::Func_mrcaIndex( void ) : TypedFunction<Natural>( )
{
    
}


/** Clone object */
Func_mrcaIndex* Func_mrcaIndex::clone( void ) const
{
    
    return new Func_mrcaIndex( *this );
}


RevBayesCore::TypedFunction<int>* Func_mrcaIndex::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    const RevBayesCore::Clade& c = static_cast<const Clade &>( this->args[1].getVariable()->getRevObject() ).getValue();
    RevBayesCore::MrcaIndexStatistic* f = new RevBayesCore::MrcaIndexStatistic( tau, c );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_mrcaIndex::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "tree" , TimeTree::getClassTypeSpec(), "The tree which is used to compute the MRCA.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "clade", Clade::getClassTypeSpec()   , "The clade for which the MRCA is searched.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_mrcaIndex::getClassType(void)
{
    
    static std::string revType = "Func_mrcaIndex";
    
	return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_mrcaIndex::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_mrcaIndex::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "mrcaIndex";
    
    return f_name;
}


const TypeSpec& Func_mrcaIndex::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
