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
    float dist;
    // number of variables passed from lua to this code + 1
    // (+1 because the first value in argv is always the location of this program)
    int n = 8;
    //int i=0;
    int a=1;
    int c=2;
    int e=6;
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
    float position[3] = {0,0,0};
    float leftMotorFrontSpeed = 1;
    float rightMotorFrontSpeed = 1;
    float leftMotorBackSpeed = 1;
    float rightMotorBackSpeed = 1;
    float* detectedPoint = NULL;
    int* detectedObjectHandle = NULL;
    float* detectedSurfaceNormalVector = NULL;
    unsigned char *detectionstate=0;
    int* b= &a;
    int* d= &c;
    int* f= &e;
    int* z= &sensorHandle;
    // functions or variable names starting with 'simx' are coppeliamsim functions
    // for details on these functions refer: https://www.coppeliarobotics.com/helpFiles/en/remoteApiFunctions.htm

    // start communication with coppeliasim
    int clientID=simxStart((simxChar*)"127.0.0.1",portNb,true,true,2000,5);

    if (clientID!=-1){
        printf("connection established\n");

        /******* SETUP ***********/
        // write code here that you wish to perform ONCE
        simxGetObjectPosition(clientID, botHandle, -1, position, simx_opmode_streaming);
        simxSetJointTargetVelocity(clientID,leftMotorFrontHandle,leftMotorFrontSpeed,simx_opmode_oneshot);
        simxSetJointTargetVelocity(clientID,rightMotorFrontHandle,rightMotorFrontSpeed,simx_opmode_oneshot);
	    simxSetJointTargetVelocity(clientID,leftMotorBackHandle,leftMotorBackSpeed,simx_opmode_oneshot);
        simxSetJointTargetVelocity(clientID,rightMotorBackHandle,rightMotorBackSpeed,simx_opmode_oneshot);
        simxReadProximitySensor(clientID, sensorHandle, NULL, detectedPoint, detectedObjectHandle,detectedSurfaceNormalVector,simx_opmode_streaming);
        simxGetObjectGroupData(clientID, sim_object_proximitysensor_type, 13, b, &z, d, &detectedObjectHandle, f, &detectedPoint, NULL,NULL,simx_opmode_streaming);
        /*******************/

        while (simxGetConnectionId(clientID)!=-1){
            //i++;  
            /******** LOOP ***********/

            // write code here that you wish to REPEAT
		simxGetObjectPosition(clientID, botHandle, -1, position, simx_opmode_streaming);
        simxSetJointTargetVelocity(clientID,leftMotorFrontHandle,leftMotorFrontSpeed,simx_opmode_oneshot);
        simxSetJointTargetVelocity(clientID,rightMotorFrontHandle,rightMotorFrontSpeed,simx_opmode_oneshot);
	    simxSetJointTargetVelocity(clientID,leftMotorBackHandle,leftMotorBackSpeed,simx_opmode_oneshot);
        simxSetJointTargetVelocity(clientID,rightMotorBackHandle,rightMotorBackSpeed,simx_opmode_oneshot);
        simxReadProximitySensor(clientID, sensorHandle, NULL, detectedPoint, detectedObjectHandle,detectedSurfaceNormalVector,simx_opmode_streaming);
        simxGetObjectGroupData(clientID, sim_object_proximitysensor_type, 13, b, &z, d, &detectedObjectHandle, f, &detectedPoint, NULL,NULL,simx_opmode_streaming);

            if( simxGetObjectPosition(clientID, botHandle, -1, position, simx_opmode_buffer) == simx_return_ok)
                printf("the pos: %f %f %f\n",position[0],position[1],position[2]);
            if( simxReadProximitySensor(clientID, sensorHandle, detectionstate, detectedPoint, detectedObjectHandle, detectedSurfaceNormalVector, simx_opmode_buffer) == simx_return_ok){
                //printf("sensor: %f %f %f  \n", detectedPoint[0],detectedPoint[1],detectedPoint[2]);
                dist= sqrt(detectedPoint[0]*detectedPoint[0]+detectedPoint[1]*detectedPoint[1]+detectedPoint[2]*detectedPoint[2]);
                printf("Distance:%f\n", dist);
                //printf("state: %u\n",detectionstate);
                //printf("Object handle:%f\n", detectedObjectHandle);
            }
            /*********************/
            //printf("%d\n",i);
        }

        // end communication with lua
        simxFinish(clientID);

    }
    printf("Simulation Stopped!");
    
}
