/**
 * @file
 * This file contains the implementation of Dist_gamma, which is used to hold
 * parameters and functions related to an gamma distribution.
 *
 * @brief Implementation of Dist_gamma
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z msuchard $
 */

#include "DAGNode.h"
#include "Dist_gamma.h"
#include "Move_mscale.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbUtil.h"
#include "Real.h"
#include "ValueRule.h"
#include "VectorString.h"
#include "Workspace.h"
#include "RbException.h"
#include "RbMathFunctions.h"
#include "DistributionGamma.h"
#include "RbStatisticsHelper.h"

#include <cmath>
#include <sstream>


// Definition of the static type spec member
const TypeSpec Dist_gamma::typeSpec(Dist_gamma_name);
const TypeSpec Dist_gamma::varTypeSpec(RealPos_name);

/** Default constructor for parser use */
Dist_gamma::Dist_gamma( void ) : DistributionContinuous( getMemberRules() ), shape( NULL ), rate( NULL ) {
    // Do nothing
}


/**
 * This function calculates the cumulative probability for
 * an gamma-distributed random variable.
 *
 * @brief Gamma cumulative probability
 *
 * @param q     Quantile
 * @return      Cumulative probability
 *
 */
double Dist_gamma::cdf( const RbLanguageObject& value ) {
    
    double alpha  = static_cast<      RealPos&>( shape->getValue() ).getValue();
    double lambda = static_cast<      RealPos&>( rate->getValue()  ).getValue();
    double x      = static_cast<const RealPos&>( value            ).getValue();    

    return RbStatistics::Gamma::cdf(alpha, lambda, x);
}


/** Clone this object */
Dist_gamma* Dist_gamma::clone( void ) const {

    return new Dist_gamma( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_gamma::getClass( void ) const {

    static VectorString rbClass = VectorString( Dist_gamma_name ) + DistributionContinuous::getClass();
    return rbClass;
}

/** Get member variable rules */
const MemberRules& Dist_gamma::getMemberRules( void ) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if ( !rulesSet ) {

        memberRules.push_back( new ValueRule( "shape", RealPos_name) );
        memberRules.push_back( new ValueRule( "rate", RealPos_name ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Dist_gamma::getTypeSpec(void) const {
    return typeSpec;
}


/** Get random variable type */
const TypeSpec& Dist_gamma::getVariableType( void ) const {

    return varTypeSpec;
}


/**
 * This function calculates the natural log of the probability
 * density for an gamma-distributed random variable.
 *
 * @brief Natural log of gamma probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_gamma::lnPdf( const RbLanguageObject& value ) const {
    
    double alpha  = static_cast<const RealPos&>( shape->getValue() ).getValue();
    double lambda = static_cast<const RealPos&>( rate->getValue()  ).getValue();
    double x      = static_cast<const RealPos&>( value            ).getValue();  
  
    return RbStatistics::Gamma::lnPdf(alpha, lambda, x); 
}


/**
 * This function calculates the probability density
 * for an gamma-distributed random variable.
 *
 * @brief Gamma probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_gamma::pdf( const RbLanguageObject& value ) const {
    
    double alpha  = static_cast<const RealPos&>( shape->getValue() ).getValue();
    double lambda = static_cast<const RealPos&>( rate->getValue()  ).getValue();
    double x      = static_cast<const RealPos&>( value            ).getValue();   
    
    return RbStatistics::Gamma::pdf(alpha, lambda, x, false);    
}


/**
 * This function calculates the quantile for a
 * gamma-distributed random variable.
 *
 * @brief Quantile of gamma probability density
 *
 * @param p     Cumulative probability of quantile
 * @return      Quantile
 *
 */
const Real& Dist_gamma::quantile(const double p) {
    
    double alpha  = static_cast<      RealPos&>( shape->getValue() ).getValue();
    double lambda = static_cast<      RealPos&>( rate->getValue()  ).getValue();
    
    double quantile = RbStatistics::Gamma::quantile(alpha, lambda, p);     
    quant.setValue( quantile );
    
    return quant;
}


/**
 * This function generates a gamma-distributed
 * random variable.
 *
 * @brief Random draw from gamma distribution
 *
 * @return      Random draw from gamma distribution
 */
const RbLanguageObject& Dist_gamma::rv( void ) {
    
    double alpha  = static_cast<      RealPos&>( shape->getValue() ).getValue();
    double lambda = static_cast<      RealPos&>( rate->getValue()  ).getValue();
    
    RandomNumberGenerator* rng = GLOBAL_RNG;        
    double rv = RbStatistics::Gamma::rv(alpha, lambda, *rng);
    randomVariable.setValue( rv );
    
    return randomVariable;
}


/** We catch here the setting of the member variables to store our parameters. */
void Dist_gamma::setMemberVariable(std::string const &name, Variable *var) {
    
    if ( name == "rate" ) {
        rate = var;
    }
    else if ( name == "shape" ){
        shape = var;
    }
    else {
        DistributionContinuous::setMemberVariable(name, var);
    }
}



