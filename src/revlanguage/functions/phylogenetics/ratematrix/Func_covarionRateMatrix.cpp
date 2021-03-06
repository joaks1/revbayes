//
//  Func_covarionRateMatrix.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/4/17.
//  Copyright © 2017 Michael Landis. All rights reserved.
//

#include "Func_covarionRateMatrix.h"
#include "CovarionRateMatrixFunction.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_covarionRateMatrix::Func_covarionRateMatrix( void ) : TypedFunction<RateGenerator>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_covarionRateMatrix* Func_covarionRateMatrix::clone( void ) const
{
    
    return new Func_covarionRateMatrix( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_covarionRateMatrix::createFunction( void ) const
{
    
    //    argumentRules.push_back( new ArgumentRule( "Q"                    , ModelVector<RateGenerator>::getClassTypeSpec(), "The rate matrix classes", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    //    argumentRules.push_back( new ArgumentRule( "switch_rates"         , RateMatrix::getClassTypeSpec(), "The class-switching rate matrix", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    //    argumentRules.push_back( new ArgumentRule( "clock_rates"          , ModelVector<RealPos>::getClassTypeSpec(), "The rate multipliers per class", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RateGenerator> >* rm = static_cast<const ModelVector<RateGenerator> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* sr = static_cast<const RateMatrix&>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* cr = static_cast<const ModelVector<RealPos> &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    
    
    RevBayesCore::AbstractRateMatrix* asr = dynamic_cast<RevBayesCore::AbstractRateMatrix*>( &sr->getValue() );
    if (asr == NULL)
    {
        throw RbException( "This type of RateMatrix cannot be properly converted to AbstractRateMatrix (unexpected bug, contact Michael Landis)." );
    }
    
    // sanity check
    if ( sr->getValue().size() != cr->getValue().size() )
    {
        throw RbException( "switch_rates and clock_rates have different numbers of classes." );
    }
    
    RevBayesCore::CovarionRateMatrixFunction* f = new RevBayesCore::CovarionRateMatrixFunction( rm, sr, cr );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_covarionRateMatrix::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        argumentRules.push_back( new ArgumentRule( "Q"                    , ModelVector<RateGenerator>::getClassTypeSpec(), "The rate matrix classes", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "switch_rates"         , RateMatrix::getClassTypeSpec(), "The class-switching rate matrix", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "clock_rates"          , ModelVector<RealPos>::getClassTypeSpec(), "The rate multipliers per class", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_covarionRateMatrix::getClassType(void)
{
    
    static std::string rev_type = "Func_covarionRateMatrix";
    
    return rev_type;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_covarionRateMatrix::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_covarionRateMatrix::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnCovarionRateMatrix";
    
    return f_name;
}


const TypeSpec& Func_covarionRateMatrix::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}
