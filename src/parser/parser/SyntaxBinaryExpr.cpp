/**
 * @file
 * This file contains the implementation of SyntaxBinaryExpr, which is
 * used to hold binary expressions in the syntax tree.
 *
 * @brief Implementation of SyntaxBinaryExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include <iostream>
#include <list>
#include <sstream>
#include <vector>

#include "Argument.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "RbUtil.h"
#include "SyntaxBinaryExpr.h"
#include "VectorString.h"
#include "Workspace.h"



/** Static vector of strings giving names of operator types */
std::string SyntaxBinaryExpr::opCode[] = { "range", "add", "sub", "mul", "div", "exp", "lt", "le",
                                           "eq", "ne", "ge", "gt", "and", "or", "and", "or"};


/** Construct from operator type and operands */
SyntaxBinaryExpr::SyntaxBinaryExpr(operatorT op, SyntaxElement* lhs, SyntaxElement* rhs) : SyntaxElement(), leftOperand(lhs), rightOperand(rhs), operation(op) {

}


/** Deep copy constructor */
SyntaxBinaryExpr::SyntaxBinaryExpr(const SyntaxBinaryExpr& x) : SyntaxElement(x) {

    leftOperand  = x.leftOperand->clone();
    rightOperand = x.rightOperand->clone();
    operation    = x.operation;
}


/** Destructor deletes operands */
SyntaxBinaryExpr::~SyntaxBinaryExpr() {
    
}


/** Assignment operator */
SyntaxBinaryExpr& SyntaxBinaryExpr::operator=(const SyntaxBinaryExpr& x) {

    if (&x != this) {

        SyntaxElement::operator=(x);

        leftOperand  = x.leftOperand->clone();
        rightOperand = x.rightOperand->clone();
        operation    = x.operation;
    }

    return *this;
}


/** Clone syntax element */
SyntaxElement* SyntaxBinaryExpr::clone () const {

    return (new SyntaxBinaryExpr(*this));
}


/** Get class name of object */
const std::string& SyntaxBinaryExpr::getClassName(void) { 
    
    static std::string rbClassName = "Binary expression";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SyntaxBinaryExpr::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( SyntaxElement::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SyntaxBinaryExpr::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/**
 * @brief Get semantic value
 *
 * We simply look up the function and calculate the value.
 *
 */
RbVariablePtr SyntaxBinaryExpr::evaluateContent( Environment& env) {

    // Package the arguments
    std::vector<Argument> args;
    RbVariablePtr left = leftOperand->evaluateContent(env);
    args.push_back( Argument("", left ) );
    RbVariablePtr right = rightOperand->evaluateContent(env);
    args.push_back( Argument("", right ) );

    // Get function and create deterministic DAG node
    std::string funcName = "_" + opCode[operation];
    
    RbFunction* theFunction = Workspace::globalWorkspace().getFunction(funcName, args);
    
    return RbVariablePtr( new Variable(new DeterministicNode( theFunction ) ) );
}


/** Print info about the syntax element */
void SyntaxBinaryExpr::printValue(std::ostream& o) const {

    o << "[" << this << "] SyntaxBinaryExpr:" << std::endl;
    o << "left operand  = [" << leftOperand  << "]";
    leftOperand->printValue(o);
    o << std::endl;
    o << "right operand = [" << rightOperand << "]" ;
    rightOperand->printValue(o);
    o << std::endl;
    o << "operation     = " << opCode[operation];
    o << std::endl;

    leftOperand->printValue(o);
    rightOperand->printValue(o);
}

