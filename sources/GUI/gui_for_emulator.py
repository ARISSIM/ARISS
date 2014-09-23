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
		global colorbox
		global sortieTube
		global entreeTube



		texte = """ Welcome in GUI for ARINC 653 emulator 
"""

		#on cree un fenetre principale 'fp' a� partir du 'modele' Tk   
		#fp = Tk()
		self.title('ARINC 653 Partition')	

		#Declaration du texte deroulant dans 'fp'
		st = ScrolledText(self,wrap="word")
		st.grid(column=1,columnspan=2,rowspan=2,sticky='nw')

		#Creation d'un panneau 'menu_bottom' insere dans 'fp'
#		menu_bottom = Frame(self,borderwidth=2)
#		menu_bottom.grid(column=1,columnspan=3,sticky='sw')

		##Creation d'une etiquette inseree dans 'menu_bottom'
		#etiquette = Label(menu_bottom,text = 'Commande : ')
		#etiquette.grid(row=1,column=1,sticky='w')

		##Creation d'un boutton dans 'menu_bottom'
		#b = Button(menu_bottom,text='execute',command=self.affiche_commande)
		#b.grid(row=1,column=3,sticky='e')

		##Creation d'un boite d'edition dans 'nenu_top'
		#entre = Entry(menu_bottom,width=20,relief='sunken')
		#entre.insert(END,'Texte a afficher')
		#entre.grid(row=1,column=2,sticky='ew')
			
		#Creation d'une etiquette inseree
		etiquette = Label(self,text = 'Command : ')
		etiquette.grid(row=2,column=0,sticky='w')			
		
		#Creation d'un boutton
		b = Button(self,text='execute',command=self.affiche_commande)
		b.grid(row=2,column=2,sticky='e')

		#Creation d'un boite d'edition dans 'nenu_top'
		entre = Entry(self,width=20,relief='sunken')
		entre.insert(END,'Text to send')
		entre.grid(row=2,column=1,sticky='ew')
		
		partitionbox = Label(self ,text = 'Active partition : ')
		partitionbox.grid(column=0,row=0,sticky='n')
		
		self.labelVariable = StringVar()
		self.backcolorlabel = StringVar()
		self.frontcolorlabel = StringVar()
		self.backcolorlabel = "LightBlue"
		self.frontcolorlabel = "Black"
		label = Label(self,textvariable=self.labelVariable,
			anchor="w",fg=self.frontcolorlabel,bg=self.backcolorlabel,height=28)
		label.grid(column=0,row=1,sticky='EW')
		


		#on insete le texte 'texte' dans le widget 'texte deroulant' nomme 'st'
		st.insert(END,texte)
		#on applique a� tout le texte la police par defaut
		st.config(font = 'Arial 12')
		st.config(foreground='Black')
		#on configure le fond du 'texte derouant'
		st.config(background='LightBlue')
		#e = Entry(st,width=25)


		self.grid_columnconfigure(1,weight=1)
		self.rowconfigure(1,weight=1)
		
		self.resizable(False,True)
		self.update()
		self.geometry(self.geometry())
		
		try:
			assert input_pipe != ''
		
		except AssertionError:
			showerror('error', "no pipe choosed")
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
				
	
		
	def affiche_commande(self):
		global output_pipe
   		commande = entre.get()
   		
   		#openning the OUTPUT pipe
		if len(sys.argv) > 2 : #if an output pipe was given as the second argument
			try:
				assert output_pipe != ''
				#opening the pipe
				try :
					entreeTube = os.open(output_pipe,os.O_WRONLY)
					if entreeTube == -1 :
						raise ValueError
					os.write(entreeTube,commande)
					#st.insert(END,'\n'+commande,'rouge')						
				except ValueError :
					showerror('Error', "Could not open Output pipe")
			except AssertionError:
				showerror("Error", "You must choose an output pipe first")
				output_pipe = tkFileDialog.askopenfilename(title='Choose the output pipe to open', defaultextension ='.fifo')			




	def click_python(self, event):
		st.insert(END,'\nVous avez cliquez sur Python')

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
			#st.see(END)

		time.sleep(0.01)
		self.after(10,self.readfunction) # => boucler apres 10 millisecondes
		




#declanchement des operations
if __name__ == "__main__":
	global input_pipe
	global output_pipe
	output_pipe=""
	input_pipe=""
	if len(sys.argv) >  1:
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
