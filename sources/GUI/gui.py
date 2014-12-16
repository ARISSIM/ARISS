#!/usr/bin/python
# -*- coding: iso-8859-1 -*-

#python 2.x
#Import
try:
    from Tkinter import *
except ImportError:
    raise ImportError,"The Tkinter module is required to run this program. run 'sudo apt-get install python-tk' to install the package"

from ScrolledText import *
from tkMessageBox import *
import os
import select
import tkFileDialog
import time
import sys

class gui_for_ARINC_653(Tk):

	def __init__(self,parent):
		global input_pipe
		global output_pipe
		Tk.__init__(self,parent)
		self.parent = parent
		self.initialisation()
		self.readfunction()
		
		


	def initialisation(self):
		global input_pipe
		global output_pipe
		if len(sys.argv) > 2 :
			print 'output_pipe : ', output_pipe
		global ChaineALire
		global st
		global fp
		global sortieTube
		global entreeTube
					
		def click_python(event):
			st.insert(END,'\nVous avez cliquez sur Python')

		def affiche_message():
			global output_pipe
			message = entre.get()
			
			#openning the OUTPUT pipe
			if len(sys.argv) > 2 : #if an output pipe was given as the second argument
				try:
					assert output_pipe != ''
					#opening the pipe
					try :
						entreeTube = os.open(output_pipe,os.O_WRONLY)
						if entreeTube == -1 :
							raise ValueError
						os.write(entreeTube,message)
						#st.insert(END,'\n'+message,'rouge')						
					except ValueError :
						showerror('Error', "Could not open Output pipe")
				except AssertionError:
					showerror("Error", "You must choose an output pipe first")
					output_pipe = tkFileDialog.askopenfilename(title='Choose the output pipe to open', defaultextension ='.fifo')

		texte = """ Welcome in GUI for ARINC 653 Partition """

		#on cree un fenetre principale 'fp' a� partir du 'modele' Tk   
		#fp = Tk()
		#self.title('ARINC 653 function')
		self.title('ARINC 653 function : ' + sys.argv[3])
	
		#Creation d'un panneau 'menu_top' insere dans 'fp'
		menu_top = Frame(self,borderwidth=2)
		menu_top.pack(side='bottom',fill='x')

		#Creation d'une etiquette inseree dans 'menu_top'
		etiquette = Label(menu_top,text = 'Commande : ')
		etiquette.pack(side = 'left')

		#Creation d'un boutton dans 'menu_top'
		b = Button(menu_top,text='execute',command=affiche_message)
		b.pack(side='right')

		#Creation d'un boite d'edition dans 'nenu_top'
		entre = Entry(menu_top,width=20,relief='sunken')
		entre.insert(END,'Texte a afficher')
		entre.pack(side='left',fill='x',expand='true')
	

	
		#Declaration du texte deroulant dans 'fp'
		st = ScrolledText(self)
		st.pack(expand=1,fill='both',side='top')

		#on insete le texte 'texte' dans le widget 'texte deroulant' nomme 'st'
		st.insert(END,texte)
		#on applique a� tout le texte la police par defaut
		st.config(font = 'Arial 12 bold')
		st.config(foreground='Black')
		#on configure le fond du 'texte derouant'
		
		if ((sys.argv[3] == "Master") or (sys.argv[3] == "Partition1")or (sys.argv[3] == "Primary")):
			st.config(background='LightYellow')
		elif ((sys.argv[3] =="Slave") or (sys.argv[3] == "Partition2")or (sys.argv[3] == "Backup")):
			st.config(background='LightGreen')
		#e = Entry(st,width=25)
		elif ((sys.argv[3] =="Leica") or (sys.argv[3] == "Partition3")or (sys.argv[3] == "Capteur")):
		        st.config(background='misty rose')
		else: 
		        st.config(background='PeachPuff')
		try:
			assert input_pipe != ''
		
		except AssertionError:
			showerror('Info', "choisissez un chemin de tube")
			input_pipe = tkFileDialog.askopenfilename(title='Choose the input pipe to open', defaultextension ='.fifo')

		#openning the INPUT pipe
		try:
			assert input_pipe != ''	

			try :

	#			sortieTube = os.open(input_pipe+nomTube,os.O_RDONLY|os.O_NONBLOCK)
				sortieTube = os.open(input_pipe,os.O_RDONLY)
				if sortieTube == -1 :
					raise ValueError
			except ValueError :		
				showerror('Error', "fail to open the Input pipe's output")
		except AssertionError:
			showerror("Error", "You must choose a pipe first")


		#openning the OUTPUT pipe
		if len(sys.argv) > 2 : #if an output pipe was given as the second argument
			try:
				assert output_pipe != ''

				#opening the pipe
				try :
					entreeTube = os.open(output_pipe,os.O_WRONLY)
			
					if entreeTube == -1 :
						raise ValueError
				except ValueError :
					showerror('Error', "Could not open Output pipe")
			except AssertionError:
				showerror("Error", "You must choose an output pipe first")
				output_pipe = tkFileDialog.askopenfilename(title='Choose the output pipe to open', defaultextension ='.fifo')


		#Positionnement de la fenetre dans l'ecran
		ScreenSizeX = self.winfo_screenwidth()  # Get screen width [pixels]
		ScreenSizeY = self.winfo_screenheight() # Get screen height [pixels]
		CorrectionX = 30
		CorrectionY = 30
		FrameSizeX  = int(ScreenSizeX * 0.5) - CorrectionX
		FrameSizeY  = int(ScreenSizeY * 0.5) - CorrectionY

		if ((sys.argv[3] == "Master") or (sys.argv[3] == "Partition1")) : # Find left and up border of window
			FramePosX   = 0
			FramePosY   = 0
		elif ((sys.argv[3] == "Slave") or (sys.argv[3] == "Partition2")) :
			FramePosX   = 0
			FramePosY   = ScreenSizeY/2 + CorrectionY
		elif sys.argv[3] =="Scao":
			FramePosX   = ScreenSizeX/2 + CorrectionX 
			FramePosY   = 0
		else: 
			FramePosX   = ScreenSizeX/2 + CorrectionX 
			FramePosY   = ScreenSizeY/2 + CorrectionY

		self.geometry("%sx%s+%s+%s"%(FrameSizeX,FrameSizeY,FramePosX,FramePosY))



	def readfunction(self):
		global ChaineALire
		global sortieTube
		global st
		global fp		
		ChaineALire = StringVar()
		ChaineALire = False
	
		r,w,x = select.select([sortieTube],[],[],0.05)
		if r:
			ChaineALire = os.read(sortieTube,256)
			st.insert(END,ChaineALire)
			st.see(END)

		time.sleep(0.01)
		self.after(10,self.readfunction) # => boucler apres 10 millisecondes
		




#declanchement des operations
if __name__ == "__main__":
	global input_pipe
	global output_pipe
	output_pipe=""
	input_pipe = sys.argv[1]
	if len(sys.argv) > 2 :
		output_pipe = sys.argv[2]
	print 'sys.argv : ', sys.argv
	app = gui_for_ARINC_653(None)
	#app.readfunction()

	app.mainloop()

#	if len(sys.argv) > 1:
#		message(sys.argv[1])
#	else:
#		message('No argument')
