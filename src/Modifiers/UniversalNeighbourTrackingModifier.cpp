// AI-generated experimental module. Original project files remain unchanged.
#include "UniversalNeighbourTrackingModifier.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "MeshBasedCellPopulationWithGhostNodes.hpp"
#include "DifferentiatedCellProliferativeType.hpp"

template<unsigned DIM>
UniversalNeighbourTrackingModifier<DIM>::UniversalNeighbourTrackingModifier()
    : AbstractCellBasedSimulationModifier<DIM>(),
      mInteractionRadius(1.5)
{
}

template<unsigned DIM>
void UniversalNeighbourTrackingModifier<DIM>::SetInteractionRadius(double radius)
{
    mInteractionRadius = radius;
}

template<unsigned DIM>
double UniversalNeighbourTrackingModifier<DIM>::GetInteractionRadius() const
{
    return mInteractionRadius;
}

template<unsigned DIM>
void UniversalNeighbourTrackingModifier<DIM>::UpdateAtEndOfTimeStep(AbstractCellPopulation<DIM, DIM>& rCellPopulation)
{
    rCellPopulation.Update();

    NodeBasedCellPopulation<DIM>* p_node_population = dynamic_cast<NodeBasedCellPopulation<DIM>*>(&rCellPopulation);
    MeshBasedCellPopulation<DIM>* p_mesh_population = dynamic_cast<MeshBasedCellPopulation<DIM>*>(&rCellPopulation);
    MeshBasedCellPopulationWithGhostNodes<DIM>* p_ghost_population = dynamic_cast<MeshBasedCellPopulationWithGhostNodes<DIM>*>(&rCellPopulation);

    for (typename AbstractCellPopulation<DIM>::Iterator cell_iter = rCellPopulation.Begin();
         cell_iter != rCellPopulation.End();
         ++cell_iter)
    {
        unsigned index = rCellPopulation.GetLocationIndexUsingCell(*cell_iter);
        unsigned neighbour_count = 0;

        if (p_node_population != nullptr)
        {
            // NodeBasedCellPopulation: use interaction radius
            std::set<unsigned> neighbours = p_node_population->GetNodesWithinNeighbourhoodRadius(index, mInteractionRadius);

            for (std::set<unsigned>::iterator it = neighbours.begin(); it != neighbours.end(); ++it)
            {
                neighbour_count++;
            }
        }
        else if (p_mesh_population != nullptr)
        {
            // MeshBasedCellPopulation: use Delaunay neighbours (epithelial only)
            std::set<unsigned> neighbours = p_mesh_population->GetNeighbouringNodeIndices(index);

            for (std::set<unsigned>::iterator it = neighbours.begin(); it != neighbours.end(); ++it)
            {
                if (p_ghost_population != nullptr && p_ghost_population->IsGhostNode(*it))
                {
                    continue;
                }
                // Only count epithelial (non-Differentiated) neighbours
                CellPtr p_neighbour_cell = rCellPopulation.GetCellUsingLocationIndex(*it);
                if (p_neighbour_cell->GetCellProliferativeType()->template IsType<DifferentiatedCellProliferativeType>())
                {
                    continue;
                }
                neighbour_count++;
            }
        }
        else
        {
            EXCEPTION("UniversalNeighbourTrackingModifier requires NodeBasedCellPopulation or MeshBasedCellPopulation.");
        }

        cell_iter->GetCellData()->SetItem("neighbour_count", (double)neighbour_count);
    }
}

template<unsigned DIM>
void UniversalNeighbourTrackingModifier<DIM>::SetupSolve(AbstractCellPopulation<DIM, DIM>& rCellPopulation, std::string outputDirectory)
{
    UpdateAtEndOfTimeStep(rCellPopulation);
}

template<unsigned DIM>
void UniversalNeighbourTrackingModifier<DIM>::OutputSimulationModifierParameters(out_stream& rParamsFile)
{
    *rParamsFile << "\t\t\t<InteractionRadius>" << mInteractionRadius << "</InteractionRadius>\n";
    AbstractCellBasedSimulationModifier<DIM>::OutputSimulationModifierParameters(rParamsFile);
}

// Explicit instantiation
template class UniversalNeighbourTrackingModifier<1>;
template class UniversalNeighbourTrackingModifier<2>;
template class UniversalNeighbourTrackingModifier<3>;

#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(UniversalNeighbourTrackingModifier)
