#ifndef SIMPLE_BT_NODES_H
#define SIMPLE_BT_NODES_H
#include <behaviortree_cpp_v3/behavior_tree.h>
#include <behaviortree_cpp_v3/bt_factory.h>

using BT::NodeStatus;
using namespace BT;
//custom SyncActionNode (synchronous action) -> Squential
namespace DummyNodes{
class ApproachObject : public BT::SyncActionNode{

public:
    ApproachObject(const std::string& name) :
        BT::SyncActionNode(name, {}){}
    NodeStatus tick() override;
   
};

// 그리퍼 행위를 NodeStatus로
class GripperInterface
{
public:
    GripperInterface(): _open(true){}

    NodeStatus open();
    NodeStatus close();
    
private:
    bool _open;
};

BT::NodeStatus ApproachObject::tick()
{
    std::cout<< "ApproachObject" << this->name()<<std::endl;
    return NodeStatus::SUCCESS;
}
    // Simple function that return a NodeStatus
BT::NodeStatus CheckBattery()
{
    std::cout << "[Battery : ok]" << std::endl;
    return BT::NodeStatus::SUCCESS;
}

BT::NodeStatus GripperInterface::open()
{
    _open = true;
    std::cout<< "GripperInterface :: open" << std::endl;
    return NodeStatus::SUCCESS;
}
BT::NodeStatus GripperInterface::close()
{
    std::cout<<"GripperInterface :: close" << std::endl;
    _open = false;
    return NodeStatus::SUCCESS;
}
}

#endif