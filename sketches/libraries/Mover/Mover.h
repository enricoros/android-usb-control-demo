/*
 * Copyright (c) 2013, Enrico Ros
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MOVER_H
#define MOVER_H

#include <Servo.h>
#include <Arduino.h>

/** This class encapsulates the motor control of Our Board
 *
 */
class Mover
{
public:
    Mover();

    /* limits (active when setting the next setpoint) */

    void setSpeedMultipler(float m);
    float getSpeedMultiplier() const { return m_mulEngine; }

    void setSteeringMultipler(float m);
    float getSteeringMultiplier() const { return m_mulSteer; }

    /// Connect to the hardware pins. Call once
    void init();

    /* set points */

    void setSpeed(float v);
    float getSpeed() const { return m_vEngine; }

    void setSteering(float v);
    float getSteering() const { return m_vSteer; }

    void setCameraPitch(float v);
    float getCameraPitch() const { return m_vCameraPitch; }

    void setCameraYaw(float v);
    float getCameraYaw() const { return m_vCameraYaw; }

    /* unused */

    void setExt1(float v);
    float getExt1() const { return m_vExt1; }

    void setExt2(float v);
    float getExt2() const { return m_vExt2; }

private:
    Servo m_sEngine;
    Servo m_sSteer;
    Servo m_sCameraPitch;
    Servo m_sCameraYaw;
    Servo m_sExt1;
    Servo m_sExt2;

    float m_vEngine;
    float m_vSteer;
    float m_vCameraPitch;
    float m_vCameraYaw;
    float m_vExt1;
    float m_vExt2;

    float m_mulEngine;
    float m_mulSteer;

    int normalToServo(float n) const;
};

#endif
