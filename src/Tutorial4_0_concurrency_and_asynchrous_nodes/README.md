Concurrency 와 Parallelism 요약
-> 둘 다 여러개의 Task를 처리할 수 있다는 개념을 포함하기에 헷갈릴 수 있음. 
-> Concurrency는 Time period를 쪼개 한번에 2개 이상의 Task를 처리할 수 있다.
-> Parallelism 은 말 그대로, 고유의 Thread에서(멀티 프로세서로) 동시에 처리.

=> 공통점 : 여러 가지의 일을 한번에 처리한다.
=> 차이점 : x축을 시간 , y축을 Task양 이라고 할 때, Concurrency는 y축의 값이 1이고, Parallelism은 2 이상일 것이다. 



BT.CPP executes all the nodes Concurrently
즉, 
- Tree는 Single-Threaded 로 동작.
- all the tick() methods are always executed sequentially.
- if any tick() method is blocking, the entire execution is blocked.

결과적으로 실행 시간이 오래걸리는 Action은 action이 시작했음을 알리기 위한 RUNNING 상태를 알려야하며, 다시 tick 됐을 때 action이 완료됐는지 확인한다. 이 역할을 Asynchronous node가 해준다.
An Asynchronous node may delegate this long execution either to another process, another server or simply another thread.



