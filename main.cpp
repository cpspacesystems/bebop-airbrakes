//#include <iostream>
//#include <cmath>
//#include <Arduino.h>
//#include "../BMI08x-Sensor-API/"
//#include <TeensyThreads.h>
//#include "telemetry_packet.h"
// We need to come up with solutions on how every module works and how it needs to be connected

using namespace std;

enum Stage {
    PAD,
    BURN,
    COAST,
    RDEPLOYMENT,
    RECOVERY,
    GROUNDED
};

bool enableAdjustments = true;
double currentDisplacement = 0.0;

class Vector {
public:
    double velo;
    double dx;
    double dy;
    double dz;

    Vector(double v, double x, double y, double z) : velo(v), dx(x), dy(y), dz(z) {}
};

class Data {
public:
    double velo;
    double ax;
    double ay;
    double az;

    Data(double v, double x, double y, double z) : velo(v), ax(x), ay(y), az(z) {}
};

Data getData() {
    // Implement data acquisition logic here
    double velo, ax, ay, az;
    // Read sensor data
    // Assign values to velo, dx, dy, dz
    return Data(velo, ax, ay, az);
}

Stage getStage(Data data) {
    // Implement stage detection logic here
    if (data.velo > 0) {
        return BURN;
    }
    if (data.velo < 0 && data.velo > 0) {
        return COAST;
    }
    if (data.velo < 0 && data.velo < 0) {
        return RECOVERY;
    }
    return PAD;
}

void saveData(bool active, Data data) {
    if (active) {
        // Implement data saving logic here
    }
}

void sendData(bool active, Data data) {
    if (active) {
        // Implement data sending logic here
    }
}


void addressActuator(double deltaX) {
    if (enableAdjustments) {
        // Implement logic to move the actuator
        // How does the actuator recieve data
    }
}
void retractAirbrakes(){
    //retract airbrakes
}

double getApogee(Vector vectV) {
    // Implement apogee look up table logic here
    // Return apogee height
    return 0.0;
}

double getRequiredDifference(Vector vectV, double currentApogee) {
    // Implement calculation for theta
    const double dx = 0.0; // Calculate theta here
    currentDisplacement = currentDisplacement + dx;
    return dx;
}


void deployAirBrakes() {
    //linear actuator entire deployment
    //addressActuator(dx);
}


int main() {
    bool run = true;
    while (run) {
        Data data = getData();
        saveData(run, data);
        sendData(run, data);
        Stage stage = getStage(data);

        while (stage == PAD) {
            // Implement logic for PAD stage
        }
        while (stage == BURN) {
            // Implement logic for BURN stage
        }
        while (stage == COAST) {
            // Implement logic for COAST stage
            Vector vectV(data.velo, data.ax, data.ay, data.az);
            double currentApogee = getApogee(vectV);
            double theta = getRequiredDifference(vectV, currentApogee);
            double beta = theta - currentDisplacement;
            addressActuator(beta);
        }
        while (stage == RDEPLOYMENT) {
            // Implement logic for Recovery Deployment stage
            deployAirBrakes();
        }
        while (stage == RECOVERY) {
            // Implement logic for Recovery stage
            enableAdjustments = false;
            // read altimeter data and undeploy 10 feet above ground
            enableAdjustments = true;
            retractAirbrakes();
            run = false; // For testing purposes, set run to false immediately
        }
    }
    return 0;
}
