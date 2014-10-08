/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author ics
 */
public class TestJNI {

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
            LibApexArinc653Jni.pSendQueuingMessage(name, port, sock, emetteur, msg);
            try {
                Thread.sleep(1000);
            } catch (InterruptedException ex) {
                Logger.getLogger(LibApexArinc653Jni.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

}
