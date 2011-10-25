/**
 * @file
 * This file contains the declaration of RbObject, which is
 * the RevBayes abstract base class for all language objects.
 *
 * @brief Declaration of RbObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbObject.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef RbObject_H
#define RbObject_H

#include "RbPtr.h"
#include "TypeSpec.h"

#include <ostream>
#include <string>


class DAGNode;
class VectorInteger;
class VectorString;
class XmlDocument;
class XmlElement;

const std::string RbObject_name = "Object";

class RbObject {

    public:
        virtual                            ~RbObject(void);                                                         //!< Virtual destructor

        // Basic utility functions you have to override (also getClass()!)
        virtual RbObject*                   clone(void) const = 0;                                                  //!< Clone object
        virtual const RbPtr<XmlElement>     encode(RbPtr<XmlDocument> doc, const std::string& name);                //!< Function to encode this object into an XML string
        virtual const VectorString&         getClass(void) const;                                                   //!< Get class vector
        virtual const TypeSpec&             getTypeSpec(void) const = 0;                                            //!< Get language type of the object

        // Basic utility functions you may want to override
        virtual bool                        allowsVariableInsertion(void) const { return false; }                   //!< Do we allow variable to be inserted in this object (only appicable for some container, e.g. DagNodeContainer)
        virtual RbObject*                   convertTo(const TypeSpec& type) const;                                  //!< Convert to type
        virtual RbPtr<RbObject>             getElement(size_t index) const;                                         //!< Get element or subcontainer

        virtual bool                        isConvertibleTo(const TypeSpec& type) const;                            //!< Is convertible to type?
        

        // Basic utility functions you should not have to override
        const std::string&                  getType(void) const;                                                    //!< Get type of object
        bool                                isTypeSpec(const TypeSpec& typeSpec) const;                             //!< Does the language type of the object fit type specification typeSpec?

protected:
    RbObject(void);                                                                                     //!< No objects of this class
    
};
#endif
