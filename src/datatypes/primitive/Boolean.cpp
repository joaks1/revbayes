/**
 * @file
 * This file contains the implementation of Boolean, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Implementation of Boolean
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */


#include "Boolean.h"
#include "Real.h"
#include "Integer.h"
#include "RbNames.h"
#include "VectorString.h"

#include <sstream>


/** Default constructor */
Boolean::Boolean(void) : RbObject(), value(false) {

}

/** Construct from bool */
Boolean::Boolean(const bool v) : RbObject(), value(v) {

}

/** Clone object */
RbObject* Boolean::clone(void) const {

	return  (RbObject*)(new Boolean(*this));
}


/** Convert to object of another class. The caller manages the object. */
RbObject* Boolean::convertTo(const std::string& type, int dim) const {

    if (type == Integer_name && dim == 0) {
        if (value)
            return new Integer(1);
        else 
            return new Integer(0);
    }
    else if (type == Real_name && dim == 0) {
        if (value)
            return new Real(1.0);
        else 
            return new Real(0.0);
    }

    return RbObject::convertTo(type, dim);
}

/** Pointer-based equals comparison */
bool Boolean::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const Boolean* p = dynamic_cast<const Boolean*>(obj);
    if (p != NULL)
        return value == p->value;

    // Try converting the object to a bool
    p = dynamic_cast<const Boolean*>(obj->convertTo(Boolean_name));
    if (p == NULL)
        return false;

    // Get result
    bool result = (value == p->value);
    delete p;
    return result;
}

/** Get class vector describing type of object */
const VectorString& Boolean::getClass() const {

    static VectorString rbClass = VectorString(Boolean_name) + RbObject::getClass();
    return rbClass;
}


/** Is convertible to type and dim? */
bool Boolean::isConvertibleTo(const std::string& type, int dim, bool once) const {

    if (type == Integer_name && dim == 0)
        return true;
    else if (type == Real_name && dim == 0)
        return true;

    return RbObject::isConvertibleTo(type, dim);
}


/** Print value for user */
void Boolean::printValue(std::ostream &o) const {

    o << (value ? "true" : "false");
}

/** Get complete info about object */
std::string Boolean::toString(void) const {

	std::ostringstream o;
    o << "Boolean: value = ";
    printValue(o);
    return o.str();
}

