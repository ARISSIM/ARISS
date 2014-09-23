#include "Horloge.h"

Horloge::Horloge(){

	//cout << "<Horloge> Construction Horloge." << endl;

	struct tm big_bang_temp;
	
	big_bang_temp.tm_hour = 8;   big_bang_temp.tm_min = 00; big_bang_temp.tm_sec = 0;
  	big_bang_temp.tm_year = 114; big_bang_temp.tm_mon = 2; big_bang_temp.tm_mday = 1;
  	
  	big_bang_temp.tm_isdst = -1; // flag pour ignorer le changement d'heure
  	
  	// 21 jan 2014 a 8h00
  	
  	big_bang= mktime(&big_bang_temp);
	

}

/*void Horloge::affichage(){

	cout << "<Horloge> Fonction Affichage" << endl;
	cout << "<Horloge> Test Big Bang = " << test_big_bang << endl;
	cout << "<Horloge> Big Bang = " << big_bang << endl;

};*/

double Horloge::get_temps(){ // Temps depuis Big Bang

	time_t temps_actuel;
	double seconds;

	time(&temps_actuel);
	
	seconds = difftime(temps_actuel,big_bang);
	
	return seconds;

}

double Horloge::date2seconds( struct tm p_temps ){

	double seconds;
	
	time_t p_temps_t;
	
	p_temps.tm_isdst = -1; // flag pour ignorer le changement d'heure
	
	p_temps_t = mktime(&p_temps);

  	seconds = difftime(p_temps_t, big_bang);

  	/*cout << "<Horloge> Temps avant mktime : " << asctime(&p_temps) << endl;
   	cout << "<Horloge> Temps avant difftime : " << ctime(&p_temps_t) << " apres difftime " << seconds << endl;
   	cout << "<Horloge> Big bang : " << ctime(&big_bang) << endl;*/
  	
  	return seconds;

}
