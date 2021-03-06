#ifndef CorrelationMatrixExpansionProposal_H
#define CorrelationMatrixExpansionProposal_H

#include <set>
#include <string>

#include "Proposal.h"
#include "MatrixReal.h"
#include "StochasticNode.h"
#include "PhyloMultivariateBrownianProcessREML.h"
#include "ContinuousCharacterData.h"

namespace RevBayesCore {
    
    /**
     * The Beta operator.
     *
     * This move randomly picks an element of a matrix of positive real numbers.
     * That means, that we randomly pick the i-th row and j-th column with equal probability.
     * Then, we propose a Beta distance and slide the value.
     * The actual Beta distance is computed by delta = lambda * ( u - 0.5 )
     * where u ~ Uniform(0,1).
     * The proposal is thus m[i][j] += lambda * ( u - 0.5 )
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Michael R. May)
     * @since 2009-09-08, version 1.0
     *
     */
    class CorrelationMatrixExpansionProposal : public Proposal {
        
    public:
        CorrelationMatrixExpansionProposal( StochasticNode<MatrixReal> *cm, TypedDagNode<RbVector<double> > *v, StochasticNode<ContinuousCharacterData>* mv);                                                                      //!<  constructor
        
        // Basic utility functions
        void                                     cleanProposal(void);                                                                //!< Clean up proposal
        CorrelationMatrixExpansionProposal*      clone(void) const;                                                                  //!< Clone object
        double                                   doProposal(void);                                                                   //!< Perform proposal
        const std::string&                       getProposalName(void) const;                                                        //!< Get the name of the proposal for summary printing
        void                                     printParameterSummary(std::ostream &o) const;                                       //!< Print the parameter summary
        void                                     prepareProposal(void);                                                              //!< Prepare the proposal
        void                                     tune(double r);                                                                     //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                     undoProposal(void);                                                                 //!< Reject the proposal
        
    protected:
        void                                     swapNodeInternal(DagNode *oldN, DagNode *newN);                                     //!< Swap the DAG nodes on which the Proposal is working on
        
    private:
        // parameters
        
        StochasticNode<MatrixReal>*                correlation_matrix;
        StochasticNode<ContinuousCharacterData>*   mvbm;
        TypedDagNode<RbVector<double> >*           variance;
        
        // stored objects to undo proposal
        MatrixReal                                 stored_matrix;
    };
    
}

#endif

