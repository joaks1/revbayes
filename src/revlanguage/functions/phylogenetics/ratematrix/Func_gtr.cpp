#include "GtrRateMatrixFunction.h"
#include "Func_gtr.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_gtr::Func_gtr( void ) : TypedFunction<RateGenerator>( )
{
    
}


/** Clone object */
Func_gtr* Func_gtr::clone( void ) const
{
    
    return new Func_gtr( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_gtr::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* er = static_cast<const Simplex &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* bf = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    
    if ( er->getValue().size() != (bf->getValue().size() * (bf->getValue().size()-1) / 2.0) )
    {
        throw RbException("The dimension betwee the base frequencies and the substitution rates does not match.");
    }
    
    RevBayesCore::GtrRateMatrixFunction* f = new RevBayesCore::GtrRateMatrixFunction( er, bf );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_gtr::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "exchangeRates"  , Simplex::getClassTypeSpec(), "The exchangeability rates between states.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "baseFrequencies", Simplex::getClassTypeSpec(), "The stationary frequencies of the states.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_gtr::getClassType(void)
{
    
    static std::string revType = "Func_gtr";
    
	return revType; 
}


/* Get class type spec describing type of object */
const TypeSpec& Func_gtr::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_gtr::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "fnGTR";
    
    return f_name;
}


const TypeSpec& Func_gtr::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
