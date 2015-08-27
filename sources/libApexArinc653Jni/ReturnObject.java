/*
 * Copyright 2015 ics.
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


/**
 *
 * @author Camille Fayollas
 */
public class ReturnObject{
    private int returnCode;
    private String sender;
    private int length;
    private String message;

    ReturnObject(int returnCode, String sender, int length, String message) {
        this.setReturnCode(returnCode);
        this.setSender(sender);
        this.setLength(length);
        this.setMessage(message);
    }
    
    ReturnObject() {
        this(0,"",0,"");
    }
    
    public void setSender(String sender) {
        this.sender = sender;
    }

    public void setLength(int length) {
        this.length = length;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public String getSender() {
        return sender;
    }

    public int getLength() {
        return length;
    }

    public String getMessage() {
        return message;
    }

    public int getReturnCode() {
        return returnCode;
    }

    public void setReturnCode(int returnCode) {
        this.returnCode = returnCode;
    }
    
    
}