#

# To compile the java program and copy the .class in the right binary repository
compile:
	javac LibApexArinc653Jni.java
	cp LibApexArinc653Jni.class ../../../../binary/UseCases/JNI_Template/LibApexArinc653Jni.class

#  To create the .h to create the jni lib
javah:
	javah -jni LibApexArinc653Jni


# To get the functions signatures for creating the jni lib
# javap = class file disassembler
# -s = print signature
# -p = show private members
getSignatures:
	javap -s -p TypeMessage
	javap -s -p CommunicationVector
	javap -s -p LibApexArinc653Jni
