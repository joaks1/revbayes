/**
 * @file
 * This file contains the declaration of DeterministicNode, which is derived
 * from DAGNode. DeterministicNode is used for DAG nodes associated with
 * an expression (equation) that determines their value.
 *
 * @brief Declaration of DeterministicNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:51:54 +0100 (Tis, 12 Jan 2010) $
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: DeterministicNode.h 221 2010-01-12 20:51:54Z Hoehna $
 */

#ifndef DeterministicNode_H
#define DeterministicNode_H

#include "VariableNode.h"

#include <string>
#include <vector>

class RbFunction;

class DeterministicNode : public VariableNode {

    public:
                                    DeterministicNode(const std::string& valType);                  //!< Constructor of empty node
                                    DeterministicNode(RbFunction* func);                            //!< Constructor from function
                                    DeterministicNode(const DeterministicNode& d);                  //!< Copy constructor
        virtual                    ~DeterministicNode(void);                                        //!< Virtual destructor

        // Assignment operator
        DeterministicNode&          operator=(const DeterministicNode& x);                          //!< Assignment operator

        // Basic utility functions
        virtual DeterministicNode*  clone(void) const;                                              //!< Clone the deterministic node
        virtual const VectorString& getClass(void) const;                                           //!< Get class vector
        const RbFunction*           getFunction(void) const { return function; }                    //!< Get function
        const RbObject*             getStoredValue(void);                                           //!< Get stored value intelligently
        const RbObject*             getValElement(const VectorInteger& index) const;                //!< Get value element
        const RbObject*             getValue(void);                                                 //!< Get current value intelligently
        const RbObject*             getValue(void) const;                                           //!< Get current value (const) if possible
        const VectorString&         getValueClass(void) const;                                      //!< Get value class
        virtual void                printStruct(std::ostream& o) const;                             //!< Print struct for user
        virtual void                printValue(std::ostream& o) const;                              //!< Print value for user
        void                        setElement(const VectorInteger& index, RbObject* value);        //!< Set value element
        void                        setValue(RbObject* value);                                      //!< Set value
        virtual std::string         toString(void) const;                                           //!< Complete info about object

        // DAG functions
        DeterministicNode*          cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const;         //!< Clone entire graph
        void    	                getAffected(std::set<StochasticNode*>& affected);               //!< Mark and get affected nodes
        virtual bool                isMutableTo(const DAGNode* newNode) const;                      //!< Is node mutable to newNode?
        virtual bool                isMutableTo(const VectorInteger& index, const RbObject* newValue) const;    //!< Is node mutable to contain newValue?
        virtual bool                isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const;    //!< Is parent mutable to newNode?
        void    	                keepAffected(void);                                             //!< Keep value of affected nodes
        virtual void                mutateTo(DAGNode* newNode);                                     //!< Mutate to new node
        DeterministicNode*          mutateTo(const VectorInteger& index, RbObject* newValue);       //!< Mutate to contain newValue
        void                        restoreAffected(void);                                          //!< Restore value of affected nodes
        void                        swapParentNode(DAGNode* oldP, DAGNode* newP);                   //!< Swap a parent node
        void                        touchAffected(void);                                            //!< Tell affected nodes value is reset

        // Move function
        MoveSchedule*               getDefaultMoves(void);                                         //!< Get default moves

    protected:
        // Utility function
        virtual void                update(void);                                                  //!< Update value and storedValue

        // Member variables
        bool                        changed;                                                       //!< True when value updated after touch
        RbFunction*                 function;                                                      //!< Function calculating value
        const RbObject*             value;                                                         //!< Ptr to current value
        RbObject*                   storedValue;                                                   //!< Stored value
};

#endif

