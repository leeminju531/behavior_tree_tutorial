#include <behaviortree_cpp_v3/bt_factory.h>
#include "sleepNode.cpp"// custom node definition
#include <chrono>
static const char* xml_text = R"(

 <root main_tree_to_execute = "MainTree" >
     <BehaviorTree ID="MainTree">
        <SequenceStar name="root">
            <SaySomething   message="1" />
            <SaySomething   message="2" />
            <SleepNode      msec="3000" />
            <SaySomething   message="3"  />
            <SaySomething   message="4" />
        </SequenceStar>
     </BehaviorTree>
 </root>
 )";

 
NodeStatus SaySomething(BT::TreeNode& self);

 int main()
 {
    using namespace BT;
    BehaviorTreeFactory factory;
    factory.registerSimpleAction("SaySomething",SaySomething,
                                                {InputPort<std::string>("message")});
    factory.registerNodeType<SleepNode>("SleepNode");

    auto tree = factory.createTreeFromText(xml_text);
    tree.tickRoot();

    using std::chrono::milliseconds;
    while(tree.tickRoot() == NodeStatus::RUNNING){
        tree.sleep(milliseconds(1000));
    }
    

    return 0;
 }

 NodeStatus SaySomething(BT::TreeNode& self)
{
    auto msg = self.getInput<std::string>("message");
    if (!msg)
    {
        throw BT::RuntimeError( "missing required input [message]: ", msg.error() );
    }

    std::cout << "Robot says: " << msg.value() << std::endl;
    return BT::NodeStatus::SUCCESS;
}