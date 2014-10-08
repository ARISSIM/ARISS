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

import java.util.Vector;

/**
 *
 * @author Camille Fayollas
 */
public class CommunicationVector {
    private String sender;
    private Vector samplingSockets = new Vector(10, 2);
    private Vector queuingSockets = new Vector(10, 2);
    private Vector samplingPorts = new Vector(10, 2);
    private Vector queuingPorts = new Vector(10, 2);

    CommunicationVector(String sender) {
        this.setSender(sender);
    }
    
    CommunicationVector() {
        this.setSender("");
    }
            
    public void setSender(String sender) {
        this.sender = sender;
    }

    public void addSamplingSocket(int sock){
        samplingSockets.add(sock);
    }
    
    public void addQueuingSocket(int sock){
        queuingSockets.add(sock);
    }
    
    public void addSamplingPort(int port){
        samplingPorts.add(port);
    }
    
    public void addQueuingPort(int port){
        queuingPorts.add(port);
    }
    
    public int getSamplingSocket(int index){
        return (int)samplingSockets.elementAt(index);
    }
    
    public int getQueuingSocket(int index){
        return (int)queuingSockets.elementAt(index);
    }
    
    public int getSamplingPort(int index){
        return (int)samplingPorts.elementAt(index);
    }
    
    public int getQueuingPort(int index){
        return (int)queuingPorts.elementAt(index);
    }

    public String getSender() {
        return sender;
    }
}