import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.Vector;

/**
 *
 * @author Camille Fayollas
 */

class TypeMessage{
    private String sender;
    private int length;
    private String message;

    TypeMessage(String sender, int length, String message) {
        this.setSender(sender);
        this.setLength(length);
        this.setMessage(message);
    }
    
    TypeMessage() {
        this("",0,"");
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
}

class CommunicationVector{
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
}

public class LibApexArinc653Jni {

    static {
        System.load("/home/ics/ARINC_653_NB/ARINC653-simulator/lib/libApexArinc653Jni.so");
    }

    //APEX ARINC 653 sampling communication
    private native int writeSamplingMessage(String jName, int jPortId, int jSock, String jEmetteur, String jMessage);
    private native int readSamplingMessage(int sock, TypeMessage rMessage);

    //APEX ARINC 653 queuing communication
    private native int sendQueuingMessage(String jName, int jPortId, int jSock, String jEmetteur, String jMessage);
    private native int receiveQueuingMessage(int sock, TypeMessage rMessage);
    
    //APEX ARINC 653 partition initialization
//    private native CommunicationVector initCommunication(String arg, String mode);
//    private native int guiArincPartition(String partitionName, int position, int restart);

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        System.out.println("Init Partition 1");
        String name = args[0];
        System.out.println("name = " + name);
        int port = Integer.parseInt(args[1]);
        System.out.println("port = " + port);
        int sock = Integer.parseInt(args[2]);
        System.out.println("sock = " + sock);
        String emetteur = args[3];
        System.out.println("emetteur = " + emetteur);
        int i = 0;
        for (i = 0; i < 20; i++) {
            System.out.println("<<<< Sendind message n° " + i + ">>>>");
            String msg = "New message n° " + i;
            new LibApexArinc653Jni().sendQueuingMessage(name, port, sock, emetteur, msg);
            try {
                Thread.sleep(1000);
            } catch (InterruptedException ex) {
                Logger.getLogger(LibApexArinc653Jni.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

}
