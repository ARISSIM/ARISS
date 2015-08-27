/*
 * Copyright 2014 ics.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.Vector;

/**
 *
 * @author Camille Fayollas
 */
public class LibApexArinc653Jni {

    static {
        System.load("/home/ics/ARISS/ARISS/lib/libApexArinc653Jni.so");
    }

    //APEX ARINC 653 sampling communication
    private native int writeSamplingMessage(String jName, int jPortId, int jSock, String jEmetteur, String jMessage);
    private native ReturnObject readSamplingMessage(int jSock);

    //APEX ARINC 653 queuing communication
    private native int sendQueuingMessage(String jName, int jPortId, int jSock, String jEmetteur, String jMessage);
    private native ReturnObject receiveQueuingMessage(int jSock);
    
    //APEX ARINC 653 partition initialization
//    private native CommunicationVector initCommunication(String arg, String mode);
//    private native int guiArincPartition(String partitionName, int position, int restart);
    
    /**
     *
     * @param name
     * @param portId
     * @param sock
     * @param sender
     * @param message
     * @return
     */
    public static int pWriteSamplingMessage(String name, int portId, int sock, String sender, String message) {
        LibApexArinc653Jni lib = new LibApexArinc653Jni();
        int toReturn = lib.writeSamplingMessage(name, portId, sock, sender, message);
        return toReturn;
    }
    
    /**
     *
     * @param sock
     * @return
     */
    public static ReturnObject pReadSamplingMessage(int jSock) {
        LibApexArinc653Jni lib = new LibApexArinc653Jni();
        ReturnObject toReturn = lib.readSamplingMessage(jSock);
        return toReturn;
    }

    /**
     *
     * @param name
     * @param portId
     * @param sock
     * @param sender
     * @param message
     * @return
     */
    public static int pSendQueuingMessage(String name, int portId, int sock, String sender, String message) {
        LibApexArinc653Jni lib = new LibApexArinc653Jni();
        int toReturn = lib.sendQueuingMessage(name, portId, sock, sender, message);
        return toReturn;
    }    
    
    /**
     *
     * @param sock
     * @return
     */    
    public static ReturnObject pReceiveQueuingMessage(int jSock) {
        LibApexArinc653Jni lib = new LibApexArinc653Jni();
        ReturnObject toReturn = lib.receiveQueuingMessage(jSock);
        return toReturn;
    }
    
//    /**
//     *
//     * @param arg
//     * @param mode
//     * @return
//     */
//    public static CommunicationVector pInitCommunication(String arg, String mode) {
//        LibApexArinc653Jni lib = new LibApexArinc653Jni();
//        CommunicationVector cVector = lib.initCommunication(arg, mode);
//        return cVector;
//    }
//    
//    
//    /**
//     *
//     * @param partitionName
//     * @param position
//     * @param restart
//     * @return
//     */
//    public static int pGuiArincPartition(String partitionName, int position, int restart) {
//        LibApexArinc653Jni lib = new LibApexArinc653Jni();
//        int toReturn = lib.guiArincPartition(partitionName, position, restart);
//        return toReturn;
//    }
}