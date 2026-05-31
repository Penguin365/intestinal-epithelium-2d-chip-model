// AI-generated experimental module. Original project files remain unchanged.
#ifndef CONFIGURABLE_CONTACT_INHIBITION_CELL_CYCLE_MODEL_HPP_
#define CONFIGURABLE_CONTACT_INHIBITION_CELL_CYCLE_MODEL_HPP_

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>
#include "AbstractSimplePhaseBasedCellCycleModel.hpp"
#include "SimulationTime.hpp"

class ConfigurableContactInhibitionCellCycleModel : public AbstractSimplePhaseBasedCellCycleModel
{
private:
    bool mUseNeighbourInhibition;
    bool mUseHeightInhibition;
    bool mAllowHardCellDivision;
    unsigned mNeighbourThreshold;
    double mHeightThreshold;
    double mTargetProportion;
    double mCurrentQuiescentOnsetTime;
    double mCurrentQuiescentDuration;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractSimplePhaseBasedCellCycleModel>(*this);
        archive & mUseNeighbourInhibition;
        archive & mUseHeightInhibition;
        archive & mAllowHardCellDivision;
        archive & mNeighbourThreshold;
        archive & mHeightThreshold;
        archive & mTargetProportion;
        archive & mCurrentQuiescentOnsetTime;
        archive & mCurrentQuiescentDuration;
    }

protected:
    ConfigurableContactInhibitionCellCycleModel(const ConfigurableContactInhibitionCellCycleModel& rModel);

public:
    ConfigurableContactInhibitionCellCycleModel();
    virtual ~ConfigurableContactInhibitionCellCycleModel() {}

    void SetUseNeighbourInhibition(bool useNeighbourInhibition);
    bool GetUseNeighbourInhibition() const;

    void SetUseHeightInhibition(bool useHeightInhibition);
    bool GetUseHeightInhibition() const;

    void SetNeighbourThreshold(unsigned neighbourThreshold);
    unsigned GetNeighbourThreshold() const;

    void SetHeightThreshold(double heightThreshold);
    double GetHeightThreshold() const;

    void SetAllowHardCellDivision(bool allowHardCellDivision);
    bool GetAllowHardCellDivision() const;

    void SetTargetProportion(double targetProportion);
    double GetTargetProportion() const;

    double GetCurrentQuiescentDuration() const;
    double GetCurrentQuiescentOnsetTime() const;

    void SetG1Duration();
    void UpdateCellCyclePhase();
    void InitialiseDaughterCell();
    void ResetForDivision();
    AbstractCellCycleModel* CreateCellCycleModel();

    void OutputCellCycleModelParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(ConfigurableContactInhibitionCellCycleModel)

#endif // CONFIGURABLE_CONTACT_INHIBITION_CELL_CYCLE_MODEL_HPP_
