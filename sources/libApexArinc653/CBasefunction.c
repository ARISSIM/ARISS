#include "CBasefunction.h"

/**********		GUI communication   	**********/
static int create_pipe(const char* Output_pipe, const char* Input_pipe) {
    int testmkfifo;

    // communication pipes' creations	
    if ((testmkfifo = mkfifo(Output_pipe, 0777)) < 0) {
        if (errno == EEXIST) { /* file allready exist */
            fprintf(stderr, "output pipe already exist and will be used\n");
        } else {
            perror("output pipe's creation failed error :");
        }

    } else {
        fprintf(stderr, "output pipe created\n");
    }

    if ((testmkfifo = mkfifo(Input_pipe, 0777)) < 0) {
        if (errno == EEXIST) { /* file allready exist */
            fprintf(stderr, "input pipe already exist and will be used\n");
        } else {
            perror("input pipe's creation failed, error :");
        }
    } else {
        fprintf(stderr, "input pipe created\n");
    }

    return testmkfifo;

}

static int open_pipe(const char* Output_pipe, const char* Input_pipe, const char* name_partition) {
    int FD_OUTPUT; //file descriptor for input and output pipes
    int FD_INPUT;

    // Pipes' openning	
    if ((FD_OUTPUT = open(Output_pipe, O_WRONLY)) == -1) {
        perror("Fail to open output pipe, error :");
        // if impossible, suppression of the pipe
        unlink(Output_pipe);
        return (-1);
    } else {
        printf("before the dup2 for stdout and stderr in %s\n", name_partition);
        dup2(FD_OUTPUT, STDOUT_FILENO);
        dup2(FD_OUTPUT, STDERR_FILENO);
        close(FD_OUTPUT); // suppression of standard input and output to redirect it to the communication pipes			
    }

    if ((FD_INPUT = open(Input_pipe, O_RDONLY)) == -1) {
        perror("Fail to open input pipe, error  :");
        // if impossible, suppression of the pipe
        unlink(Input_pipe);
        return (-1);
    } else {
        printf("before the dup2 for stdin in %s\n", name_partition);
        if (dup2(FD_INPUT, STDIN_FILENO) == -1) {
            perror("dup2 failed, Error : ");
        }
        close(FD_INPUT); // suppression of standard input and output to redirect it to the communication pipes	
    }

    return (0);

}

/*
static int read_in_pipe(char* input_chain) {
    input_chain = '\0';

    return (read(STDIN_FILENO, input_chain, 256));

}
*/

int GUI_ARINC_partition(char* name_partition, int position, int redemarrage) {
    char name_machine[40];
    gethostname(name_machine, sizeof (name_machine));
    // Cleaning and creation of the "memory" folders (stable_memory and camera_memory)
    //    system("rm -rf stable_memory camera_memory");
    //     system("mkdir stable_memory camera_memory");
    //    bool raspi = false;
//    int i = 0;
    // communication pipes' names	
    char Output_pipe [256];
    char Input_pipe [256];
    //    char cmd[256] = "ps -eo pid,command | grep \"gui.py\" | grep -v grep | awk '{print $1}'"; //tuer le processus associé à la GUI
    //    char buf1[BUFSIZ]; //BUFSIZ est une constante connue du système	
    //   FILE *ptr;

    //   double x;
    //   std::stringstream str;



    //    if ((!strcmp(name_machine, "raspberryPI-TSF")) && redemarrage == 0) {
    //       raspi = true;
    //   }

    sprintf(Output_pipe, "Output_pipe_for_%s.fifo", name_partition);
    sprintf(Input_pipe, "Input_pipe_for_%s.fifo", name_partition);

    create_pipe(Output_pipe, Input_pipe);

    char VAR_system_instruction[256];
    sprintf(VAR_system_instruction, "./gui.py %s %s %s&", Output_pipe, Input_pipe, name_partition);
    system(VAR_system_instruction);
    printf("%s\n", name_machine);
    open_pipe(Output_pipe, Input_pipe, name_partition);

    //first thing to be print on the GUI
    printf("---------- %s\n", name_partition);

    //   if (raspi == true) printf(" (RasPI) \n");
    //   else printf(" (PC) \n");
    //   printf(" ------------\n");

    //


    //    if ((ptr = popen(cmd, "r")) != NULL) {
    //       while (fgets(buf1, BUFSIZ, ptr) != NULL) {
    //           if ((i == position) && raspi) {
    //               str << buf1;
    //               str >> x;
    //          }
    //           i = i + 1;
    //       }
    //      pclose(ptr);
    //  } else {
    //     fprintf(stderr, "Echec de popen\n");
    //  }

    //    if (strcmp(name_machine, "raspberryPI-TSF")) {
    //        if (raspi == false) {
    //            str << buf1;
    //            str >> x;
    //        }
    //    }

    //    return (x);
    return (0);
}

void GUI_ARINC_partition_kill(int pid_to_kill) {

    char VAR_system_instruction[256];
    sprintf(VAR_system_instruction, "kill -9 %d", pid_to_kill);
    system(VAR_system_instruction);
}

int GUI_ARINC_emulator() {
    // communication pipes' names	
    const char * Output_pipe = "Output_pipe_for_emulator.fifo";
    const char * Input_pipe = "Input_pipe_for_emulator.fifo";
    const char *name_emulator = "emulator";

    create_pipe(Output_pipe, Input_pipe);

    char VAR_system_instruction[256];
    sprintf(VAR_system_instruction, "./gui_for_emulator.py %s %s&", Output_pipe, Input_pipe);
    system(VAR_system_instruction);

    open_pipe(Output_pipe, Input_pipe, name_emulator);

    printf("---------- ARINC 653 emulator ------------\n"); //first thing to be print on the GUI
    return (0);
}

/**********		inter partition communication   	**********/
COMMUNICATION_VECTOR init_communication(char* argument[], char** mode) {
    COMMUNICATION_VECTOR Cvector;
    Cvector.emetteur = argument[0]; //NOM DU BINAIRE QUI SERA SON IDENFIANT POUR COMMUNIQUER
    char* arg_samp_sock = argument[1]; //ligne à découper
    char* arg_Qsock = argument[2];
    char* arg_Sport = argument[3];
    char* arg_Qport = argument[4];
    if (mode != NULL) {
        *mode = argument[5];
    }

    vector_init(&(Cvector.vsamp_socket),sizeof(int),10);
    vector_init(&(Cvector.vqueuing_socket),sizeof(int),10);
    vector_init(&(Cvector.vsamp_port),sizeof(int),10);
    vector_init(&(Cvector.vqueuing_port),sizeof(int),10);

    Cvector.vsamp_socket = split_arg(arg_samp_sock);
    Cvector.vqueuing_socket = split_arg(arg_Qsock);
    Cvector.vsamp_port = split_arg(arg_Sport);
    Cvector.vqueuing_port = split_arg(arg_Qport);
    return (Cvector);
}




