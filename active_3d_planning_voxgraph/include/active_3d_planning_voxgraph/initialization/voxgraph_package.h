#ifndef ACTIVE_3D_PLANNING_VOXBLOX_INITIALIZE_VOXBLOX_PACKAGE_H
#define ACTIVE_3D_PLANNING_VOXBLOX_INITIALIZE_VOXBLOX_PACKAGE_H

namespace active_3d_planning {
    namespace initialize {
        // force the linker to include this lib
        // todo: theres probably a better way to do this...
        void voxgraph_package();

    } // namespace initialize
} // namespace active_3d_planning

#endif // ACTIVE_3D_PLANNING_VOXBLOX_INITIALIZE_VOXBLOX_PACKAGE_H