// AI-generated experimental module. Original project files remain unchanged.
#include "EpithelialCellCycleTrackingModifier.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "MeshBasedCellPopulationWithGhostNodes.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "AbstractPhaseBasedCellCycleModel.hpp"
#include "SimulationTime.hpp"
#include "OutputFileHandler.hpp"
#include <cmath>

template<unsigned DIM>
EpithelialCellCycleTrackingModifier<DIM>::EpithelialCellCycleTrackingModifier()
    : AbstractCellBasedSimulationModifier<DIM>(),
      mSamplingInterval(50),
      mTimestepCounter(0)
{
}

template<unsigned DIM>
EpithelialCellCycleTrackingModifier<DIM>::~EpithelialCellCycleTrackingModifier()
{
}

template<unsigned DIM>
void EpithelialCellCycleTrackingModifier<DIM>::SetSamplingInterval(unsigned interval)
{
    mSamplingInterval = interval;
}

template<unsigned DIM>
unsigned EpithelialCellCycleTrackingModifier<DIM>::GetSamplingInterval() const
{
    return mSamplingInterval;
}

template<unsigned DIM>
void EpithelialCellCycleTrackingModifier<DIM>::SetupSolve(
    AbstractCellPopulation<DIM, DIM>& rCellPopulation, std::string outputDirectory)
{
    OutputFileHandler output_file_handler(outputDirectory + "/", false);
    mpOutputFile = output_file_handler.OpenOutputFile("epithelial_cellcycle.dat");

    // Write header
    *mpOutputFile << "# time\tepi_count\tepi_G0\tepi_G1\tepi_S\tepi_G2\tepi_M"
                  << "\tn_inhibited\th_mean\th_max\th_std\tmean_q_dur\tmean_epi_neighbours\n";

    mTimestepCounter = 0;
    WriteData(rCellPopulation);
}

template<unsigned DIM>
void EpithelialCellCycleTrackingModifier<DIM>::UpdateAtEndOfTimeStep(
    AbstractCellPopulation<DIM, DIM>& rCellPopulation)
{
    mTimestepCounter++;
    if (mTimestepCounter % mSamplingInterval == 0)
    {
        WriteData(rCellPopulation);
    }
}

template<unsigned DIM>
void EpithelialCellCycleTrackingModifier<DIM>::UpdateAtEndOfSolve(
    AbstractCellPopulation<DIM, DIM>& rCellPopulation)
{
    WriteData(rCellPopulation);
    mpOutputFile->close();
}

template<unsigned DIM>
void EpithelialCellCycleTrackingModifier<DIM>::WriteData(
    AbstractCellPopulation<DIM, DIM>& rCellPopulation)
{
    unsigned epi_count = 0;
    unsigned epi_G0 = 0, epi_G1 = 0, epi_S = 0, epi_G2 = 0, epi_M = 0;
    unsigned n_inhibited = 0;
    double h_sum = 0.0, h_sum_sq = 0.0, h_max = -1e30;
    double q_dur_sum = 0.0;
    unsigned q_dur_count = 0;
    double neighbour_sum = 0.0;
    unsigned neighbour_data_count = 0;

    for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = rCellPopulation.Begin();
         cell_iter != rCellPopulation.End();
         ++cell_iter)
    {
        // Skip non-epithelial cells
        if (cell_iter->GetCellProliferativeType()->template IsType<DifferentiatedCellProliferativeType>())
        {
            continue;
        }

        epi_count++;

        // Phase counting
        AbstractPhaseBasedCellCycleModel* p_model =
            static_cast<AbstractPhaseBasedCellCycleModel*>(cell_iter->GetCellCycleModel());
        switch (p_model->GetCurrentCellCyclePhase())
        {
            case G_ZERO_PHASE: epi_G0++; break;
            case G_ONE_PHASE:  epi_G1++; break;
            case S_PHASE:      epi_S++;  break;
            case G_TWO_PHASE:  epi_G2++; break;
            case M_PHASE:      epi_M++;  break;
            default: break;
        }

        // Height from y-coordinate
        c_vector<double, DIM> location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
        double h = location[DIM - 1]; // y in 2D, z in 3D
        h_sum += h;
        h_sum_sq += h * h;
        if (h > h_max)
        {
            h_max = h;
        }

        // CI inhibition tracking (if available)
        if (cell_iter->GetCellData()->HasItem("ci_inhibited"))
        {
            if (cell_iter->GetCellData()->GetItem("ci_inhibited") > 0.5)
            {
                n_inhibited++;
            }
        }
        if (cell_iter->GetCellData()->HasItem("quiescent_duration"))
        {
            double q = cell_iter->GetCellData()->GetItem("quiescent_duration");
            if (q > 0.0)
            {
                q_dur_sum += q;
                q_dur_count++;
            }
        }

        // Neighbour count (if available)
        if (cell_iter->GetCellData()->HasItem("neighbour_count"))
        {
            neighbour_sum += cell_iter->GetCellData()->GetItem("neighbour_count");
            neighbour_data_count++;
        }
    }

    // Compute statistics
    double h_mean = (epi_count > 0) ? h_sum / epi_count : 0.0;
    double h_std = 0.0;
    if (epi_count > 1)
    {
        double variance = (h_sum_sq / epi_count) - (h_mean * h_mean);
        h_std = (variance > 0.0) ? std::sqrt(variance) : 0.0;
    }
    if (epi_count == 0)
    {
        h_max = 0.0;
    }
    double mean_q_dur = (q_dur_count > 0) ? q_dur_sum / q_dur_count : 0.0;
    double mean_neighbours = (neighbour_data_count > 0) ? neighbour_sum / neighbour_data_count : 0.0;

    // Write line
    double time = SimulationTime::Instance()->GetTime();
    *mpOutputFile << time << "\t"
                  << epi_count << "\t"
                  << epi_G0 << "\t" << epi_G1 << "\t" << epi_S << "\t" << epi_G2 << "\t" << epi_M << "\t"
                  << n_inhibited << "\t"
                  << h_mean << "\t" << h_max << "\t" << h_std << "\t"
                  << mean_q_dur << "\t" << mean_neighbours << "\n";
}

template<unsigned DIM>
void EpithelialCellCycleTrackingModifier<DIM>::OutputSimulationModifierParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<SamplingInterval>" << mSamplingInterval << "</SamplingInterval>\n";
    AbstractCellBasedSimulationModifier<DIM>::OutputSimulationModifierParameters(rParamsFile);
}

// Explicit instantiation
template class EpithelialCellCycleTrackingModifier<1>;
template class EpithelialCellCycleTrackingModifier<2>;
template class EpithelialCellCycleTrackingModifier<3>;

#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(EpithelialCellCycleTrackingModifier)
