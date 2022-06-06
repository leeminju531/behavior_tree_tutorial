#include <behaviortree_cpp_v3/behavior_tree.h>
#include <behaviortree_cpp_v3/bt_factory.h>
#include <chrono>
using namespace BT;

// Asynchronous node use StatefulActionNode as base class
class SleepNode : public BT::StatefulActionNode
{
public:
    SleepNode(const std::string& name,const BT::NodeConfiguration& config)
     : BT::StatefulActionNode(name,config)
    {}
    static BT::PortsList providedPorts()
    {
        // amount of ms that we want to sleep
        return {BT::InputPort<int>("msec")};
    }
    // When the SleepNode is ticked the first time, the onStart() method is executed.
    NodeStatus onStart() override
    {
        int msec = 0;
        getInput("msec",msec);
        
        if(msec <= 0 ){
            // No need to go into the RUNNING state
            return NodeStatus::SUCCESS;
        }
        else{
            using namespace std::chrono;
            // once the deadline is reached, we will return SUCCESS.
            deadline_ = system_clock::now() + milliseconds(msec);
            return NodeStatus::RUNNING;
        }
    }


    // method invoked by an action in the RUNNING state.

    // We should continue ticking the tree in a loop
    // This will invoke the method onRunning() that may return RUNNING again or, eventually, SUCCESS.
    NodeStatus onRunning() override
    {
        std::cout <<" running "<<std::endl;
        if(std::chrono::system_clock::now() >= deadline_){
            return NodeStatus::SUCCESS;
        }
        else{
            return NodeStatus::RUNNING;
        }
    }

    // 다른 노드의 halt() signal에 의해 실행될 수 있다.
    void onHalted() override
    {
        std::cout << "SleepNode Interrupted" << std::endl;
    }

private:
    std::chrono::system_clock::time_point deadline_;

};