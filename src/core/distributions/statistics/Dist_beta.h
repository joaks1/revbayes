/**
 * @file
 * This file contains the declaration of Dist_beta, which is used to hold
 * parameters and functions related to an beta distribution.
 *
 * @brief Declaration of Dist_beta
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#ifndef Dist_beta_H
#define Dist_beta_H

#include "DistributionContinuous.h"
#include "RealPos.h"

#include <ostream>
#include <string>

class DAGNode;
class StochasticNode;
class VectorString;

const std::string Dist_beta_name = "Dist_beta";

class Dist_beta: public DistributionContinuous {

    public:
                                    Dist_beta(void);                                                    //!< Parser constructor

        // Basic utility functions
        Dist_beta*                  clone(void) const;                                                  //!< Clone object
        const VectorString&         getClass(void) const;                                               //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object

        // Member variable setup
        const RbPtr<MemberRules>    getMemberRules(void) const;                                         //!< Get member variable rules

        // Exponential distribution functions
        double                      cdf(const RbPtr<RbLanguageObject> value);                                 //!< Cumulative density
        const TypeSpec&             getVariableType(void) const;                                        //!< Get random variable type (RealPos)
        double                      lnPdf(const RbPtr<RbLanguageObject> value);                               //!< Ln probability density
        double                      pdf(const RbPtr<RbLanguageObject> value);                                 //!< Probability density
        RbPtr<Real>                 quantile(const double p);                                           //!< Quantile
        RbPtr<RbLanguageObject>     rv(void);                                                           //!< Generate random variable
    
    private:
        static const TypeSpec       typeSpec;
        static const TypeSpec       varTypeSpec;
};

#endif

