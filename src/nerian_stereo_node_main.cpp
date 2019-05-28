/*******************************************************************************
 * Copyright (c) 2019 Nerian Vision Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *******************************************************************************/

#include "nerian_stereo_node.h"

namespace nerian_stereo {

class StereoNodeStandalone: public StereoNode {
private:
    // The standalone node has its own private node handles
    ros::NodeHandle nhInternal;
    ros::NodeHandle privateNhInternal;
    inline ros::NodeHandle& getNH() override { return nhInternal; }
    inline ros::NodeHandle& getPrivateNH() override { return privateNhInternal; }
public:
    StereoNodeStandalone(): privateNhInternal("~") { }

    /**
     * \brief The main loop of this node
     */
    int run() {
        prepareAsyncTransfer();
        try {
            while(ros::ok()) {
                // Dispatch any queued ROS callbacks
                ros::spinOnce();
                // Get a single image pair and process it
                processOneImagePair();
            }
        } catch(const std::exception& ex) {
            ROS_FATAL("Exception occured: %s", ex.what());
        }
    }
};

} // namespace

int main(int argc, char** argv) {
    try {
        ros::init(argc, argv, "nerian_stereo");
        nerian_stereo::StereoNodeStandalone node;
        node.init();
        node.initDynamicReconfigure();
        return node.run();
    } catch(const std::exception& ex) {
        ROS_FATAL("Exception occured: %s", ex.what());
        return 1;
    }
}

