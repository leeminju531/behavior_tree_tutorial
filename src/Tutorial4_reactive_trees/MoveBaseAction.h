#ifndef MOVE_BASE_ACTION_NODE_H
#define MOVE_BASE_ACTION_NODE_H
#include <behaviortree_cpp_v3/behavior_tree.h>
#include <behaviortree_cpp_v3/bt_factory.h>
// An Asynchronous node may delegate this long execution
// either to another process, another server or simply another thread.
using namespace BT;
struct Pose2D // custom type
{
    double x,y,theta;
};
// bcuz of generic port
namespace BT
{
    template <> inline Pose2D convertFromString(StringView str)
    {
        printf("Converting string: \"%s\"\n", str.data() ); // In production code, you would (obviously) remove the printf statement.

        // real numbers separated by semicolons
        auto parts = splitString(str,';'); 
        if(parts.size() != 3)
        {
            throw RuntimeError("invalid input");
        }
        else
        {
            Pose2D output;
            output.x = convertFromString<double>(parts[0]); // Once we split the input into single numbers, we can reuse the specialization
            output.y = convertFromString<double>(parts[1]);
            output.theta = convertFromString<double>(parts[2]);
            return output;
        }
    }
}

// An Asynchronous Action has it's own thread. 
// This allows the user to use blocking functions
// but to return the flow of execution to the tree.
class MoveBaseAction : public AsyncActionNode
{
public:
    MoveBaseAction(const std::string& name,const NodeConfiguration& config)
     : AsyncActionNode(name,config)
    {}
    static PortsList providedPorts()
    {
        return { InputPort<Pose2D>("goal")} ;
    }
    NodeStatus tick() override;

    // This overloaded method is used to stop the execution of this node.
    void halt() override
    {
        _halt_requested.store(true);
    }
private:
    std::atomic_bool _halt_requested;

};

// 해당 function이 main thread와는 다른 thread로 동작한다 
NodeStatus MoveBaseAction::tick()
{
    Pose2D goal;
    if( !getInput<Pose2D>("goal",goal))
    {
        throw RuntimeError("missing required input [goal]");
    }
    printf("[ MoveBase: STARTED ]. goal: x=%.f y=%.1f theta=%.2f\n", 
           goal.x, goal.y, goal.theta);
    int count = 0;

    // computing 하는데 250ms 걸린다고 가정
    while(!_halt_requested && count++ < 25)
    {
        if(count%5==0)
        {
            std::cout << count * 10 <<"ms passed at async moveBase action node"<<std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    if(_halt_requested)
        std::cout<<"halt invoked" << std::endl;
    std::cout << "[ MoveBase: FINISHED ]" << std::endl;
    return _halt_requested ? NodeStatus::FAILURE : NodeStatus::SUCCESS;
}
#endif