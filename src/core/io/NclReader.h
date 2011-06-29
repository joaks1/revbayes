/**
 * @file
 * This file contains the declaration of NclReader, which is the wrapper class for the NCL 
 * for reading character matrices, sequences, and trees.
 
 * @brief Declaration of NclReader
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-11-19 17:29:33 +0100 (Tor, 19 Nov 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id:$
 */

#ifndef NclReader_H
#define NclReader_H

#include "ncl.h"
#include "nxsmultiformat.h"
#include "TreeNode.h"
#include <map>
#include <set>
#include <string>
#include <vector>

class CharacterMatrix;
class Tree;

class NclReader {
    
        friend class NxsBlock;
        
    public:
                                            NclReader(void);                                                                //!< Default constructor
        virtual                            ~NclReader(void);                                                                //!< Destructor
        
        void                                addWarning(std::string s) { warningsSummary.insert(s); }
        void                                clearWarnings(void) { warningsSummary.clear(); }
        size_t                              getNumWarnings(void) { return warningsSummary.size(); }
        std::set<std::string>&              getWarnings(void) { return warningsSummary; }
        static NclReader&                   getInstance(void);
        std::vector<CharacterMatrix*>       readMatrices(const std::map<std::string,std::string>& fileMap);
        std::vector<CharacterMatrix*>       readMatrices(const std::vector<std::string> fn, const std::string fileFormat, const std::string dataType, const bool isInterleaved);
        
        // TAH: stuff for reading trees
        std::vector<Tree*>*                 readTrees(const std::string fn, const std::string fileFormat);
        void                                clearContent(void) { nexusReader.ClearContent(); }
        
    private:
        CharacterMatrix*                    createAminoAcidMatrix(NxsCharactersBlock* charblock);
        CharacterMatrix*                    createContinuousMatrix(NxsCharactersBlock* charblock);
        CharacterMatrix*                    createDnaMatrix(NxsCharactersBlock* charblock);
        CharacterMatrix*                    createRnaMatrix(NxsCharactersBlock* charblock);
        CharacterMatrix*                    createStandardMatrix(NxsCharactersBlock* charblock);
        bool                                fileExists(const char *fn) const;
        
        // methods for reading sequence alignments
        std::vector<CharacterMatrix*>       convertFromNcl(std::vector<std::string>& fnv);
        std::vector<CharacterMatrix*>       readMatrices(const char* fileName, const std::string fileFormat, const std::string dataType, const bool isInterleaved);
        
        // methods for reading trees
        void                                constructTreefromNclRecursively(TreeNode *tn, const NxsSimpleNode* tnNcl);
        std::vector<Tree*>*                 readTrees(const char* fileName, const std::string fileFormat);
        std::vector<Tree*>*                 convertTreesFromNcl(void);
        Tree*                               translateNclSimpleTreeToTree(NxsSimpleTree &nTree);
        
        MultiFormatReader                   nexusReader;
        std::set<std::string>               warningsSummary;
};

#endif