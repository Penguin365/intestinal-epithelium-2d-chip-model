#include "ConfigurableContactInhibitionCellCycleModel.hpp"
#include "Exception.hpp"
#include "SmartPointers.hpp"
#include "Cell.hpp"
#include "CellLabel.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "RandomNumberGenerator.hpp"
#include "WildTypeCellMutationState.hpp"
#include "HardCellMutationState.hpp"
#include "TransitCellProliferativeType.hpp"
#include "SimulationTime.hpp"

ConfigurableContactInhibitionCellCycleModel::ConfigurableContactInhibitionCellCycleModel()
    : AbstractSimplePhaseBasedCellCycleModel(),
      mUseNeighbourInhibition(true),
      mUseHeightInhibition(true),
      mAllowHardCellDivision(false),
      mNeighbourThreshold(6),
      mHeightThreshold(4.0),
      mTargetProportion(0.9),
      mCurrentQuiescentOnsetTime(SimulationTime::Instance()->GetTime()),
      mCurrentQuiescentDuration(0.0)
{
}

ConfigurableContactInhibitionCellCycleModel::ConfigurableContactInhibitionCellCycleModel(const ConfigurableContactInhibitionCellCycleModel& rModel)
    : AbstractSimplePhaseBasedCellCycleModel(rModel),
      mUseNeighbourInhibition(rModel.mUseNeighbourInhibition),
      mUseHeightInhibition(rModel.mUseHeightInhibition),
      mAllowHardCellDivision(rModel.mAllowHardCellDivision),
      mNeighbourThreshold(rModel.mNeighbourThreshold),
      mHeightThreshold(rModel.mHeightThreshold),
      mTargetProportion(rModel.mTargetProportion),
      mCurrentQuiescentOnsetTime(rModel.mCurrentQuiescentOnsetTime),
      mCurrentQuiescentDuration(rModel.mCurrentQuiescentDuration)
{
}

void ConfigurableContactInhibitionCellCycleModel::SetUseNeighbourInhibition(bool useNeighbourInhibition)
{
    mUseNeighbourInhibition = useNeighbourInhibition;
}

bool ConfigurableContactInhibitionCellCycleModel::GetUseNeighbourInhibition() const
{
    return mUseNeighbourInhibition;
}

void ConfigurableContactInhibitionCellCycleModel::SetUseHeightInhibition(bool useHeightInhibition)
{
    mUseHeightInhibition = useHeightInhibition;
}

bool ConfigurableContactInhibitionCellCycleModel::GetUseHeightInhibition() const
{
    return mUseHeightInhibition;
}

void ConfigurableContactInhibitionCellCycleModel::SetNeighbourThreshold(unsigned neighbourThreshold)
{
    mNeighbourThreshold = neighbourThreshold;
}

unsigned ConfigurableContactInhibitionCellCycleModel::GetNeighbourThreshold() const
{
    return mNeighbourThreshold;
}

void ConfigurableContactInhibitionCellCycleModel::SetHeightThreshold(double heightThreshold)
{
    mHeightThreshold = heightThreshold;
}

double ConfigurableContactInhibitionCellCycleModel::GetHeightThreshold() const
{
    return mHeightThreshold;
}

void ConfigurableContactInhibitionCellCycleModel::SetAllowHardCellDivision(bool allowHardCellDivision)
{
    mAllowHardCellDivision = allowHardCellDivision;
}

bool ConfigurableContactInhibitionCellCycleModel::GetAllowHardCellDivision() const
{
    return mAllowHardCellDivision;
}

void ConfigurableContactInhibitionCellCycleModel::SetTargetProportion(double targetProportion)
{
    mTargetProportion = targetProportion;
}

double ConfigurableContactInhibitionCellCycleModel::GetTargetProportion() const
{
    return mTargetProportion;
}

double ConfigurableContactInhibitionCellCycleModel::GetCurrentQuiescentDuration() const
{
    return mCurrentQuiescentDuration;
}

double ConfigurableContactInhibitionCellCycleModel::GetCurrentQuiescentOnsetTime() const
{
    return mCurrentQuiescentOnsetTime;
}

void ConfigurableContactInhibitionCellCycleModel::SetG1Duration()
{
    assert(mpCell != nullptr);

    // HardCellMutationState: never divide (unless AllowHardCellDivision is set)
    if (!mAllowHardCellDivision && mpCell->GetMutationState()->IsType<HardCellMutationState>())
    {
        mG1Duration = DBL_MAX;
    }
    // DifferentiatedCellProliferativeType: never divide
    else if (mpCell->GetCellProliferativeType()->IsType<DifferentiatedCellProliferativeType>())
    {
        mG1Duration = DBL_MAX;
    }
    // TransitCellProliferativeType with WildType: can divide
    else
    {
        mG1Duration = GetTransitCellG1Duration() + 2.0 * RandomNumberGenerator::Instance()->ranf();
    }
}

void ConfigurableContactInhibitionCellCycleModel::UpdateCellCyclePhase()
{
    // Non-proliferating types: stay quiescent
    if ((!mAllowHardCellDivision && mpCell->GetMutationState()->IsType<HardCellMutationState>())
        || mpCell->GetCellProliferativeType()->IsType<DifferentiatedCellProliferativeType>())
    {
        mCurrentCellCyclePhase = G_ZERO_PHASE;
        mpCell->GetCellData()->SetItem("ci_inhibited", 0.0);
        mpCell->GetCellData()->SetItem("quiescent_duration", 0.0);
        return;
    }

    double dt = SimulationTime::Instance()->GetTimeStep();
    double time_since_birth = GetAge();

    // If in G1 phase, check contact inhibition
    if (mCurrentCellCyclePhase == G_ONE_PHASE)
    {
        bool inhibited = false;

        if (mUseNeighbourInhibition)
        {
            double neighbour_count = mpCell->GetCellData()->GetItem("neighbour_count");
            if ((unsigned)neighbour_count >= mNeighbourThreshold)
            {
                inhibited = true;
            }
        }

        if (mUseHeightInhibition)
        {
            double height = mpCell->GetCellData()->GetItem("height");
            if (height > mHeightThreshold)
            {
                inhibited = true;
            }
        }

        if (inhibited)
        {
            // Time dilation: extend G1 duration
            mG1Duration += dt;

            // Track quiescent duration
            mCurrentQuiescentDuration = SimulationTime::Instance()->GetTime() - mCurrentQuiescentOnsetTime;
        }
        else
        {
            // Reset quiescent tracking
            mCurrentQuiescentDuration = 0.0;
            mCurrentQuiescentOnsetTime = SimulationTime::Instance()->GetTime();
        }

        // Write inhibition state to CellData for tracking
        mpCell->GetCellData()->SetItem("ci_inhibited", inhibited ? 1.0 : 0.0);
        mpCell->GetCellData()->SetItem("quiescent_duration", mCurrentQuiescentDuration);
    }
    else
    {
        mpCell->GetCellData()->SetItem("ci_inhibited", 0.0);
        mpCell->GetCellData()->SetItem("quiescent_duration", 0.0);
    }

    // Explicit phase update (do NOT call base class UpdateCellCyclePhase)
    double m_duration = GetMDuration();
    double g1_duration = GetG1Duration();
    double s_duration = GetSDuration();
    double g2_duration = GetG2Duration();

    if (time_since_birth < m_duration)
    {
        mCurrentCellCyclePhase = M_PHASE;
    }
    else if (time_since_birth < m_duration + g1_duration)
    {
        mCurrentCellCyclePhase = G_ONE_PHASE;
    }
    else if (time_since_birth < m_duration + g1_duration + s_duration)
    {
        mCurrentCellCyclePhase = S_PHASE;
    }
    else if (time_since_birth < m_duration + g1_duration + s_duration + g2_duration)
    {
        mCurrentCellCyclePhase = G_TWO_PHASE;
    }
}

void ConfigurableContactInhibitionCellCycleModel::InitialiseDaughterCell()
{
    // Set daughter to transit
    boost::shared_ptr<AbstractCellProperty> p_transit =
        mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<TransitCellProliferativeType>();
    mpCell->SetCellProliferativeType(p_transit);

    // Stochastic mutation assignment
    double rand = RandomNumberGenerator::Instance()->ranf();
    if (rand < (1.0 - mTargetProportion))
    {
        // Hard cell
        boost::shared_ptr<AbstractCellProperty> p_hard =
            mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<HardCellMutationState>();
        mpCell->SetMutationState(p_hard);
    }
    else
    {
        // Soft cell (WildType)
        boost::shared_ptr<AbstractCellProperty> p_wt =
            mpCell->rGetCellPropertyCollection().GetCellPropertyRegistry()->Get<WildTypeCellMutationState>();
        mpCell->SetMutationState(p_wt);
    }

    AbstractSimplePhaseBasedCellCycleModel::InitialiseDaughterCell();
}

void ConfigurableContactInhibitionCellCycleModel::ResetForDivision()
{
    AbstractSimplePhaseBasedCellCycleModel::ResetForDivision();
    mCurrentQuiescentOnsetTime = SimulationTime::Instance()->GetTime();
    mCurrentQuiescentDuration = 0.0;
}

AbstractCellCycleModel* ConfigurableContactInhibitionCellCycleModel::CreateCellCycleModel()
{
    return new ConfigurableContactInhibitionCellCycleModel(*this);
}

void ConfigurableContactInhibitionCellCycleModel::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<UseNeighbourInhibition>" << mUseNeighbourInhibition << "</UseNeighbourInhibition>\n";
    *rParamsFile << "\t\t\t<UseHeightInhibition>" << mUseHeightInhibition << "</UseHeightInhibition>\n";
    *rParamsFile << "\t\t\t<NeighbourThreshold>" << mNeighbourThreshold << "</NeighbourThreshold>\n";
    *rParamsFile << "\t\t\t<HeightThreshold>" << mHeightThreshold << "</HeightThreshold>\n";
    *rParamsFile << "\t\t\t<AllowHardCellDivision>" << mAllowHardCellDivision << "</AllowHardCellDivision>\n";
    *rParamsFile << "\t\t\t<TargetProportion>" << mTargetProportion << "</TargetProportion>\n";
    AbstractSimplePhaseBasedCellCycleModel::OutputCellCycleModelParameters(rParamsFile);
}

#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(ConfigurableContactInhibitionCellCycleModel)
