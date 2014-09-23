This file explains briefly how the AGILE Satellite Simulator works.
It is based on the last version developped during 2014 Projet Long.
This document was written on 11/03/2014.
For a more complete description, please refer to the associated Projet Long Report and the Doxygen Documentation (/ReadMe/Chaire_CESEC_Report).


¤ The simulator itself can be obtained using the GIT repository (to clone, git clone git@trac.laas.fr:ARINC653_simulator) or on the Raspberry PI at /home/pi/Desktop/Code/ARINC653_simulator.
The code source of the simulator is located in the /sources folder.
Every class and partition has its code more precisely in the folder : /sources/Chaire_SE_Student
In this directory, every class is at the root whereas every partition's code is in others folders. Currently, 4 partitions are used and therefore we can list 4 directories in /sources/Chaire_SE_Student : Master, Slave, Leica and Scao.
The 4 remaining folders are special purpose folders:
 - Configuration: contains configuration files of the simulator (communication and time allocated to partitions)
 - plans: where the plans are loaded. There is always a file called "DefaultPlan.txt" (WARNING, the simulator is case sensitive !) and the new plans called "NewPlan.txt" must be copied there. Once they are loaded in the simulator, these new plans are renamed to "OldPlan.txt".
 - PlanGenerator: contains the code necessary to create new plan. Copy this folder to your computer, simply execute "make" and run the created executable. It will ask you the delay before the creation of the plan and the number of instructions (limited to 50 by the simulator). Then a "NewPlan.txt" file will be generated and will be send to the Raspberry PI using SSH. Depending on how you access to the Raspberry PI, you must modify the last lines of the "PGen.cpp" file (lines "system(scp [...])") to match the current IP Address of the Raspberry PI. The general syntax is : scp NewPlan.txt pi@RASPBERRY_PI_IP_ADRESS_OR_NAME:/home/pi/Desktop/Code/ARINC653_simulator/sources/Chaire_SE_Student/plans
 - TERRE : contains the images which were used on the Master Copy version when we didn't have any camera. These images are still used when the simulator detects that it is launched on a PC (and so that it does not have access to the camera).
 - Ground_Station : symbolizes the ground station. It must be copied to a remote computer. It contains a script "display.h" which uncompress and display the received images. The .zip files are then move to the folder Old. Note that 2 versions of the script are available: 1 for Linux and another for MAC. Once executed, the script waits to receive new compressed images in an infinite loop. 
 
==> To conclude, only the "plans" and "PlanGenerator" folders must be handled by the users when using the simulator.

Note: The photographs are currently transfered to a distant PC using the "CarteComm.cpp object. To receive these images, simply modify the user and the IP address in the function "CarteComm::envoyer" at the line "strcat(commande_envoi, ...)".

¤ To launch the simulator, execute the "make" command at the root of the simulator. The two folders "binary" and "build" will then be created. Next, change directory to /binary/Chaire_SE_Student and execute "./simulateur.out". The simulator is now launched and will execute by default the file "DefaultPlan.txt". Then use "PGen" (see above) to send and make load a new plan.
At the first launch, the directories "camera_memory" and "stable_memory" are created. "camera_memory" represents the temporal memory of the camera and "stable_memory" is the memory of the Leica partition. This is where the images will be stocked and compressed before their sending to the ground station. By the way, if the simulator detects that it is executed on a PC, a folder called "ground_station" is also created in which one the images will be copied instead of sending them to a remote computer.

¤ In the last configuration, the Wi-Fi dongle plugged into the Raspberry PI is used to create an AdHoc Wi-Fi network. It is called "AdHocRasPI" (case sensitive) and the Raspberry PI can be joined at 192.168.1.1 on this network. Simply connect to this network using your computer and then connect to the Raspberry PI with the command "ssh -YC 192.168.1.1 -l pi" ("-YC" is used to enable display control.)
You can also use a VNC client to visualize the Raspberry PI Desktop on your computer screen. Simply execute the command "vncserver :x" on the Raspberry PI (after logging in via ssh). Then, open your favorite VNC client and use the actual IP address of the Raspberry PI. If you use the AdHoc Wi-Fi network, connect to "vnc://192.168.1.1:590x" where "x" is the number you put in the commande "vncserver :x".
In case, you want to change the configuration of this wireless network (or any other network), execute the command "nano /etc/network/interfaces" to modify the "interfaces" file. DO NOT FORGET to create back-up versions.

¤ Motors are powered and controlled thanks to the GPIO (General Purpose Input / Output) pins of the Raspberry PI. Currently the simulator use the pins 17 et 23 for the control signals. A table explaining the use of each pin is provided at ReadMe/Chaire_CESEC_Report/gpio-pinout.jpg. The numbers of the pins used can be changed in the "define" area of the Scao partition (sources/Chaire_SE_Student/Scao/Scao.cpp).

