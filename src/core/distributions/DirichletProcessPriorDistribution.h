/**
 * @file
 * This file contains the declaration of the DPP class. 
 * A mixture object holds the mapping between parameter values and the indix of this parameters.
 *
 *
 * @brief Declaration of the Mixture class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-18
 *
 * $Id$
 */

#ifndef DirichletProcessPriorDistribution_H
#define DirichletProcessPriorDistribution_H

#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template <class valueType>
    class DirichletProcessPriorDistribution : public TypedDistribution< std::vector<valueType> > {
        
    public:
        // constructor(s)
        DirichletProcessPriorDistribution(TypedDistribution<valueType> *g, const TypedDagNode< double > *cp, int n);
        DirichletProcessPriorDistribution(const DirichletProcessPriorDistribution<valueType> &n);
        
        // public member functions
        DirichletProcessPriorDistribution*                  clone(void) const;                                                                      //!< Create an independent clone
        double                                              computeLnProbability(void);
        int													getNumberOfCategories(void) const;
        int													getNumberOfElements(void) const;
        void                                                redrawValue(void);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                //!< Implementation of swaping parameters
		
        std::vector<std::set<int> >							getTableSeating(void); // maybe unnecessary
		std::vector<valueType>								getTableParameters(void);
		std::vector<int>									getElementAllocationVec(void);
		void												createRestaurantVectors(void);
		std::vector<int>									getNumElemPerTable(void);
		double												getConcentrationParam(void);
		TypedDistribution<valueType>*						getBaseDistribution(void);

    private:
        // helper methods
        void                                                computeDenominator();
		std::vector<valueType>*                             simulate();
		      
        // private members
		TypedDistribution<valueType>*						baseDistribution;
		const TypedDagNode< double > *						concentration;
		int													numElements;
        int													numTables;
		
        double                                              denominator;
        bool                                                concentrationHasChanged;
		std::vector<int>									numCustomerPerTable;
		std::vector<int>									allocationVector;
		std::vector<valueType>								valuePerTable;
    };
    
}

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbMathCombinatorialFunctions.h"
#include "VectorIndexOperator.h"

#include <cmath>

template <class valueType>
RevBayesCore::DirichletProcessPriorDistribution<valueType>::DirichletProcessPriorDistribution(TypedDistribution<valueType> *g, const TypedDagNode< double > *cp,
																							  int n) :
																							  TypedDistribution< std::vector<valueType> >( new std::vector<valueType>() ), baseDistribution( g ), concentration( cp ),
																							  numElements( n ), numTables( 0 ), concentrationHasChanged( true ), denominator( 0.0 ) {
    
    this->addParameter( concentration );
//    this->addParameter( baseDistribution );
    const std::set<const DagNode*>& p = baseDistribution->getParameters();
	for (std::set<const DagNode*>::const_iterator it = p.begin(); it != p.end(); ++it) {
		this->addParameter( *it );
	}
    
	delete this->value;

    this->value = simulate();
                                                                                                  
}

template <class valueType>
RevBayesCore::DirichletProcessPriorDistribution<valueType>::DirichletProcessPriorDistribution(const DirichletProcessPriorDistribution<valueType> &n) : TypedDistribution<std::vector<valueType> >( n ),
																								baseDistribution( n.baseDistribution ), concentration( n.concentration ), 
																								numElements( n.numElements ), numTables( n.numTables ), concentrationHasChanged( true ), denominator( 0.0 ) {
    // parameters are added automatically
    delete this->value;
    this->value = simulate();
                                                                                                    
}



template <class valueType>
RevBayesCore::DirichletProcessPriorDistribution<valueType>* RevBayesCore::DirichletProcessPriorDistribution<valueType>::clone( void ) const {
	
    return new DirichletProcessPriorDistribution<valueType>( *this );
}


template <class valueType>
void RevBayesCore::DirichletProcessPriorDistribution<valueType>::computeDenominator( void ) {
    // the denominator is sum^N_{i=1} ln( alpha + i - 1 )
    
    denominator = 0;
    int N = numElements;
    double cp = concentration->getValue() - 1.0;
    for (int i = 1; i <= N; ++i)
    {
        denominator += log( cp + i );
    }
    
//    concentrationHasChanged = false;
}




template <class valueType>
double RevBayesCore::DirichletProcessPriorDistribution<valueType>::computeLnProbability( void ) {
    
    // the probability is given by:
    // lnP = K * ln( alpha ) + sum^K_{i=1}( ln( factorial( n_i - 1 ) ) ) - sum^N_{i=1} ln( alpha + i - 1 )
    
    // reset the lnProb and set it to log( alpha^K )
    double lnProb = log( concentration->getValue() ) * numTables;
    
    if ( concentrationHasChanged == true ){
        computeDenominator();
    }
	
	lnProb += log(RbMath::stirlingFirst(numElements, numTables));
    
    for (int i = 0; i < numTables; ++i){
		// compute the probability of having n_i customers per at table i
		lnProb += RbMath::lnFactorial( numCustomerPerTable[i] - 1 );
        
        // we also need to multiply with the probability of the value for this table
        baseDistribution->setValue( valuePerTable[i] );
        lnProb += baseDistribution->computeLnProbability();
    }
    
    lnProb -= denominator;
	return lnProb;
}



template <class valueType>
int RevBayesCore::DirichletProcessPriorDistribution<valueType>::getNumberOfCategories( void ) const {
    
    return numTables;
}

template <class valueType>
int RevBayesCore::DirichletProcessPriorDistribution<valueType>::getNumberOfElements( void ) const {
    
    return numElements;
}


template <class valueType>
std::vector<valueType>* RevBayesCore::DirichletProcessPriorDistribution<valueType>::simulate() {
    
    // simulator function
	std::vector<valueType>* rv = new std::vector<valueType>();
	std::vector<valueType>& temp = *rv;
	double cp = concentration->getValue();
	
	RandomNumberGenerator* rng = GLOBAL_RNG;
	numTables = 0;
	
	for( int i=0; i<numElements; i++){
		double probNewCat = cp / (i + cp);
		double u = rng->uniform01();
		if( u < probNewCat){
			numCustomerPerTable.push_back(1);
			numTables++;
			allocationVector.push_back(numTables);
			baseDistribution->redrawValue();
			valueType newVal = baseDistribution->getValue(); //simulate function
			valuePerTable.push_back(newVal);
			temp.push_back(newVal);
		}
		else{
			double sum = 0.0;
			double m = rng->uniform01();
			for(int j=0; j<numCustomerPerTable.size(); j++){
				sum += (double)numCustomerPerTable[j] / i;
				if(m < sum){
					numCustomerPerTable[j] += 1;
					temp.push_back(valuePerTable[j]);
					allocationVector.push_back(j);
					break;
				}
			}
		}		
	}
	return rv ;
}


template <class valueType>
void RevBayesCore::DirichletProcessPriorDistribution<valueType>::redrawValue( void ) {

    delete this->value;
    this->value = simulate();
}



template <class valueType>
void RevBayesCore::DirichletProcessPriorDistribution<valueType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
//    if (oldP == baseDistribution){
//        baseDistribution = static_cast<const TypedDagNode< TypedDistribution<valueType> >* >( newP );
//    }
    if (oldP == concentration){
        concentration = static_cast<const TypedDagNode< double > *>( newP );
    }
    
}

template <class valueType>
std::vector<std::set<int> > RevBayesCore::DirichletProcessPriorDistribution<valueType>::getTableSeating(void){
	
	std::vector<std::set<int> > rest(numTables);
	for(int i=0; i<numElements; i++){
		rest[allocationVector[i]].insert(i);
	}
	return rest;
}

template <class valueType>
std::vector<valueType> RevBayesCore::DirichletProcessPriorDistribution<valueType>::getTableParameters(void){
	
	return valuePerTable;
}

template <class valueType>
std::vector<int> RevBayesCore::DirichletProcessPriorDistribution<valueType>::getElementAllocationVec(void){
	
	return allocationVector;
}

template <class valueType>
void RevBayesCore::DirichletProcessPriorDistribution<valueType>::createRestaurantVectors(void){
	
	std::vector<valueType>& pv = *this->value;
	valuePerTable.clear();
	numCustomerPerTable.clear();
	numTables = 0;
	for(int i=0; i<numElements; i++){
		valueType v = pv[i];
		size_t tID = std::find (valuePerTable.begin(), valuePerTable.end(), v) - valuePerTable.begin();
		if(tID < valuePerTable.size()){
			numCustomerPerTable[tID] += 1;
		}
		else{
			valuePerTable.push_back(v);
			numCustomerPerTable.push_back(1);
			numTables++;
		}
	allocationVector[i] = (int)tID;
	}
}

template <class valueType>
std::vector<int> RevBayesCore::DirichletProcessPriorDistribution<valueType>::getNumElemPerTable(void) {
	
	return numCustomerPerTable;
}

template <class valueType>
double RevBayesCore::DirichletProcessPriorDistribution<valueType>::getConcentrationParam(void) {

	return concentration->getValue();
}

template <class valueType>
RevBayesCore::TypedDistribution<valueType>* RevBayesCore::DirichletProcessPriorDistribution<valueType>::getBaseDistribution(void) {

	return baseDistribution;
}


#endif

