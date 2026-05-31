#ifndef PROLIFERATIVE_CELL_COLOUR_LABEL_HPP_
#define PROLIFERATIVE_CELL_COLOUR_LABEL_HPP_

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>
#include "AbstractCellProperty.hpp"

class ProliferativeCellColourLabel : public AbstractCellProperty
{
private:

    unsigned mColour;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellProperty>(*this);
        archive & mColour;
    }

public:
    /** Constructor **/
    ProliferativeCellColourLabel(unsigned colour = 1);

    /** Destructor **/
    ~ProliferativeCellColourLabel();

    unsigned GetColour() const;
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(ProliferativeCellColourLabel)

#endif // PROLIFERATIVE_CELL_COLOUR_LABEL_HPP_
