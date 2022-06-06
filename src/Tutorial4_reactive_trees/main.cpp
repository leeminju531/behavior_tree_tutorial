#include <behaviortree_cpp_v3/bt_factory.h>
#include "MoveBaseAction.h"// custom node definition

/*
test 결과 확인한 사실 

- Sequence 안에 AsyncAction 노드가 껴있을 경우
  SUCCESS를 반환해도 다음 action node로 automatic trigger가 되지 않는다.
 
 => main에서 tree.tickRoot() 를 통해 trigger 시켜줘야 한다.

 => AsyncAction노드는 독립적인 스레드로 실행되는데, 이 상태를 확인하는 방법은 tree.tickRoot를 통해 확인할 수 있다.
    이 확인 과정을 고려했을때, Main에서 Trigger 시키도록 설계된 건 자연스러운 걸지도?


- tree.sleep()은 AsyncAction의 SUCCESS 반환에 Wakeup 된다. (이는 docs에 존재)

*/
BT::NodeStatus CheckBattery();
NodeStatus SaySomething(BT::TreeNode& self);

void StatusPrint(const NodeStatus& status);

int main()
{
    using std::chrono::milliseconds;

    BehaviorTreeFactory factory;
    factory.registerSimpleCondition("BatteryOK",std::bind(CheckBattery));
    factory.registerNodeType<MoveBaseAction>("MoveBase");
    factory.registerSimpleAction("SaySomething",SaySomething,
                                                {InputPort<std::string>("message")});
    auto tree = factory.createTreeFromFile("./sequence_tree.xml");
    // auto tree = factory.createTreeFromFile("./reactiveSequence_tree.xml");
    NodeStatus status;
    std::cout << "\n--- 1st executeTick() --- "<<std::endl;
    status = tree.tickRoot();

    // BT.CPP을 사용함에 있어서
    // 다른 sleep 대신에 , 'tree.sleep()' format 사용이 선호된다.
    // tree.sleep() 은 프로세서를 슬립하면서, 만약 노드 안에 AsyncActionNode::tick()가 끝내 무언가를 반환한다면
    // signal을 받아 깨어나게 된다.
    tree.sleep(milliseconds(150));
    // std::cout<<"awake"<<std::endl;
    //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout<<"\n--- 2nd executeTick() --- "<<std::endl;
    // status = tree.tickRoot();

    tree.sleep(milliseconds(150));
    std::cout << "\n--- 3rd executeTick() --- "<<std::endl;
    // status = tree.tickRoot();

    
    std::cout << std::endl; 

    return 0;
}


// Simple function that return a NodeStatus
BT::NodeStatus CheckBattery()
{
    std::cout << "[Battery : ok]" << std::endl;
    return BT::NodeStatus::SUCCESS;
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