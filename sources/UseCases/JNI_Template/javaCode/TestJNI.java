/*
* To test : 
*   1) compile TestJNI.java (command = javac TestJNI.java)
*   2) copy TestJNI.class and LibApexArinc653Jni.class in binary/UsesCases/JNI_Template/
*   3) copy libApexArinc653Jni.so in /usr/lib/jvm/java-8-oracle/jre/bin/
*   4) Dans un terminal, se placer dans le useCase : ARISS/binary/UseCases/JNI_Template
*   5) exporter la variable correspondant au chemin des librairies : commande "export LD_LIBRARY_PATH=/home/ics/ARISS/ARISS/lib"
*/


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Camille Fayollas
 */
public class TestJNI {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws InterruptedException {
        System.out.println("Init Partition 1");
        String name = args[0];
        System.out.println("name = " + name);
        int port = Integer.parseInt(args[1]);
        System.out.println("port = " + port);
        int sock = Integer.parseInt(args[2]);
        System.out.println("sock = " + sock);
        String emetteur = args[3];
        System.out.println("emetteur = " + emetteur);
        String emetteurR = "", msgR = "";
        int lengthR = 0;
        int i = 0;
        while(true){
            System.out.println("<<<< Sendind message n° " + i + ">>>>");
            String msg = "New message n° " + i;
            LibApexArinc653Jni.pSendQueuingMessage(name, port, sock, emetteur, msg);
            int ifMessage = LibApexArinc653Jni.pReceiveQueuingMessage(sock, emetteurR, lengthR, msgR);
            if (ifMessage > 0){
                System.out.println("New queuing message");
            }else{
                System.out.println("No new message");
            }
            Thread.sleep(1000);
            i++;
        }
    }

}
