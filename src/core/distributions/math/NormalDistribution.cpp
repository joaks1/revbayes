
#include "NormalDistribution.h"
#include "DistributionNormal.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;

NormalDistribution::NormalDistribution(const TypedDagNode<double> *m, const TypedDagNode<double> *s, const TypedDagNode<double> *mi, const TypedDagNode<double> *ma) : ContinuousDistribution( new double( 0.0 ) ),
    mean( m ),
    stDev( s ),
    min( mi ),
    max( ma )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( mean );
    addParameter( stDev );
    addParameter( min );
    addParameter( max );
    if(min->getValue() != RbConstants::Double::neginf && max->getValue() != RbConstants::Double::inf){
        if ( min->getValue() >= max->getValue() )
        {
            throw RbException("The minimum value must be smaller then the maximum value for the truncated normal distribution.");
        }
        
        do {
            *value = RbStatistics::Normal::rv(mean->getValue(), stDev->getValue(), min->getValue(), max->getValue(), *GLOBAL_RNG);
        } while ( *value < min->getValue() && *value > max->getValue() );
        

    }
    else
        *value = RbStatistics::Normal::rv(mean->getValue(), stDev->getValue(), *GLOBAL_RNG);
}


NormalDistribution::~NormalDistribution( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


double NormalDistribution::cdf( void ) const {
    if(min->getValue() != RbConstants::Double::neginf && max->getValue() != RbConstants::Double::inf)
        return RbStatistics::Normal::cdf( mean->getValue(), stDev->getValue(), *value, min->getValue(), max->getValue());
    else
        return RbStatistics::Normal::cdf( mean->getValue(), stDev->getValue(), *value);

}


NormalDistribution* NormalDistribution::clone( void ) const {
    return new NormalDistribution( *this );
}


double NormalDistribution::computeLnProbability( void ) {
    if(min->getValue() != RbConstants::Double::neginf && max->getValue() != RbConstants::Double::inf){
        if ( min->getValue() > *value )
        {
            return RbConstants::Double::neginf;
        }
    
        if ( max->getValue() < *value )
        {
            return RbConstants::Double::neginf;
        }

        return RbStatistics::Normal::lnPdf(mean->getValue(), stDev->getValue(), *value, min->getValue(), max->getValue());
    }
    else
        return RbStatistics::Normal::lnPdf(mean->getValue(), stDev->getValue(), *value);

}


double NormalDistribution::getMax( void ) const {
    if(max->getValue() != RbConstants::Double::inf){
        return max->getValue();
    }
    else{
        return RbConstants::Double::inf;
    }
}


double NormalDistribution::getMin( void ) const {
    if(min->getValue() != RbConstants::Double::neginf){
        return min->getValue();
    }
    else{
        return RbConstants::Double::neginf;
    }
}


double NormalDistribution::quantile(double p) const {
    if(min->getValue() != RbConstants::Double::neginf && max->getValue() != RbConstants::Double::inf)
        return RbStatistics::Normal::quantile(mean->getValue(), stDev->getValue(), p, min->getValue(), max->getValue());
    else
        return RbStatistics::Normal::quantile(mean->getValue(), stDev->getValue(), p);
}


void NormalDistribution::redrawValue( void ) {
    if(min->getValue() != RbConstants::Double::neginf && max->getValue() != RbConstants::Double::inf){
        if ( min->getValue() >= max->getValue() )
        {
            throw RbException("The minimum value must be smaller then the maximum value for the truncated normal distribution.");
        }
        
        do {
            *value = RbStatistics::Normal::rv(mean->getValue(), stDev->getValue(), min->getValue(), max->getValue(), *GLOBAL_RNG);
        } while ( *value < min->getValue() && *value > max->getValue() );
    }
    else
        *value = RbStatistics::Normal::rv(mean->getValue(), stDev->getValue(), *GLOBAL_RNG);
}


/** Swap a parameter of the distribution */
void NormalDistribution::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == mean)
    {
        mean = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == stDev)
    {
        stDev = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == min)
    {
        min = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == max)
    {
        max = static_cast<const TypedDagNode<double>* >( newP );
    }
}
