// AI-generated experimental module. Original project files remain unchanged.
#ifndef UNIVERSAL_NEIGHBOUR_TRACKING_MODIFIER_HPP_
#define UNIVERSAL_NEIGHBOUR_TRACKING_MODIFIER_HPP_

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>
#include "AbstractCellBasedSimulationModifier.hpp"

template<unsigned DIM>
class UniversalNeighbourTrackingModifier : public AbstractCellBasedSimulationModifier<DIM>
{
private:
    double mInteractionRadius;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellBasedSimulationModifier<DIM> >(*this);
        archive & mInteractionRadius;
    }

public:
    UniversalNeighbourTrackingModifier();
    virtual ~UniversalNeighbourTrackingModifier() {}

    void SetInteractionRadius(double radius);
    double GetInteractionRadius() const;

    void UpdateAtEndOfTimeStep(AbstractCellPopulation<DIM, DIM>& rCellPopulation) override;
    void SetupSolve(AbstractCellPopulation<DIM, DIM>& rCellPopulation, std::string outputDirectory) override;
    void OutputSimulationModifierParameters(out_stream& rParamsFile) override;
};

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(UniversalNeighbourTrackingModifier)

#endif // UNIVERSAL_NEIGHBOUR_TRACKING_MODIFIER_HPP_
