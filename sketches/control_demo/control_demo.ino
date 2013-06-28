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

#include <AndroidAccessory.h>
#include <Servo.h>
#include <Mover.h>

AndroidAccessory acc(
  "EnricoRos", "ControlDemo",
  "Cool Controller Board",
  "1.0",
  "http://www.google.com");

Mover mover;

#define USB_MSG_LEN 4
byte recMsg[USB_MSG_LEN];

void setup() {
  delay(1000);

  Serial.begin(115200);
  Serial.write("hello\n");

  pinMode(A0, INPUT);  

  // init USB Host
  acc.begin();

  // init Mover
  mover.setSpeedMultipler(0.3);
  mover.setSteeringMultipler(0.8);
  mover.init();

  // wait for the Mover to stabilize
  delay(2000);
}

void loop() {
  // run USB event loop
  acc.refresh();

  // when connected, read USB commands
  if (acc.isConnected())
    parseUsbMessage();
    
  // .. do more here ..
}

void parseUsbMessage() {
  recMsg[0] = 0;
  acc.readBytes((char *) recMsg, USB_MSG_LEN);
  byte rCmd    = recMsg[0];
  byte rTarget = recMsg[1];
  byte rValue1 = recMsg[2];
  byte rValue2 = recMsg[3];

  switch (rCmd) {
    // C: 00 -> Ignore
    case 0x00:
      break;

    // C: 01 -> set value for target
    case 0x01: {
      float n = ((float)rValue1 - 100.0f) / 100.0f;
      if (rTarget == 0x01)
        mover.setSpeed(n);
      else if (rTarget == 0x02)
        mover.setSteering(n);
      else if (rTarget == 0x03)
        mover.setCameraPitch(n);
      else if (rTarget == 0x04)
        mover.setCameraYaw(n);
      } break;

    // C: 02 -> set traction pair
    case 0x02: {
      float nSp = ((float)rValue1 - 100.0f) / 100.0f;
      float nSt = ((float)rValue2 - 100.0f) / 100.0f;
      mover.setSpeed(nSp);
      mover.setSteering(nSt);
      } break;

    // C: 03 -> set camera pair
    case 0x03: {
      float nPitch = ((float)rValue1 - 100.0f) / 100.0f;
      float nYaw = ((float)rValue2 - 100.0f) / 100.0f;
      mover.setCameraPitch(nPitch);
      mover.setCameraYaw(nYaw);
      } break;
  }
}
