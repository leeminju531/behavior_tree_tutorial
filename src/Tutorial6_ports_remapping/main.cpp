#include <behaviortree_cpp_v3/bt_factory.h>
#include "MoveBaseAction.h"
NodeStatus SaySomething(BT::TreeNode& self);

int main()
{
    BT::BehaviorTreeFactory factory;
    factory.registerSimpleAction("SaySomething",SaySomething,
                                                {InputPort<std::string>("message")});
    factory.registerNodeType<MoveBaseAction>("MoveBase");

    auto tree = factory.createTreeFromFile("./my_tree.xml");

    NodeStatus status = NodeStatus::RUNNING;
    // Keep on ticking until you get either a SUCCESS or FAILURE state
    while(status == NodeStatus::RUNNING)
    {
        status = tree.tickRoot();
        // IMPORTANT: add sleep to avoid busy loops.
        tree.sleep(std::chrono::milliseconds(1));
    }
    // let's visualize some information about the current state of the blackboards.
    std::cout << "--------------" << std::endl;
    tree.blackboard_stack[0]->debugMessage();
    std::cout << "--------------" << std::endl;
    tree.blackboard_stack[1]->debugMessage();
    std::cout << "--------------" << std::endl;

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