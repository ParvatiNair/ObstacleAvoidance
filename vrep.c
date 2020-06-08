#include "vrep.h"

// for more info refer to 
// https://www.coppeliarobotics.com/helpFiles/en/legacyRemoteApiOverview.htm 

int main(int argc,char* argv[])
{

    // all handles that will be passed from lua to this code
    int portNb=0;
    int botHandle;
    int leftMotorFrontHandle;
    int rightMotorFrontHandle;
    int leftMotorBackHandle;
    int rightMotorBackHandle; 
    int sensorHandle;
    
    // number of variables passed from lua to this code + 1
    // (+1 because the first value in argv is always the location of this program)
    int n = 8;

    // save the handles
    if (argc>=n){
        portNb=atoi(argv[1]);
        botHandle = atoi(argv[2]);
        leftMotorFrontHandle = atoi(argv[3]);
        rightMotorFrontHandle = atoi(argv[4]);
	    leftMotorBackHandle = atoi(argv[5]);
        rightMotorBackHandle = atoi(argv[6]);
        sensorHandle = atoi(argv[7]);
    }
    else{
        printf("Sufficient arguments not provided'!\n");
        extApi_sleepMs(5000);
        return 0;
    }

    // variables for the code
    
    float leftMotorFrontSpeed = 1;
    float rightMotorFrontSpeed = 1;
    float leftMotorBackSpeed = 1;
    float rightMotorBackSpeed = 1;
    float* detectedPoint = NULL;
    int* detectedObjectHandle = NULL;
    float* detectedSurfaceNormalVector = NULL;

    // functions or variable names starting with 'simx' are coppeliamsim functions
    // for details on these functions refer: https://www.coppeliarobotics.com/helpFiles/en/remoteApiFunctions.htm

    // start communication with coppeliasim
    int clientID=simxStart((simxChar*)"127.0.0.1",portNb,true,true,2000,5);

    if (clientID!=-1){
        printf("connection established\n");
    
        /******************** SETUP ******************************/
        // write code here that you wish to perform ONCE
        
        simxSetJointTargetVelocity(clientID,leftMotorFrontHandle,leftMotorFrontSpeed,simx_opmode_oneshot);
        simxSetJointTargetVelocity(clientID,rightMotorFrontHandle,rightMotorFrontSpeed,simx_opmode_oneshot);
	    simxSetJointTargetVelocity(clientID,leftMotorBackHandle,leftMotorBackSpeed,simx_opmode_oneshot);
        simxSetJointTargetVelocity(clientID,rightMotorBackHandle,rightMotorBackSpeed,simx_opmode_oneshot);
        simxReadProximitySensor(clientID, sensorHandle, NULL, detectedPoint, detectedObjectHandle,detectedSurfaceNormalVector,simx_opmode_streaming);
        /*********************************************************/
        
        while (simxGetConnectionId(clientID)!=-1){  
            /************************ LOOP *****************************/
            // write code here that you wish to REPEAT
           
            if( simxReadProximitySensor(clientID, sensorHandle, NULL, detectedPoint, detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_buffer) == simx_return_ok){
                printf("sensor: %f %f %f  \n", &detectedPoint);
            }
            /***********************************************************/
        }
        
        // end communication with lua
        simxFinish(clientID);

    }
    printf("Simulation Stopped!");
}
