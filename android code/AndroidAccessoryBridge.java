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

import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;

import android.content.Context;
import android.content.Intent;
import android.hardware.usb.UsbAccessory;
import android.hardware.usb.UsbManager;
import android.os.ParcelFileDescriptor;
import android.os.Parcelable;

import com.enricoros.controllerdemo.utils.Logger;

public class AndroidAccessoryBridge implements Runnable {

	private final UsbManager mUsbManager;

	private UsbAccessory mAccessory;

	private ParcelFileDescriptor mParcelFileDescriptor;
	private FileInputStream mInputStream;
	private FileOutputStream mOutputStream;

	public AndroidAccessoryBridge(Context context) {
	mUsbManager = (UsbManager) context.getSystemService(Context.USB_SERVICE);

	listenForPluggedAccessories();		
	}

	public boolean tryUseAccessoryFromIntent(Intent intent) {
		if (intent == null)
			return false;

	Parcelable parcelable = intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
	if (parcelable == null)
		return false;

		UsbAccessory accessory = (UsbAccessory) parcelable;
		boolean permissionGranted = intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false);
		Logger.info("AndroidAccessoryBridge: passed UsbAccessory with intent (permission granted: "+permissionGranted+")");
		setAccessory(accessory, permissionGranted);
		return true;
	}

	public void setAccessory(UsbAccessory accessory, boolean permissionGranted) {
		if (mAccessory != null)
			closeAccessory();
		mAccessory = accessory;
		if (mAccessory != null)
			openAccessory();
	}	

	public void listenForPluggedAccessories() {
		// TODO Auto-generated method stub
	/*
		mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
		IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
		filter.addAction(usbHandler.get_ACTION_USB_ACCESSORY_DETACHED());
		registerReceiver(mUsbReceiver, filter);
	*/	

	}
	
	protected FileInputStream getInputStream() {
		return mInputStream;
	}
	
	protected FileOutputStream getOutputStream() {
		return mOutputStream;
	}

	// NOTE: we assume we have permission to open the accessory here
	private void openAccessory() {
		// sanity check
		if (mAccessory == null) {
			Logger.error("AndroidAccessoryBridge.openAccessory: cannot open a null accessory. accessory disabled.");
			return;
		}
	
		// need to keep this - otherwise GC will cause interruption of the comm in the file descs
		mParcelFileDescriptor = mUsbManager.openAccessory(mAccessory);
		if (mParcelFileDescriptor == null) {
			Logger.error("AndroidAccessoryBridge.openAccessory: cannot get a parcel file descriptor. accessory disabled.");
			return;
		}

		FileDescriptor fd = mParcelFileDescriptor.getFileDescriptor();
		if (fd == null) {
			Logger.error("AndroidAccessoryBridge.openAccessory: cannot get a file descriptor. accessory disabled.");
			return;
		}

		// create the I/O streams on the Accessory
		mInputStream = new FileInputStream(fd);
		mOutputStream = new FileOutputStream(fd);

		// start the internal thread (See @run())
		//Thread thread = new Thread(null, this, "AccessoryThread");
		//thread.start();
	}

	private void closeAccessory() {
		// TODO Auto-generated method stub
	}


	@Override
	public void run() {
		// This is used as the entry point for the Thread too
		// TODO ...		
	}

}
