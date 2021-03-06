#include "CharacterHistoryContinuous.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "Tree.h"

using namespace RevBayesCore;


CharacterHistoryContinuous::CharacterHistoryContinuous(Tree *t, size_t nc, bool rb ) : CharacterHistory( t, nc, rb )
{
    
    if ( tree != NULL )
    {
        n_branches = tree->getNumberOfNodes() - 1 + ( use_root_branch ? 1 : 0 );
        // create a branch history object for each branch
        for (size_t i=0; i<n_branches; ++i)
        {
            histories.push_back( new BranchHistoryContinuous(n_character, i) );
        }
        
    }
    
}



/**
 * Dummy destructor.
 */
CharacterHistoryContinuous::~CharacterHistoryContinuous()
{
    
}


/**
 * Index operator.
 */
const BranchHistoryContinuous& CharacterHistoryContinuous::operator[](size_t i) const
{
    if ( i > histories.size() )
    {
        throw RbException("Index out of bounds in character history.");
    }
    
    return static_cast<const BranchHistoryContinuous &>( *histories[i] );
}


/**
 * Create a deep copy.
 */
CharacterHistoryContinuous* CharacterHistoryContinuous::clone( void ) const
{
    
    return new CharacterHistoryContinuous( *this );
}


/**
 * Pick a random event.
 */
CharacterEventContinuous* CharacterHistoryContinuous::pickRandomEvent( size_t &branch_index )
{
    
    return static_cast<CharacterEventContinuous*>( CharacterHistory::pickRandomEvent(branch_index) );
}
 

/**
 * Set the new tree.
 */
void CharacterHistoryContinuous::setTree(Tree *t)
{
    
    tree = t;
    if ( tree != NULL )
    {
        histories.clear();
        n_events = 0;
        n_branches = tree->getNumberOfNodes() - 1  + (use_root_branch ? 1 : 0);
        
        // create a branch history object for each branch
        for (size_t i=0; i<n_branches; ++i)
        {
            histories.push_back( new BranchHistoryContinuous(n_character, i) );
        }
        
    }
    
}
