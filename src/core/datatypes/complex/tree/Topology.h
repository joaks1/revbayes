/**
 * @file
 * This file contains the declaration of a Topology a light-weight class that holds the topology of a tree as a set of nodes.
 *
 * @brief Declaration of the Topology
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id$
 */

#ifndef Topology_H
#define Topology_H

#include "ConstantMemberObject.h"
#include <set>
#include <string>

class ArgumentEnvironment;
class ArgumentRule;
class DAGNode;
class MethodTable;
class TopologyNode;
class VectorString;


/** TopologyChange: A struct describing a topology change
    in terms of a parent shift */
struct TopologyChange {
    int node;
    int oldParentNode;
    int newParentNode;
};


class Topology: public ConstantMemberObject {
    
    public:
                                        Topology(void);                                                         //!< Default constructor
                                        Topology(const Topology& t);                                            //!< Copy constructor
                                       ~Topology(void);                                                         //!< Destructor

        // Basic utility functions
        Topology*                       clone(void) const;                                                      //!< Clone object
        const VectorString&             getClass(void) const;                                                   //!< Get class vector   
        void                            printValue(std::ostream& o) const;                                      //!< Print value for user
        std::string                     richInfo(void) const;                                                   //!< Complete info

        // Member variable rules
        const MemberRules&              getMemberRules(void) const;                                             //!< Get member rules

        // Member method inits
        RbLanguageObject*               executeOperation(const std::string& name, Environment& args);           //!< Execute method
        const MethodTable&              getMethods(void) const;                                                 //!< Get methods
        
        // Topology functions
        void                            changeTopology(std::vector<TopologyChange>& topChanges);                //!< Change the topology according to instructions
        TopologyNode*                   cloneTree(TopologyNode *parent);                                        //!< Deep copy of the nodes
        bool                            getIsBinary(void) const { return isBinary; }                            //!< Is the tree rooted
        bool                            getIsRooted(void) const { return isRooted; }                            //!< Is the tree rooted
        std::vector<TopologyNode*>&     getNodes(void) { return nodes; }                                        //!< Get a pointer to the nodes in the tree
        size_t                          getNumberOfInteriorNodes(void) const;                                   //!< Get the number of nodes in the tree
        size_t                          getNumberOfNodes(void) const { return nodes.size(); }                   //!< Get the number of nodes in the tree
        size_t                          getNumberOfTips(void) const;                                            //!< Get the number of tip nodes in the tree
        TopologyNode*                   getInteriorNode(int indx) const;                                        //!< Get a pointer to interior node i
        TopologyNode*                   getRoot(void) const { return root; }                                    //!< Get a pointer to the root node of the tree
        TopologyNode*                   getTipNode(size_t indx) const;                                          //!< Get a pointer to tip node i
        void                            setIsRooted(bool tf) { isRooted = tf; }                                 //!< Set the rootedness of the tree
        void                            setRoot(TopologyNode* r);                                               //!< Set the root and bootstrap the tree from it
        
	private:
        void                            fillNodesByPreorderTraversal(TopologyNode *node);                       //!< fill the nodes vector by a preorder traversal recursively starting with this node.
        std::string                     buildNewickString(TopologyNode *node);                                  //!< compute the newick string for a tree rooting at this node
    
        std::vector<TopologyNode*>      nodes;                                                                  //!< Vector of pointers to all nodes
        std::string                     newick;                                                                 //!< the newick string
        TopologyNode*                   root;                                                                   //!< Pointer to the root node
        bool                            isRooted;                                                               //!< Is the topology rooted?
        bool                            isBinary;                                                               //!< Is the topology binary?
};

#endif

