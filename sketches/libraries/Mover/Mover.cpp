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

#include "Mover.h"

#define BOARD_SERVOS        6
#define PIN_CONTROL_0       46
#define PIN_CONTROL_1       44
#define PIN_CONTROL_2       42
#define PIN_CONTROL_3       40
#define PIN_CONTROL_4       38
#define PIN_CONTROL_5       36
#define PIN_CONTROL_REFGND  48

Mover::Mover()
    : m_vEngine(0)
    , m_vSteer(0)
    , m_vCameraPitch(0)
    , m_vCameraYaw(0)
    , m_vExt1(0)
    , m_vExt2(0)
    , m_mulEngine(1.0)
    , m_mulSteer(1.0)
{
}

void Mover::init()
{
    // set the ground pin to LOW
    pinMode(PIN_CONTROL_REFGND, OUTPUT);
    digitalWrite(PIN_CONTROL_REFGND, LOW);

    // attach
    m_sEngine.attach(PIN_CONTROL_0);
    m_sSteer.attach(PIN_CONTROL_1);
    m_sCameraPitch.attach(PIN_CONTROL_4);
    m_sCameraYaw.attach(PIN_CONTROL_5);
    m_sExt1.attach(PIN_CONTROL_2);
    m_sExt2.attach(PIN_CONTROL_3);

    // write initial values
    setSpeed(m_vEngine);
    setSteering(m_vSteer);
    setCameraPitch(m_vCameraPitch);
    setCameraYaw(m_vCameraYaw);
    setExt1(m_vExt1);
    setExt2(m_vExt2);
}

void Mover::setSpeedMultipler(float m)
{
    if (m > 0.01 && m < 2.0)
        m_mulEngine = m;
}


void Mover::setSteeringMultipler(float m)
{
    if (m > 0.01 && m < 2.0)
        m_mulSteer = m;
}

void Mover::setSpeed(float v)
{
    if (v != m_vEngine) {
        m_vEngine = v;
        if (m_mulEngine != 1.0)
            v *= m_mulEngine;
        m_sEngine.write(normalToServo(-v));
    }
}

void Mover::setSteering(float v)
{
    if (v != m_vSteer) {
        m_vSteer = v;
        if (m_mulSteer != 1.0)
            v *= m_mulSteer;
        m_sSteer.write(normalToServo(v));
    }
}

void Mover::setCameraPitch(float v)
{
    if (v != m_vCameraPitch) {
        m_vCameraPitch = v;
        m_sCameraPitch.write(normalToServo(v));
    }
}

void Mover::setCameraYaw(float v)
{
    if (v != m_vCameraYaw) {
        m_vCameraYaw = v;
        m_sCameraYaw.write(normalToServo(v));
    }
}

void Mover::setExt1(float v)
{
    if (v != m_vExt1) {
        m_vExt1 = v;
        m_sExt1.write(normalToServo(v));
    }
}

void Mover::setExt2(float v)
{
    if (v != m_vExt2) {
        m_vExt2 = v;
        m_sExt2.write(normalToServo(v));
    }
}

int Mover::normalToServo(float n) const
{
    const int s = round(n * 90.0f);
    if (s <= -90)
        return 0;
    if (s >= 90)
        return 180;
    return s + 90;
}
