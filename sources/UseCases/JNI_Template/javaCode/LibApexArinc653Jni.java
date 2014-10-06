
import java.util.logging.Level;
import java.util.logging.Logger;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author ics
 */
public class LibApexArinc653Jni {

    static {
        System.load("/home/ics/ARINC_653_NB/ARINC653-simulator/lib/libApexArinc653Jni.so");
    }

    //APEX ARINC 653 sampling communication
    private native int writeSamplingMessage(String jName, int jPortId, int jSock, String jEmetteur, String jMessage);
//    int READ_SAMPLING_MESSAGE(int sock, Type_Message *rMessage);

    //APEX ARINC 653 queuing communication
    private native int sendQueuingMessage(String jName, int jPortId, int jSock, String jEmetteur, String jMessage);
//    int RECEIVE_QUEUING_MESSAGE(int sock, Type_Message *rMessage);

    //APEX ARINC 653 partition initialization
//    int GUI_ARINC_partition(char* name_partition, int position, int redemarrage);
//    COMMUNICATION_VECTOR init_communication(int nbarg, char* argument[], char** mode);
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
