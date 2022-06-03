
#include <behaviortree_cpp_v3/bt_factory.h>
#include "dummy_node.h"// custom node definition
#include <iostream>

int main(){
  // 내 custom 노드를 등록시켜줘야 한다.
  BehaviorTreeFactory factory;
  // 등록되는 이름은 XML에서 사용되는 이름과 같아야 한다.
  using namespace DummyNodes;
  factory.registerNodeType<ApproachObject>("ApproachObject");

  // 아래는 class obj가 아닌 function을 node 처럼 등록함.
  factory.registerSimpleCondition("CheckBattery", std::bind(CheckBattery));
  GripperInterface gripper; // class를 이용해서 binding도 가능하다.
  factory.registerSimpleAction("OpenGripper", 
                                 std::bind(&GripperInterface::open, &gripper));
  factory.registerSimpleAction("CloseGripper", 
                                 std::bind(&GripperInterface::close, &gripper));
  //tree는 run time에 시작할 때 생성된다.
  auto tree = factory.createTreeFromFile("./my_tree.xml");
  // tree를 실행할려면 tick (호출) 해줘야 한다.
  tree.tickRoot();


  return 0;
}

