/*
    HeightTrackingModifier.cpp
    Author: Yue Wang
    Based on VolumeTrackingModifier.cpp by Gary Mirams
    Last modified: 11 Nov 2024
*/

#include "HeightTrackingModifier.hpp"
#include "NodeBasedCellPopulation.hpp"

template<unsigned DIM>
HeightTrackingModifier<DIM>::HeightTrackingModifier()
    : AbstractCellBasedSimulationModifier<DIM>()
{
}

template<unsigned DIM>
HeightTrackingModifier<DIM>::~HeightTrackingModifier()
{
}

template<unsigned DIM>
void HeightTrackingModifier<DIM>::UpdateAtEndOfTimeStep(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void HeightTrackingModifier<DIM>::SetupSolve(AbstractCellPopulation<DIM,DIM>& rCellPopulation, std::string outputDirectory)
{
    /*
     * We must update CellData in SetupSolve(), otherwise it will not have been
     * fully initialised by the time we enter the main time loop.
     */
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void HeightTrackingModifier<DIM>::UpdateCellData(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    // Make sure the cell population is updated
    rCellPopulation.Update();

    // if (bool(dynamic_cast<MeshBasedCellPopulation<DIM>*>(&rCellPopulation)))
    // {
    //     static_cast<MeshBasedCellPopulation<DIM>*>(&(rCellPopulation))->CreateVoronoiTessellation();
    // }

    // Iterate over cell population
    // 1. Ask if the cell are going to divide, 2. If yes, check the height and update
    for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = rCellPopulation.Begin();
         cell_iter != rCellPopulation.End();
         ++cell_iter)
    {
        // Get the position of this cell
        c_vector<double, DIM> cell_position = rCellPopulation.GetLocationOfCellCentre(*cell_iter);
        double cell_height = cell_position[1];
        // Store the cell's volume in CellData
        cell_iter->GetCellData()->SetItem("height", cell_height);
    }
}

template<unsigned DIM>
void HeightTrackingModifier<DIM>::OutputSimulationModifierParameters(out_stream& rParamsFile)
{
    // No parameters to output, so just call method on direct parent class
    AbstractCellBasedSimulationModifier<DIM>::OutputSimulationModifierParameters(rParamsFile);
}

// Explicit instantiation
template class HeightTrackingModifier<1>;
template class HeightTrackingModifier<2>;
template class HeightTrackingModifier<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(HeightTrackingModifier)

