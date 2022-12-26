#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void InitENG()
{
	cout << "++ Font creator ++" << endl;
	cout << "Version:	Alpha V0.1" << endl;
	cout << "Build date:	09/11/2020" << endl;
	cout << "This version does not represent the final product." << endl << endl;
}

void InitDEU()
{
	cout << "++ Font-Creator ++" << endl;
	cout << "Version:	Alpha V0.1" << endl;
	cout << "Build-Datum:	11.09.2020" << endl;
	cout << "Diese Version stellt nicht die finale Version dar." << endl << endl;
}

int main()
{
	string Name;
	string colortex;
	string alphatex;
	string ZeichenHGT;
	string ZeichenAbst;
	string CDZ;
	string PXL;
	string PXR;
	string HGT;
	string HGTbckp;
	//char language;
	int restart = 1;
	int anzahl = 0;
	int i = 0;
/*restart:
	cout << "Bitte Sprache ausw\204hlen." << endl;
	cout << "Please select language." << endl << endl;
	cout << "Deutsch: Tippe 'd'." << endl;
	cout << "English: Type 'e'." << endl;
	cin >> language;
	system("cls");
	if (language == 'd')
	{
		InitDEU();
		restart = 0;
	}
	else if (language == 'e')
	{
		InitENG();
		restart = 0;
	}
	else if (restart == 1)
	{
		goto restart;
	}
	*/
	cout << "\214";
	InitDEU();
	Name += ".oft";
	fstream Font;
	Font.open(Name, ios::out);
	Font << "Diese Font wurde mit dem Font-Creator erstellt." << endl;
	Font << "This font has been created with the font creator." << endl << endl;
	cout << "Der interne Name und der Name der Datei ist gleich (Angabe unter 'Name').";
	cout << endl << "Fontname: ";
	cin >> Name;
	cout << endl << "Bildtextur (mit Dateiendung!): ";
	cin >> colortex;
	cout << endl << "Alphatextur (mit Dateiendung!): ";
	cin >> alphatex;
	cout << endl << "Maximalh\224he der Zeichen (inkl. vertikalem Abstand): ";
	cin >> ZeichenHGT;
	cout << endl << "Abstand zwischen den Zeichen: ";
	cin >> ZeichenAbst;
	Font << "[newfont]" << endl << Name << endl << colortex << endl;
	Font << alphatex << endl << ZeichenHGT << endl << ZeichenAbst << endl << endl;

	cout << "Wie viele Zeichen sollen eingetragen werden?" << endl;
	cin >> anzahl;
restartletter:
	while (i != anzahl)
	{
		system("cls");
		cout << "Zeichen " << i+1 <<" von "<< anzahl << ":" << endl << endl;
		cout << "Zeichen: ";
		cin >> CDZ;
		if (CDZ == "res")
		{
			goto restartletter;
		}
		cout << "Anfangspixel: ";
		cin >> PXL;
		if (PXL == "res")
		{
			goto restartletter;
		}
		cout << "Endpixel: ";
		cin >> PXR;
		if (PXR == "res")
		{
			goto restartletter;
		}
		cout << "Oberster Pixel der Zeichenreihe: ";
		cin >> HGT;
		if (HGT == "res")
		{
			goto restartletter;
		}
		if (CDZ == "leer")
		{
			CDZ = " ";
		}
		if (HGT == "p")
		{
			HGT = HGTbckp;
		}
		else if (HGT != "p")
		{
			HGTbckp = HGT;
		}
		Font << "[char]" << endl << CDZ << endl << PXL << endl << PXR << endl << HGT << endl << endl;
		i++;
	}
	Font.close();
}