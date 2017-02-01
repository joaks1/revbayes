//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

//#include "BiogeographyRateGeneratorSequenceFunction.h"
#include "Func_generalRateGeneratorSequence.h"
#include "GeneralRateGeneratorSequenceFunction.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlRateMatrix.h"
#include "RateMatrix.h"
#include "RateGeneratorSequence.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "RlRateGeneratorSequence.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_generalRateGeneratorSequence::Func_generalRateGeneratorSequence( void ) : TypedFunction<RateGeneratorSequence>( ) {
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_generalRateGeneratorSequence* Func_generalRateGeneratorSequence::clone( void ) const {
    
    return new Func_generalRateGeneratorSequence( *this );
}


RevBayesCore::TypedFunction<RevBayesCore::RateGeneratorSequence>* Func_generalRateGeneratorSequence::createFunction() const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* rm = static_cast<const RateGenerator&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    unsigned nc = static_cast<const Natural&>( this->args[2].getVariable()->getRevObject() ).getValue();
    size_t ns = rm->getValue().getNumberOfStates();

    RevBayesCore::GeneralRateGeneratorSequenceFunction* f = new RevBayesCore::GeneralRateGeneratorSequenceFunction(ns, nc);
    f->setRateMatrix(rm);
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_generalRateGeneratorSequence::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "Q", RateGenerator::getClassTypeSpec(), "The per-character rate generator.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "numChars", Natural::getClassTypeSpec(), "The number of characters.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_generalRateGeneratorSequence::getClassType(void)
{
    
    static std::string revType = "Func_generalRateGeneratorSequence";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_generalRateGeneratorSequence::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_generalRateGeneratorSequence::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnGeneralRateGeneratorSequence";
    
    return f_name;
}


const TypeSpec& Func_generalRateGeneratorSequence::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Set a member variable */
void Func_generalRateGeneratorSequence::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{

    if ( name == "qSite" )
    {
        q = var;
    }
    else if ( name == "rfSite" )
    {
        rootFrequencies = var;
    }
    else if ( name == "numChars" )
    {
        numChars = var;
    }
    else
    {
        Function::setConstParameter(name, var);
    }
    
}
