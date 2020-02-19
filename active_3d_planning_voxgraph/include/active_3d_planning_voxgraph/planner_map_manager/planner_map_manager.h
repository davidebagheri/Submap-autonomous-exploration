#ifndef ACTIVE_3D_PLANNING_VOXGRAPH_PLANNER_MAP_MANAGER_H
#define ACTIVE_3D_PLANNING_VOXGRAPH_PLANNER_MAP_MANAGER_H

#include "voxgraph/frontend/submap_collection/voxgraph_submap_collection.h"
#include "voxblox/integrator/merge_integration.h"
#include "active_3d_planning_voxgraph/planner_map_manager/tools/merge_layer.h"
#include <vector>
#include <thread>

namespace active_3d_planning {
    using SubmapID = voxgraph::SubmapID;
    using Transformation = voxblox::Transformation;

    class PlannerMapManager {
    public:

        PlannerMapManager(voxgraph::VoxgraphSubmapCollection* submap_collection_ptr,
                voxgraph::VoxgraphSubmap::Config config)
                : active_submap_ptr_(std::make_shared<cblox::TsdfEsdfSubmap>(config)),
                  current_neighbours_ptr_(std::make_shared<cblox::TsdfEsdfSubmap>(config)){
            config_ = config;
            submap_collection_ptr_ = submap_collection_ptr;
        }

        void updateActiveSubmap();

        void updateCurrentNeighbours(const SubmapID& active_submap_id);

        template <typename VoxelType>
        void transformLayer(const Layer<VoxelType>& layer_in, Transformation T_out_in, Layer<VoxelType>* layer_out);

        void getUpdatedBlocksLayer(Layer<TsdfVoxel>* tsdf_layer_in,
                                   Layer<TsdfVoxel>* tsdf_layer_out,
                                   voxblox::Update::Status bit);

        // This function gets the IDs of the submaps overlapping with the active map
        std::vector<SubmapID> getOverlappingSubmapIDs();

        // This function distinguish among ID to add, to remove or already present comparing an incoming
        // list with local_area_submap_IDs_ and updating the latter
        void selectCurrentNeighboursSubmapIDs(const std::vector<SubmapID>& full_list,
                std::vector<SubmapID>* IDs_to_add,
                std::vector<SubmapID>* IDs_to_remove);

        void getEsdfFromTsdfLayer(Layer<TsdfVoxel>* tsdf_layer,
                                  Layer<EsdfVoxel>* esdf_layer,
                                  const voxblox::EsdfIntegrator::Config& esdf_integrator_config);

        void emtpyActiveSubmap();

        void addSubmapInCurrentNeighbours(const voxblox::Layer<TsdfVoxel>& tsdf_layer,
                                          const voxblox::Layer<EsdfVoxel>& esdf_layer,
                                          const SubmapID id);

        void erodeSubmapFromCurrentNeighbours(const SubmapID& id);

        bool isInNeighbouringSubmapList(const SubmapID& id);

        const cblox::TsdfEsdfSubmap& getActiveSubmap(){
            return *active_submap_ptr_;
        }

        cblox::TsdfEsdfSubmap::Ptr getActiveSubmapPtr(){
            return active_submap_ptr_;
        }

        const cblox::TsdfEsdfSubmap& getCurrentNeighbours(){
            return *current_neighbours_ptr_;
        }

        cblox::TsdfEsdfSubmap::Ptr getCurrentNeighboursPtr(){
            return current_neighbours_ptr_;
        }


    private:
        voxgraph::VoxgraphSubmap::Config config_;

        cblox::TsdfEsdfSubmap::Ptr active_submap_ptr_;

        cblox::TsdfEsdfSubmap::Ptr current_neighbours_ptr_;

        voxgraph::VoxgraphSubmapCollection* submap_collection_ptr_;

        std::map<SubmapID,BlockIndexList> current_neighbours_block_index_map_;
    };
}

#endif //ACTIVE_3D_PLANNING_VOXGRAPH_PLANNER_MAP_MANAGER_H