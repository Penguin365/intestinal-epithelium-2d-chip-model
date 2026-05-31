// AI-generated experimental module. Original project files remain unchanged.
#ifndef EPITHELIALCELLCYCLETRACKINGMODIFIER_HPP_
#define EPITHELIALCELLCYCLETRACKINGMODIFIER_HPP_

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>
#include "AbstractCellBasedSimulationModifier.hpp"

/**
 * Modifier that tracks epithelial-only cell cycle phase counts,
 * height statistics, inhibition state, and neighbour counts.
 *
 * Output file: epithelial_cellcycle.dat
 * Columns: time  epi_count  epi_G0  epi_G1  epi_S  epi_G2  epi_M
 *          n_inhibited  h_mean  h_max  h_std  mean_q_dur  mean_epi_neighbours
 */
template<unsigned DIM>
class EpithelialCellCycleTrackingModifier : public AbstractCellBasedSimulationModifier<DIM>
{
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellBasedSimulationModifier<DIM> >(*this);
        archive & mSamplingInterval;
    }

    out_stream mpOutputFile;
    unsigned mSamplingInterval;
    unsigned mTimestepCounter;

public:
    EpithelialCellCycleTrackingModifier();
    virtual ~EpithelialCellCycleTrackingModifier();

    void SetSamplingInterval(unsigned interval);
    unsigned GetSamplingInterval() const;

    virtual void UpdateAtEndOfTimeStep(AbstractCellPopulation<DIM, DIM>& rCellPopulation);
    virtual void SetupSolve(AbstractCellPopulation<DIM, DIM>& rCellPopulation, std::string outputDirectory);
    virtual void UpdateAtEndOfSolve(AbstractCellPopulation<DIM, DIM>& rCellPopulation);

    void WriteData(AbstractCellPopulation<DIM, DIM>& rCellPopulation);

    void OutputSimulationModifierParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(EpithelialCellCycleTrackingModifier)

#endif // EPITHELIALCELLCYCLETRACKINGMODIFIER_HPP_
