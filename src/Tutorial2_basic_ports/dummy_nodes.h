#ifndef SIMPLE_BT_NODES_H
#define SIMPLE_BT_NODES_H
#include <behaviortree_cpp_v3/behavior_tree.h>
#include <behaviortree_cpp_v3/bt_factory.h>

using namespace BT;

namespace DummyNodes
{
// for dealing with input ports
class SaySomething : public SyncActionNode
{
public:
    // bt는 node끼리 전달될 수 있는 데이터를 'port'라 칭함 -- dataflow through ports
    
    // node에 ports가 존재하면, 해당 클래스 상속해야함
    SaySomething(const std::string& name,const NodeConfiguration& config)
        :  SyncActionNode(name,config)
    {}


    // port를 위해 해당 static 함수는 꼭 정의되야함.
    // When a custom TreeNode has input and/or output ports,
    // these ports must be declared in the static method
    // 'static MyCustomNode::PortsList providedPorts()' format
    static PortsList providedPorts()
    {
        // This action has a single input port called "message"
        // Any port must have a name. The type is optional
        return {InputPort<std::string>("message")};
        // input from the port template 
        // 'TreeNode::getInput<T>(key)'
    }
    
    NodeStatus tick() override
    {
        Optional<std::string> msg = getInput<std::string>("message");
        if(!msg){// Check if optional is valid. If not, throw its error
            throw BT::RuntimeError("missing required input [message]: ",msg.error());
            // 이때는 fail을 return 해도 좋다.
        }
        // use the method value() to extract the valid message.
        std::cout <<"msg value : "<<msg.value() << std::endl;
        return NodeStatus::SUCCESS;
    }
};

// for dealing with output ports
class ThinkWhatToSay : public SyncActionNode
{
public:
    ThinkWhatToSay(const std::string& name,const NodeConfiguration& config)
        :  SyncActionNode(name,config)
    {}

    static PortsList providedPorts()
    {
        return { OutputPort<std::string>("text")};
    }

    // This Action writes a value into the port "text"
    NodeStatus tick() override
    {
        setOutput("text","The answer is 42");   // 디버깅 목적으로 XML에서도 쓰기가 가능하다.
        return NodeStatus::SUCCESS;
    }

};

//Same as class SaySomething, but to be registered with SimpleActionNode
NodeStatus SaySomethingSimple(BT::TreeNode& self)
{
    auto msg = self.getInput<std::string>("message");
    if (!msg)
    {
        throw BT::RuntimeError( "missing required input [message]: ", msg.error() );
    }

    std::cout << "Robot says: " << msg.value() << std::endl;
    return BT::NodeStatus::SUCCESS;
}

}
#endif