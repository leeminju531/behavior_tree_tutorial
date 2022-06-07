/*
asynchronous action은 아래의 특징을 갖는다
- 결과를 반환하는데 시간이 걸릴 때 주로 사용
- 그 동안 RUNNING을 반환하고 
- halt() 될 수 있다.
-----------------------------------
해당 튜토리얼은 Async 한 방법으로 CoroActionNode 상속받는 액션을 소개한다.
이는 별도의 스레드가 아닌, Corountine 하게 구현한 방법으로 

실제 프로파일 결과
- AsyncActionNode -> main 스레드와 action tick()은 다른 thread id
- CoroActionNode  -> main 스레드와 action tick()은 같은 thread id 를 가졌다.

The first class is a AsyncActionNode that executes the tick() method in a separate thread.
In this tutorial, we introduce the CoroActionNode, 
a different action that uses coroutines to achieve similar results.

=> 결과적으로 CoroActionNode가 thread_safe를 고려하지 않아도되고 더 효율적이라고 소개한다.
*/

#include <behaviortree_cpp_v3/bt_factory.h>
using namespace BT;
typedef std::chrono::milliseconds Milliseconds;

class MyAsyncAction: public CoroActionNode
{
  public:
    MyAsyncAction(const std::string& name):
        CoroActionNode(name, {})
    {}

  private:
    // This is the ideal skeleton/template of an async action:
    //  - A request to a remote service provider.
    //  - A loop where we check if the reply has been received.
    //  - You may call setStatusRunningAndYield() to "pause".
    //  - Code to execute after the reply.
    //  - A simple way to handle halt().
    NodeStatus tick() override
    {
        std::cout << name() <<": Started. Send Request to server." << std::endl;

        TimePoint initial_time = Now();
        TimePoint time_before_reply = initial_time + Milliseconds(100);

        int count = 0;
        bool reply_received = false;

        while( !reply_received )
        {
            if( count++ == 0)
            {
                // call this only once
                std::cout << name() <<": Waiting Reply..." << std::endl;
            }
            // pretend that we received a reply
            if( Now() >= time_before_reply )
            {
                reply_received = true;
            }

            if( !reply_received )
            {
                // set status to RUNNING and "pause/sleep"
                // If halt() is called, we will NOT resume execution
                setStatusRunningAndYield();
            }
        }
        std::cout <<"async thread id : "<< std::this_thread::get_id() << std::endl;
        // This part of the code is never reached if halt() is invoked,
        // only if reply_received == true;
        std::cout << name() <<": Done. 'Waiting Reply' loop repeated "
                  << count << " times" << std::endl;
        cleanup(false);
        return NodeStatus::SUCCESS;
    }

    // you might want to cleanup differently if it was halted or successful
    void cleanup(bool halted)
    {
        if( halted )
        {
            std::cout << name() <<": cleaning up after an halt()\n" << std::endl;
        }
        else{
            std::cout << name() <<": cleaning up after SUCCESS\n" << std::endl;
        }
    }

    void halt() override
    {
        std::cout << name() <<": Halted." << std::endl;
        cleanup(true);
        // Do not forget to call this at the end.
        CoroActionNode::halt();
    }

    TimePoint Now()
    { 
        return std::chrono::high_resolution_clock::now(); 
    };
};

// clang-format off
static const char* xml_text = R"(

 <root >
     <BehaviorTree>
        <Timeout msec="150">
            <SequenceStar name="sequence">
                <MyAsyncAction name="action_A"/>
                <MyAsyncAction name="action_B"/>
            </SequenceStar>
        </Timeout>
     </BehaviorTree>
 </root>
 )";

// clang-format on

int main()
{
    // Simple tree: a sequence of two asycnhronous actions,
    // but the second will be halted because of the timeout.

    BehaviorTreeFactory factory;
    factory.registerNodeType<MyAsyncAction>("MyAsyncAction");

    auto tree = factory.createTreeFromText(xml_text);

    //---------------------------------------
    // keep executing tick until it returns either SUCCESS or FAILURE
    while( tree.tickRoot() == NodeStatus::RUNNING)
    {
        std::cout <<"main thread id : "<< std::this_thread::get_id() << std::endl;
        tree.sleep( std::chrono::milliseconds(10) );
    }
    return 0;
}