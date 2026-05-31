/*
 * 2D chip-inspired intestinal epithelial model demo with modified proliferation regulation.
 *
 * This demo builds on an initial CHASTE project framework developed by
 * Axel A. Almet, with later modifications by Sandra Montes-Olivas.
 *
 * Yue Wang substantially adapted and extended this framework for a 2D
 * chip-inspired epithelial model.
 */

#ifndef TEST_CHIP_FRAME_2D_AP_DEMO_HPP_
#define TEST_CHIP_FRAME_2D_AP_DEMO_HPP_

#include <cxxtest/TestSuite.h>
#include "AbstractCellBasedTestSuite.hpp"
#include "CheckpointArchiveTypes.hpp"
#include "SmartPointers.hpp"
#include "FakePetscSetup.hpp"
#include "Exception.hpp"
#include "SimulationTime.hpp"
#include <cmath>
#include <random>
#include <sstream>
#include <iomanip>
#include <string>

#include "CylindricalHoneycombMeshGenerator.hpp"
#include "Cylindrical2dMesh.hpp"
#include "MeshBasedCellPopulationWithGhostNodes.hpp"
#include "OffLatticeSimulation.hpp"
#include "FixedRegionPlaneBoundaryCondition.hpp"

#include "TransitCellProliferativeType.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "WildTypeCellMutationState.hpp"
#include "HardCellMutationState.hpp"

#include "StochasticTargetProportionBasedCellCycleModel.hpp"

#include "EpithelialLayerLinearSpringForce.hpp"
#include "EpithelialLayerAnoikisCellKiller.hpp"
#include "EpithelialLayerDataTrackingModifier.hpp"
#include "EpithelialCellCycleTrackingModifier.hpp"
#include "UniversalNeighbourTrackingModifier.hpp"

#include "VoronoiDataWriter.hpp"
#include "CellProliferativeTypesWriter.hpp"
#include "CellMutationStatesWriter.hpp"
#include "CellAgesWriter.hpp"
#include "CellProliferativePhasesWriter.hpp"
#include "CellIdWriter.hpp"
#include "CellProliferativePhasesCountWriter.hpp"
#include "CellDivisionLocationsWriter.hpp"

#include "CellsGenerator.hpp"
#include "RandomNumberGenerator.hpp"

class TestChipFrame_2D_AP_demo : public AbstractCellBasedTestSuite
{
private:
    void ResetSingletons()
    {
        SimulationTime::Instance()->Destroy();
        SimulationTime::Instance()->SetStartTime(0.0);
    }

    void RunSimulation(double tp, double sr, const std::string& outputDir)
    {
        // Reseed each run.
        RandomNumberGenerator::Instance()->Reseed(std::random_device{}());

        // Mesh setup for a cylindrical 2D chip-inspired domain.
        unsigned cells_across = 40;
        unsigned cells_up = 15;
        unsigned ghosts = 3;
        CylindricalHoneycombMeshGenerator generator(cells_across, cells_up, ghosts);
        boost::shared_ptr<Cylindrical2dMesh> p_mesh = generator.GetCylindricalMesh();
        std::vector<unsigned> location_indices = generator.GetCellLocationIndices();

        MAKE_PTR(DifferentiatedCellProliferativeType, p_diff);
        MAKE_PTR(WildTypeCellMutationState, p_wt);

        // Create the initial cell population with the selected TP parameter.
        std::vector<CellPtr> cells;
        for (unsigned i = 0; i < location_indices.size(); i++)
        {
            StochasticTargetProportionBasedCellCycleModel* p_model =
                new StochasticTargetProportionBasedCellCycleModel();
            p_model->SetTargetProportion(tp);
            p_model->SetDimension(2);

            double birth_time = 12.0 * RandomNumberGenerator::Instance()->ranf();
            CellPtr p_cell(new Cell(p_wt, p_model));
            p_cell->SetCellProliferativeType(p_diff);
            p_cell->SetBirthTime(-birth_time);
            p_cell->InitialiseCellCycleModel();
            cells.push_back(p_cell);
        }

        // Build the mesh-based cell population and enable output writers.
        MeshBasedCellPopulationWithGhostNodes<2> cell_population(*p_mesh, cells, location_indices);
        cell_population.AddPopulationWriter<VoronoiDataWriter>();
        cell_population.AddCellWriter<CellProliferativeTypesWriter>();
        cell_population.AddCellWriter<CellMutationStatesWriter>();
        cell_population.AddCellWriter<CellAgesWriter>();
        cell_population.AddCellWriter<CellProliferativePhasesWriter>();
        cell_population.AddCellWriter<CellIdWriter>();
        cell_population.AddCellPopulationCountWriter<CellProliferativePhasesCountWriter>();
        cell_population.AddCellPopulationEventWriter<CellDivisionLocationsWriter>();

        // Seed epithelial layer near the basal region and assign hard cells by TP.
        MAKE_PTR(TransitCellProliferativeType, p_transit);
        MAKE_PTR(HardCellMutationState, p_hard);
        for (unsigned i = 0; i < location_indices.size(); i++)
        {
            unsigned idx = location_indices[i];
            double y = p_mesh->GetNode(idx)->rGetLocation()[1];
            if (y > 0.5 && y < 1.2)
            {
                CellPtr cell = cell_population.GetCellUsingLocationIndex(idx);
                cell->SetCellProliferativeType(p_transit);
                if (RandomNumberGenerator::Instance()->ranf() >= tp)
                {
                    cell->SetMutationState(p_hard);
                }
            }
        }

        // Configure and run an off-lattice simulation.
        OffLatticeSimulation<2> simulator(cell_population);
        simulator.SetEndTime(168.0);
        simulator.SetDt(0.01);
        simulator.SetSamplingTimestepMultiple(50);
        simulator.SetOutputDirectory(outputDir);

        // Add mechanical interaction force with the selected SR parameter.
        MAKE_PTR(EpithelialLayerLinearSpringForce<2>, p_force);
        p_force->SetEpithelialEpithelialSpringStiffness(15.0);
        p_force->SetEpithelialNonepithelialSpringStiffness(15.0);
        p_force->SetNonepithelialNonepithelialSpringStiffness(15.0);
        p_force->SetHardCellStiffnessRatio(sr);
        p_force->SetMeinekeDivisionRestingSpringLength(0.5);
        p_force->SetMeinekeSpringGrowthDuration(1.0);
        p_force->SetCutOffLength(1.5);
        simulator.AddForce(p_force);

        // Add modifiers.
        MAKE_PTR(EpithelialLayerDataTrackingModifier<2>, p_data_modifier);
        simulator.AddSimulationModifier(p_data_modifier);

        MAKE_PTR(EpithelialCellCycleTrackingModifier<2>, p_epi_modifier);
        p_epi_modifier->SetSamplingInterval(50);
        simulator.AddSimulationModifier(p_epi_modifier);

        MAKE_PTR(UniversalNeighbourTrackingModifier<2>, p_n_modifier);
        p_n_modifier->SetInteractionRadius(1.5);
        simulator.AddSimulationModifier(p_n_modifier);

        // Apply top/bottom fixed-plane boundaries for the chip channel.
        c_vector<double, 2> bottom_point = zero_vector<double>(2);
        c_vector<double, 2> bottom_normal; bottom_normal[0] = 0.0; bottom_normal[1] = -1.0;
        MAKE_PTR_ARGS(FixedRegionPlaneBoundaryCondition<2>, p_bottom_bc,
            (&cell_population, bottom_point, bottom_normal));
        simulator.AddCellPopulationBoundaryCondition(p_bottom_bc);

        double top_y = (cells_up - 1) * std::sqrt(3.0) / 2.0;
        c_vector<double, 2> top_point; top_point[0] = 0.0; top_point[1] = top_y;
        c_vector<double, 2> top_normal; top_normal[0] = 0.0; top_normal[1] = 1.0;
        MAKE_PTR_ARGS(FixedRegionPlaneBoundaryCondition<2>, p_top_bc,
            (&cell_population, top_point, top_normal));
        simulator.AddCellPopulationBoundaryCondition(p_top_bc);

        // Anoikis killer.
        MAKE_PTR_ARGS(EpithelialLayerAnoikisCellKiller, p_anoikis, (&cell_population));
        p_anoikis->SetCutOffRadius(1.5);
        simulator.AddCellKiller(p_anoikis);

        simulator.Solve();
    }

public:
    void Test2DChipModelAPDemo()
    {
        EXIT_IF_PARALLEL;

        // Single compact demo parameter set.
        const double tp = 0.7;
        const double sr = 1.0;
        const int replicate = 1;

        // Demo-oriented output folder naming.
        std::ostringstream oss;
        oss << "ChipFrame2D_demo/AP_demo"
            << "_tp" << std::fixed << std::setprecision(1) << tp
            << "_sr" << std::setprecision(1) << sr
            << "_R" << replicate;
        std::string dir = oss.str();

        ResetSingletons();
        try {
            TS_TRACE(">>> START: " + dir);
            RunSimulation(tp, sr, dir);
            TS_TRACE(">>> DONE:  " + dir);
        } catch (Exception& e) {
            TS_TRACE(">>> FAILED: " + dir + " — " + e.GetMessage());
        } catch (std::exception& e) {
            TS_TRACE(">>> FAILED: " + dir + " — " + std::string(e.what()));
        } catch (...) {
            TS_TRACE(">>> FAILED: " + dir + " — unknown error");
        }
    }
};

#endif // TEST_CHIP_FRAME_2D_AP_DEMO_HPP_
