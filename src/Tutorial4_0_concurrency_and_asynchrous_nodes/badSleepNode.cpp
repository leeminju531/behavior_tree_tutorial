#include <behaviortree_cpp_v3/behavior_tree.h>
#include <behaviortree_cpp_v3/bt_factory.h>
#include <chrono>
using namespace BT;

// This is the synchronous version of the Node. probably not what we want.

// A wrong way to implement the SleepNode would be this one:
// sleep 액션 노드인데 Synchronous 클래스를 상속받았음.
class BadSleepNode1 : public BT::ActionNodeBase
{
public:
    BadSleepNode1(const std::string& name,const BT::NodeConfiguration& config)
    : BT::ActionNodeBase(name,config)
    {}
    static BT::PortsList providedPorts()
    {
        return {BT::InputPort<int>("msec")};
    }
    NodeStatus tick() override
    {
        int msec = 0;
        getInput("msec",msec);
        // This blocking function will FREEZE the entire tree
        std::this_thread::sleep_for(std::chrono::milliseconds(msec));
        return NodeStatus::SUCCESS;
    }
    // Can be stopped as fast as possible when the method halt() (to be implemented by the developer) is invoked.
    void halt() override
    {
        // No one can invoke this method, because I freezed the tree.
        // Even if this method COULD be executed, there is no way I can
    }
};

// This will spawn its own thread. But it still has problems when halted
class BadSleepNode2 : public BT::AsyncActionNode
{
public :
    BadSleepNode2(const std::string& name,const BT::NodeConfiguration& config)
    : BT::ActionNodeBase(name,config)
    {}
    static BT::PortsList providedPorts()
    {
      return{ BT::InputPort<int>("msec") };
    }
    NodeStatus tick() override
    {
        int msec = 0;
        getInput("msec", msec);
        // This code runs in its own thread, therefore the Tree is still running.
        // This seems good but the thread still can't be aborted -> 동작은 하지만, 멈추는 것도 생각해서 구현해야된다 !! 
        std::this_thread::sleep_for( std::chrono::milliseconds(msec) );
        return NodeStatus::SUCCESS;
    }

    // The halt() method can not kill the spawned thread

};

// correct version 
// I will create my own thread here, for no good reason
class ThreadedSleepNode : public BT::AsyncActionNode
{
public:
    ThreadedSleepNode(const std::string& name, const BT::NodeConfiguration& config)
      : BT::ActionNodeBase(name, config)
    {}
    static BT::PortsList providedPorts()
    {
      return{ BT::InputPort<int>("msec") };
    }
    NodeStatus tick() override
    {
        // This code run in its own thread, therefore the Tree is still running.
        int msec = 0;
        getInput("msec",msec);

        using namespace std::chrono;
        const auto deadline = system_clock::now() + milliseconds(msec);

        // periodically check isHaltRequested() 
        // and sleep for a small amount of time only (1 millisecond)
        while( !isHaltRequested() && system_clock::now() < deadline )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds(1) );
        }
        return NodeStatus::SUCCESS;     
    }


};

// 결과적으로, sleepNode.cpp (BT::StatefulActionNode 상속 받은) 보다 더 복잡함
//  you must remember that introducing multi-threading make things more complicated and should be avoided by default.