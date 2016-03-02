# TODO

* When DUT initialize data, the each test step info m_bExecuted should be false;
* if m_bExecuted is false, the test info and result are not guranteed. 
* Before one test step execute, DO NOT need to check if pre-condition test steps are executed. 
* Before one test step try to use the other test info and result, must check the m_bExecuted. 