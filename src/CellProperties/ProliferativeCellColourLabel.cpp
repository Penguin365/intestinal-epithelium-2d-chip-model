#include "ProliferativeCellColourLabel.hpp"

ProliferativeCellColourLabel::ProliferativeCellColourLabel(unsigned colour)
    : AbstractCellProperty(),
      mColour(colour)
{
}

ProliferativeCellColourLabel::~ProliferativeCellColourLabel()
{
}

unsigned ProliferativeCellColourLabel::GetColour() const
{
    return mColour;
}
#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(ProliferativeCellColourLabel)