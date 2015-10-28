#include "DeterministicNode.h"
#include "Func_jones.h"
#include "Natural.h"
#include "RateMatrix_Jones.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_jones::Func_jones( void ) : Procedure( )
{
    
}


/** Clone object */
Func_jones* Func_jones::clone( void ) const
{
    
    return new Func_jones( *this );
}


RevPtr<RevVariable> Func_jones::execute()
{
    

    RevBayesCore::RateMatrix_Jones *rmj = new RevBayesCore::RateMatrix_Jones();
    RateMatrix* value = new RateMatrix( rmj );
    
    return new RevVariable( value );
}


/* Get argument rules */
const ArgumentRules& Func_jones::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& Func_jones::getClassType(void)
{
    
    static std::string revType = "Func_jones";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_jones::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_jones::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "fnJones";
    
    return f_name;
}


/* Get return type */
const TypeSpec& Func_jones::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RateGenerator::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_jones::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
