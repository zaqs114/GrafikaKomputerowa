# Dodane funkcje
* przekonwertowano bazowy projekt z wersji na Windows/VStudio na Linuxa
* zmieniono klawisze ruchu
* poruszamy się statkiem podwodnym (WSAD + ZX (góra/dół) + QE (obroty)
* stworzono tło (wielką kulę) imitujące wodę  
* utworzono podwodny układ złożony z podwodnych min, ryb różnego rodzaju, rekinów
* rekiny mają animowany (poruszający się) ogon

# Kompilacja i uruchamianie
Aby uruchomić projekt na Linuxie należy w terminalu wpisać

`sudo apt install libglu1-mesa-dev freeglut3-dev libglew-dev libglm-dev build-essential`


Następnie w folderze z projektem uruchom terminal i wprowadź komendy: 
-  `make clean`
-  `make`
-  `./main`

Jeżeli projekt nie kompiluje się `error: no matching function to call translate\rotate\scale` należy usunąć pakiet 

`sudo apt-get remove libglm-dev`<br> i pobrać go i zainstalować z [tej biblioteki](cz.archive.ubuntu.com/ubuntu/pool/main/g/glm/libglm-dev_0.9.7.2-1_all.deb)
# Uwagi
Do projektu został wykorzystany kod z ćwiczeń. [Można go znaleźć tutaj.](https://wp.faculty.wmi.amu.edu.pl/GRK.html)

# Autorzy
* Jakub Stefko
* Hubert Staszek
