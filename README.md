# Heuristic_algorithm_finding_clique_in_graph
 Searching for common motif in DNA sequence

1.	Problem:

Wyszukanie w grafie kliki (struktury do niej zbliżonej) przy pomocy algorytmu heurystycznego. Kliką w grafie nazwiemy jego podgraf, którego wszystkie wierzchołki są ze sobą nawzajem połączone krawędziami. Innymi słowy, klika jest podgrafem pełnym danego grafu.

2.	Implementacja:

•	Na podstawie dwóch plików zawierających – 5 wybranych ze zbioru sekwencji nukleotydowych i 5 odpowiadającym im ocenom wiarygodności wystąpienia każdego nukleotydu na danej pozycji, zostaje stworzona instancja testowa.
•	Z każdej sekwencji, dla której po wczytaniu tworzymy jej obiekt przechowujący wszystkie dane na jej temat, usuwamy nukleotydy o poziomie wiarygodności poniżej wartości pożądanej przez użytkownika (może on wybrać wartość od 0 do 40). Użytkownik jest także proszony jest o podanie długości podciągu sekwencji (wartość 4-9). 
•	W każdej sekwencji zapewnione jest istnienie motywu o długości kilkunastu nukleotydów. W wybranych miejscach motywy różnią się od siebie na przestrzeni do 2 znaków. 
•	Na tym etapie stworzony zostaje graf. Przechodzimy po pięciu sekwencjach tworząc wierzchołki o podanej długości - każde wystąpienie danego podciągu to osobny wierzchołek w grafie. 
•	W grafie łączymy ze sobą wierzchołki nieskierowanymi krawędziami, jeśli są one identycznymi podciągami, występującymi w różnych sekwencjach. 

3.	Wyszukanie kliki:

•	Algorytm heurystyczny wyszukuje klikę w utworzonym grafie. Zaczynając od pierwszego stworzonego wierzchołka sprawdzamy jego 
sąsiadów (wszystkie krawędzie odwiedzanego wierzchołka). Sprawdzone wierzchołki zapisujemy do wektora je przechowującego. Jeśli wierzchołek obecnie analizowany nie został jeszcze sprawdzony i nie mamy wierzchołka z tej samej sekwencji dodajemy go do listy i powtarzamy proces. Koniec przeszukiwania ma miejsce gdy nie ma już krawędzi prowadzących do nowego (nieodwiedzonego) wierzchołka. Funkcja zwraca wektor z dopasowanymi wierzchołkami (gdy to możliwe po jednym z każdej sekwencji) - finalnie otrzymujemy klikę maksymalną.

4.	Wyświetlenie wyniku:

•	Program wypisuje informację o znalezionej klice: fragment sekwencji reprezentującej wierzchołek, numer sekwencji z której ona pochodzi oraz jej pozycję w sekwencji wyjściowej.

5.	Szacowana złożoność algorytmu: O(n^3)   n ~ ilość wierzchołków


6.	Wnioski:

Program został przetestowany na różnych instancjach wejściowych. Długość podciągu może wpłynąć na brak niektórych krawędzi w grafie, ma także wpływ na różnice w powtarzającym się motywie. Wyższy próg wiarygodności skutkuje trudnością w znalezieniu powtarzających się fragmentów.
