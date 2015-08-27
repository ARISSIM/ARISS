make :
# compile java application
	javac -sourcepath /home/ics/ARISS/ARISS/sources/UseCases/JNI_Template/javaCode/ TestJNI.java
# copy .class in the right repository
	cp TestJNI.class ../../../../binary/UseCases/JNI_Template/TestJNI.class
	cp /libapexarinc653jni/ReturnObject.class ../../../../binary/UseCases/JNI_Template/ReturnObject.class
	cp /libapexarinc653jni/LibApexArinc653Jni.class ../../../../binary/UseCases/JNI_Template/LibApexArinc653Jni.class
#copy librairies
	sudo cp ../../../../lib/libApexArinc653Jni.so /usr/lib/jvm/java-8-oracle/jre/bin/libApexArinc653Jni.so
	sudo cp ../../../../lib/libApexArinc653.so /usr/lib/jvm/java-8-oracle/jre/bin/libApexArinc653.so


#

## To compile the java program and copy the .class in the right binary repository*
# javac className.java
# don't forget to copy the .class in the right binary use case dir
# cp LibApexArinc653Jni.class ../../../../binary/UseCases/JNI_Template/LibApexArinc653Jni.class

#  To create the .h to create the jni lib
# javah -jni className

# To get the functions signatures for creating the jni lib
# javap = class file disassembler
# -s = print signature
# -p = show private members
# javap -s -p className
