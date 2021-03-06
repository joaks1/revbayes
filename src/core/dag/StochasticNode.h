#ifndef StochasticNode_H
#define StochasticNode_H

#include "DynamicNode.h"

namespace RevBayesCore {
    
    
    template <class valueType>
    class TypedDistribution;
    
    template <class valueType>
    class StochasticNode : public DynamicNode<valueType> {
        
    public:
        
        StochasticNode(const std::string &n, TypedDistribution<valueType> *d);
        StochasticNode(const StochasticNode<valueType> &n);                                                                             //!< Copy constructor
        
        virtual                                            ~StochasticNode(void);                                                       //!< Virtual destructor
        
        // Assignment operator
        StochasticNode&                                     operator=(const StochasticNode& n);                                         //!< Assignment operator
        
        // Basic utility function
        virtual StochasticNode<valueType>*                  clone(void) const;
        
        // methods
        void                                                bootstrap(void);                                                            //!< Bootstrap the current value of the node (applies only to stochastic nodes)
        void                                                clamp(valueType *val);                                                      //!< Clamp an observation to this random variable
        virtual TypedDistribution<valueType>&               getDistribution(void);
        virtual const TypedDistribution<valueType>&         getDistribution(void) const;
        virtual double                                      getLnProbability(void);
        virtual double                                      getLnProbabilityRatio(void);
        valueType&                                          getValue(void);
        const valueType&                                    getValue(void) const;
        bool                                                isClamped(void) const;                                                      //!< Is this DAG node clamped?
        bool                                                isStochastic(void) const;                                                   //!< Is this DAG node stochastic?
        void                                                printStructureInfo(std::ostream &o, bool verbose=false) const;              //!< Print the structural information (e.g. name, value-type, distribution/function, children, parents, etc.)
        void                                                redraw(void);                                                               //!< Redraw the current value of the node (applies only to stochastic nodes)
        virtual void                                        reInitializeMe(void);                                                       //!< The DAG was re-initialized so maybe you want to reset some stuff (delegate to distribution)
        virtual void                                        setClamped(bool tf);                                                        //!< Set directly the flag whether this node is clamped.
        void                                                setIgnoreRedraw(bool tf=true);
        void                                                setMcmcMode(bool tf);                                                       //!< Set the modus of the DAG node to MCMC mode.
        virtual void                                        setValue(valueType *val, bool touch=true);                                  //!< Set the value of this node
        void                                                setValueFromFile(const std::string &dir);                                   //!< Set value from string.
        void                                                setValueFromString(const std::string &v);                                   //!< Set value from string.
        void                                                unclamp(void);                                                              //!< Unclamp the variable
        
        // Parent DAG nodes management functions
        std::vector<const DagNode*>                         getParents(void) const;                                                     //!< Get the set of parents
        void                                                swapParent(const DagNode *oldP, const DagNode *newP);                       //!< Exchange the parent (distribution parameter)
        
    protected:
        
        virtual void                                        getAffected(RbOrderedSet<DagNode *>& affected, DagNode* affecter);          //!< Mark and get affected nodes
        virtual void                                        keepMe(DagNode* affecter);                                                  //!< Keep value of this and affected nodes
        virtual void                                        restoreMe(DagNode *restorer);                                               //!< Restore value of this nodes
        virtual void                                        setActivePIDSpecialized(size_t i, size_t n);                                          //!< Set the number of processes for this class.
        virtual void                                        touchMe(DagNode *toucher, bool touchAll);                                   //!< Tell affected nodes value is reset
        
        // protected members
        bool                                                clamped;
        bool                                                ignore_redraw;
        double                                              lnProb;                                                                     //!< Current log probability
        bool                                                needs_probability_recalculation;                                              //!< Do we need recalculation of the ln prob?
        double                                              stored_ln_prob;
        TypedDistribution<valueType>*                       distribution;
        
    };
}


#include "RbConstants.h"
#include "RbOptions.h"
#include "RbMathLogic.h"
#include "TypedDistribution.h"


template<class valueType>
RevBayesCore::StochasticNode<valueType>::StochasticNode( const std::string &n, TypedDistribution<valueType> *d ) : DynamicNode<valueType>( n ),
    clamped( false ),
    ignore_redraw(false),
    lnProb( RbConstants::Double::neginf ),
    needs_probability_recalculation( true ),
    distribution( d )
{
    this->type = DagNode::STOCHASTIC;
    
    // Get the parameters from the distribution and add us as a child of them in the DAG
    const std::vector<const DagNode*>& distParents = distribution->getParameters();
    for (std::vector<const DagNode*>::const_iterator it = distParents.begin(); it != distParents.end(); ++it)
    {
        (*it)->addChild( this );
        
        // Increment the reference count
        // We don't want this parent to get deleted while we are still alive
        (*it)->incrementReferenceCount();
    }
    
    // Set us as the DAG node of the distribution
    distribution->setStochasticNode( this );
}



template<class valueType>
RevBayesCore::StochasticNode<valueType>::StochasticNode( const StochasticNode<valueType> &n ) : DynamicNode<valueType>( n ),
    clamped( n.clamped ),
    ignore_redraw( n.ignore_redraw ),
    needs_probability_recalculation( true ),
    distribution( n.distribution->clone() )
{
    this->type = DagNode::STOCHASTIC;
    
    // Get the parameters from the distribution and add us as a child of them in the DAG
    const std::vector<const DagNode*>& distParents = distribution->getParameters();
    for (std::vector<const DagNode*>::const_iterator it = distParents.begin(); it != distParents.end(); ++it)
    {
        (*it)->addChild( this );
        
        // Increment the reference count
        // We don't want this parent to get deleted while we are still alive
        (*it)->incrementReferenceCount();
    }
    
    // Set us as the DAG node of the distribution
    distribution->setStochasticNode( this );
}


template<class valueType>
RevBayesCore::StochasticNode<valueType>::~StochasticNode( void )
{
    
    // Remove us as the child of the distribution parameters
    std::vector<const DagNode*> distParents = distribution->getParameters();
    for (std::vector<const DagNode*>::const_iterator it = distParents.begin(); it != distParents.end(); ++it)
    {
        (*it)->removeChild( this );
        
        // Decrement the reference count and check whether we need to delete the DAG node
        if ( (*it)->decrementReferenceCount() == 0)
        {
            delete (*it);
        }
        
    }
    
    delete distribution;
    
}


/**
 * Assignment operator. Make sure we deal with parent nodes correctly here.
 */
template<class valueType>
RevBayesCore::StochasticNode<valueType>& RevBayesCore::StochasticNode<valueType>::operator=( const StochasticNode<valueType>& n )
{
    
    if ( &n != this )
    {
        // Call base class assignment operator
        DynamicNode<valueType>::operator=( n );
        
        // Remove us as the child of the distribution parameters
        const std::vector<const DagNode*>& dist_parents = distribution->getParameters();
        for (std::vector<const DagNode*>::const_iterator it = dist_parents.begin(); it != dist_parents.end(); ++it)
        {
            (*it)->removeChild( this );
            
            // Decrement the reference count and check whether we need to delete the DAG node
            if ( (*it)->decrementReferenceCount() == 0)
            {
                delete (*it);
            }
            
        }
        
        // Delete the distribution
        delete distribution;
        
        // Recreate the distribution
        distribution = n.distribution->clone();
        
        // Get the parameters from the new distribution and add us as child of them in the DAG
        const std::vector<const DagNode*>& new_dist_parents = distribution->getParameters();
        for (std::vector<const DagNode*>::const_iterator it = new_dist_parents.begin(); it != new_dist_parents.end(); ++it)
        {
            (*it)->addChild( this );
            
            // Increment the reference count
            // We don't want this parent to get deleted while we are still alive
            (*it)->incrementReferenceCount();
        }
        
        // Set us as the DAG node of the new distribution
        distribution->setStochasticNode( this );
        
        clamped                             = n.clamped;
        ignore_redraw                       = n.ignore_redraw;
        needs_probability_recalculation     = true;
    }
    
    return *this;
}


template<class valueType>
void RevBayesCore::StochasticNode<valueType>::bootstrap( void )
{
    
    distribution->bootstrap();
    
    // touch this node for probability recalculation
    this->touch();
    
}


template<class valueType>
void RevBayesCore::StochasticNode<valueType>::clamp(valueType *val)
{
    // clamp the node with the value
    // we call set value because some derived classes might have special implementations for setting values (e.g. mixtures)
    setValue( val );
    
    clamped = true;
    
}



template<class valueType>
RevBayesCore::StochasticNode<valueType>* RevBayesCore::StochasticNode<valueType>::clone( void ) const
{
    
    return new StochasticNode<valueType>( *this );
}



/**
 * Get the affected stochastic nodes. We keep track of who issued the call (affecter). The
 * implementation simply inserts this node in the set of affected nodes. The call is not
 * passed on to the children, because the likelihood of descendant stochastic nodes is not
 * affected unless the call comes from this node. In the latter case, the call originates
 * in the getAffectedNodes of this node, and passes on directly to the children
 * without inserting this node in the set of affected nodes. See the DagNode base class for
 * the implementation of getAffectedNodes(...).
 */
template<class valueType>
void RevBayesCore::StochasticNode<valueType>::getAffected( RbOrderedSet<DagNode*>& affected, DagNode* affecter )
{
    
    // Insert this node as one of the affected
    affected.insert( this );
    
    // Call the distribution for potential specialized handling (e.g. internal flags)
    distribution->getAffected( affected, affecter );
    
}


template<class valueType>
RevBayesCore::TypedDistribution<valueType>& RevBayesCore::StochasticNode<valueType>::getDistribution( void ) {
    
    return *distribution;
}


template<class valueType>
const RevBayesCore::TypedDistribution<valueType>& RevBayesCore::StochasticNode<valueType>::getDistribution( void ) const
{
    
    return *distribution;
}


template<class valueType>
double RevBayesCore::StochasticNode<valueType>::getLnProbability( void )
{
    
    if ( needs_probability_recalculation )
    {
        // compute and store log-probability
        if ( this->prior_only == false || this->clamped == false )
        {
            lnProb = distribution->computeLnProbability();
        }
        else
        {
            lnProb = 0.0;
        }
        
        // reset flag
        needs_probability_recalculation = false;
    }
    
    return lnProb;
}


template<class valueType>
double RevBayesCore::StochasticNode<valueType>::getLnProbabilityRatio( void )
{
    
    return getLnProbability() - stored_ln_prob;
}


/**
 * Get the parents of this node. Simply ask the distribution to provide its parameters,
 * no need to keep parents here.
 */
template<class valueType>
std::vector<const RevBayesCore::DagNode*> RevBayesCore::StochasticNode<valueType>::getParents( void ) const
{
    return distribution->getParameters();
}


template<class valueType>
valueType& RevBayesCore::StochasticNode<valueType>::getValue( void )
{
    
    return distribution->getValue();
}


template<class valueType>
const valueType& RevBayesCore::StochasticNode<valueType>::getValue( void ) const
{
    
    return distribution->getValue();
}


template<class valueType>
bool RevBayesCore::StochasticNode<valueType>::isClamped( void ) const
{
    
    return clamped;
}


template<class valueType>
bool RevBayesCore::StochasticNode<valueType>::isStochastic( void ) const
{
    
    return true;
}


/**
 * Keep the current value of the node.
 * At this point, we also need to make sure we update the stored ln probability.
 */
template<class valueType>
void RevBayesCore::StochasticNode<valueType>::keepMe( DagNode* affecter )
{
    
    if ( this->touched == true )
    {
        
        stored_ln_prob = 1.0E6;       // An almost impossible value for the density
        if ( needs_probability_recalculation )
        {
            if ( this->prior_only == false || this->clamped == false )
            {
                lnProb = distribution->computeLnProbability();
            }
            else
            {
                lnProb = 0.0;
            }
        }
        
        distribution->keep( affecter );
        
        // clear the list of touched element indices
        this->touched_elements.clear();
        
    }
    
    needs_probability_recalculation   = false;
    
    
    // delegate call
    DynamicNode<valueType>::keepMe( affecter );
    
}


template<class valueType>
void RevBayesCore::StochasticNode<valueType>::printStructureInfo( std::ostream &o, bool verbose ) const
{
    
    if ( verbose == true )
    {
        o << "_dagNode      = " << this->name << " <" << this << ">" << std::endl;
    }
    
    o << "_dagType      = Stochastic DAG node" << std::endl;
    
    if ( verbose == true )
    {
        o << "_refCount     = " << this->getReferenceCount() << std::endl;
        o << "_distribution = " << "<" << distribution << ">" << std::endl;
        o << "_touched      = " << ( this->touched ? "TRUE" : "FALSE" ) << std::endl;
    }
    
    o << "_clamped      = " << ( clamped ? "TRUE" : "FALSE" ) << std::endl;
    o << "_lnProb       = " << const_cast< StochasticNode<valueType>* >( this )->getLnProbability() << std::endl;
    
    if ( this->touched == true && verbose == true)
    {
        o << "_stored_ln_prob = " << stored_ln_prob << std::endl;
    }
    o << "_parents      = ";
    this->printParents(o, 16, 70, verbose);
    o << std::endl;
    
    o << "_children     = ";
    this->printChildren(o, 16, 70, verbose);
    o << std::endl;
}


template<class valueType>
void RevBayesCore::StochasticNode<valueType>::redraw( void )
{
    
    // draw the value
    if ( ignore_redraw == false )
    {
        distribution->redrawValue();
    }
    
    // touch this node for probability recalculation
    this->touch();
    
}


template<class valueType>
void RevBayesCore::StochasticNode<valueType>::reInitializeMe( void )
{
    
    distribution->reInitialized();
    
}


/** Restore the old value of the node and tell affected */
template<class valueType>
void RevBayesCore::StochasticNode<valueType>::restoreMe(DagNode *restorer)
{
    
    if ( this->touched == true )
    {
        lnProb              = stored_ln_prob;
        stored_ln_prob      = 1.0E6;    // An almost impossible value for the density
        
        // reset flags that recalculation is not needed
        needs_probability_recalculation = false;
        
        // call for potential specialized handling (e.g. internal flags)
        distribution->restore(restorer);
        
        // clear the list of touched element indices
        this->touched_elements.clear();
        
    }
    
    // delegate call
    DynamicNode<valueType>::restoreMe( restorer );
    
}


/**
 * Set the active PID of this specific DAG node object.
 */
template <class valueType>
void RevBayesCore::StochasticNode<valueType>::setActivePIDSpecialized(size_t a, size_t n)
{
    
    if ( distribution != NULL )
    {
        distribution->setActivePID( a, n );
    }
    
}


/**
 * Set directly the flag whether this node is clamped.
 * The caller needs to be responsible enough to know that we will assume
 * that the current value is the observed value.
 * We could use instead as well a call: clamp( getValue() );
 */
template<class valueType>
void RevBayesCore::StochasticNode<valueType>::setClamped(bool tf)
{
    
    clamped = tf;
    
}



template<class valueType>
void RevBayesCore::StochasticNode<valueType>::setMcmcMode(bool tf)
{
    
    distribution->setMcmcMode( tf );
    
}



/**
 * Set the value.
 */
template<class valueType>
void RevBayesCore::StochasticNode<valueType>::setValue(valueType *val, bool force_touch)
{
    // set the value
    distribution->setValue( val, true );
    
    if ( force_touch == true )
    {
        // touch this node for probability recalculation
        this->touch();
    }
    
}


template<class valueType>
void RevBayesCore::StochasticNode<valueType>::setValueFromFile(const std::string &dir)
{
    
    Serializer<valueType, IsDerivedFrom<valueType, RevBayesCore::Serializable>::Is >::ressurectFromFile( &getValue(), dir, this->getName() );
    
    // delegate to the standard function of setting the value
    this->setValue( &this->getValue() );
    
}


template<class valueType>
void RevBayesCore::StochasticNode<valueType>::setValueFromString(const std::string &v)
{
    
    Serializer<valueType, IsDerivedFrom<valueType, RevBayesCore::Serializable>::Is >::ressurectFromString( &getValue(), v );
    
    // delegate to the standard function of setting the value
    this->setValue( &this->getValue() );
    
}


template <class valueType>
void RevBayesCore::StochasticNode<valueType>::setIgnoreRedraw( bool tf )
{
    ignore_redraw = tf;

}


/**
 * This function replaces the earlier swapParameter function. If we rely on the
 * internal RevBayesCore::Distribution to manage our parents, we simply need to ask
 * the distribution to swap its parameter, and then manage the connection of the
 * old and new parents (parameters) to this node.
 */
template <class valueType>
void RevBayesCore::StochasticNode<valueType>::swapParent( const RevBayesCore::DagNode *oldParent, const RevBayesCore::DagNode *newParent )
{
    // We are sure to get into trouble if either one of these is NULL
    if ( oldParent == NULL || newParent == NULL )
    {
        throw RbException( "Attempt to swap NULL distribution parameter of RevBayesCore::StochasticNode" );
    }
    
    // This throws an error if the oldParent cannot be found
    distribution->swapParameter( oldParent, newParent );
    
    oldParent->removeChild( this );
    if ( oldParent->decrementReferenceCount() == 0 )
    {
        delete ( oldParent );
    }
    
    newParent->addChild( this );
    newParent->incrementReferenceCount();
    
    this->touch();
}


/** touch this node for recalculation */
template<class valueType>
void RevBayesCore::StochasticNode<valueType>::touchMe( DagNode *toucher, bool touchAll )
{
    
    if ( this->touched == false )
    {
        stored_ln_prob = lnProb;
    }
    
    needs_probability_recalculation = true;
    
    // call for potential specialized handling (e.g. internal flags), we might have been touched already by someone else, so we need to delegate regardless
    distribution->touch( toucher, touchAll );
    
    // delegate call
    DynamicNode<valueType>::touchMe( toucher, touchAll );
}


/**
 * Unclamp this node. If I understand this correctly, we
 * can just set the clamped flag to false if we keep the
 * current value. We do not need to tell anyone that we
 * have changed value because we really haven't.
 */
template<class valueType>
void RevBayesCore::StochasticNode<valueType>::unclamp( void )
{
    clamped = false;
}


#endif

