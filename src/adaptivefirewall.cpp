#include "adaptivefirewall.h"

int main(int argc, char* argv[])
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
    PolicyApplier* pa = new PolicyApplier(std::string(CRITERION_DIRECTORY), std::string(POLICY_DIRECTORY));
    
    Log::getInstance()->setPath(std::string(LOG_FILE));
    std::string test = "Adaptive Firewall Started";
    Log::getInstance()->write(test);
    
    while(1)
    {
        pa->loop();
        sleep(10);
    }
    
	return 0;
}
