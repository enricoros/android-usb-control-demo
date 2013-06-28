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

package com.enricoros.controllerdemo;

import java.io.FileOutputStream;
import java.io.IOException;

import android.content.Context;

import com.enricoros.controllerdemo.AndroidAccessoryBridge;
import com.enricoros.controllerdemo.utils.Logger;

public class ControllerOutputUSB extends AndroidAccessoryBridge {

	private static final int USB_PACKET_SIZE = 4;

	private final byte[] mSendBuffer;

	public ControllerOutputUSB(Context context) {
		super(context);
		mSendBuffer = new byte[USB_PACKET_SIZE];
	}

	/**
	 * Controls the servos on channel 1 and 2
	 *
	 * @param fwdSpeed forward speed; -1: max back: 0: stop, 1: max forward
	 * @param steerPos wheels direction; -1: all left, 0: center, 1: all right
	 */
	public void controlMotion(float fwdSpeed, float steerPos) {
		fwdSpeed = floatSuppress(-0.05f, floatBound(-1f, fwdSpeed, 1f), 0.05f, 0);
		steerPos = floatSuppress(-0.05f, floatBound(-1f, steerPos, 1f), 0.05f, 0);

		final int fwdVal = Math.round(fwdSpeed * 100f + 100f);
		final int steerVal = Math.round(steerPos * 100f + 100f);

		sendCommand4((byte)0x02, (byte)0x01, fwdVal, steerVal);
	}

	/* Private stuff ahead */

	private float floatBound(float min, float val, float max) {
		if (val > max)
			return max;
		if (val < min)
			return min;
		return val;
	}

	private float floatSuppress(float from, float val, float to, float dest) {
		if (val > from && val < to)
			return dest;
		return val;
	}

	private boolean sendCommand4(byte command, byte target, int value1, int value2) {
		if (value1 > 255)
			value1 = 255;
		if (value2 > 255)
			value2 = 255;

		mSendBuffer[0] = command;
		mSendBuffer[1] = target;
		mSendBuffer[2] = (byte) value1;
		mSendBuffer[3] = (byte) value2;

		FileOutputStream out = getOutputStream();
		if (out != null && mSendBuffer[1] != -1) {
			try {
				out.write(mSendBuffer, 0, USB_PACKET_SIZE);
				return true;
			} catch (IOException e) {
				Logger.exception("ControllerOutputUSB: USB write failed", e);
			}
		}
		return false;
	}

}
