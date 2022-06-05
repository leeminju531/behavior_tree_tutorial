#include <behaviortree_cpp_v3/behavior_tree.h>
#include <behaviortree_cpp_v3/bt_factory.h>

using namespace BT;

// A typical (and recommended) way to do this in ROS is using ActionLib.
// ActionLib provides exactly the kind of API 
// that we need to implement correctly an asynchronous behavior:

// None of these operations are "blocking", therefore we don't need to spawn our own thread.

// This action talk to a remote server
class ActionClientNode : public BT::StatefulActionNode
{
public:
    ActionClientNode(const std::string& name,const BT::NodeConfiguration& config)
    : BT::StatefulActionNode(name,config)
    {}
    NodeStatus onStart() override
    {
        // send a request to the server
        bool accepted = sendStartRequestToServer();
        if(!accepted){
            return NodeStatus::FAILURE;
        }
        else{
            return NodeStatus::RUNNING;
        }
    }
    /// method invoked by an action in the RUNNING state.
    NodeStatus onRunning() override
    {
        // more psuedo-code
        auto current_state = getCurrentStateFromServer();
        if(current_state == DONE)
        {
            // retrieve the result
            auto result = getResult();
            // check if this result is "good"
            if(IsValidResult(result)){
                return NodeStatus::SUCCESS;
            }
            else {
                NodeStatus::FAILURE;
            }
        }
        else if(current_state == ABORTED){
            // fail if the action was aborted by some other client
            return NodeStatus::FAILURE;
        }
        else{
            // probably (current_state == EXECUTING) 
            return NodeStatus::RUNNING;
        }
    }
    void onHalted() override
    {
        // notify the server that the operation have been aborted
        sendAbortSignalToServer();
    }
};