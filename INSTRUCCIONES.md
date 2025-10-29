# Universidad del Bio-Bio
# Proyecto de Titulación
    Proyecto de Titulación 2022 Esteban Risopatrón

# Nota importante
    Este programa fue hecho en C++ 14.

# Como usar 

-en la carpeta CPP files utilize su compilador de c++, tanto como linux y windows se utiliza GNU g++ y compile los archivos
	
	g++ -o "nombre del ejecutable" -O3 "archivo.cpp"

si quiere,puede hacer un script para acaparar todos de una, es recomendable.
se usa O3, argumento de compilación de mayor rendimiento, trate de usar mas argumentos de compilación y experimente con aquello,
use este link para ver la documentación del compilador: https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html

*el programa no ha sido probado con el compilador clang, por lo que a lo mejor los resultados pueden variar.

uso de LatPoint.c

*deje el archivo de texto en CPP files a cual quiera transformar de lat,long a una normalizada para poder ser usada por "fstream.cpp" .

./["nombre ejecutable"] ["cantidad puntos"] ["nodos"(tamaño matriz)] ["archivo.txt"(Entrada)] ["archivo.txt"(Salida)]

*si supera los cantidad de puntos del archivo de texto, tendrá errores, trate de no hacer eso porfavor.

uso de test_knn.cpp

./["nombre ejecutable"] ["archivo.DAT"] ["numero de vecinos"] ["x"] ["y"]

uso de knn_with_query.cpp

./["nombre ejecutable"] ["archivo.DAT"] ["numero de vecinos"] ["QUERYSET"]

*el queryset debe estar en la carpeta Pruebas/Query_Point y debe ser un archivo de texto.

uso de fstream.cpp

./["nombre ejecutable"]  ["archivo.txt"] [x] [y]

*el archivo txt son los que estan en la carpeta Pruebas/Datasets
*por lo tanto este programa los compactara, realizará el k2tree y lo guardará de forma compactada en Pruebas/Compactados
*asi sera más rápida la ejecución.

uso de checklink.cpp 
./["nombre ejecutable"]  ["archivo.DAT"] [x] [y]

uso de range 
./["nombre ejecutable"]  ["archivo.DAT"] [x1] [x2] [y1] [y2]




ORDEN DE EJECUCIÓN (para datos reales):

 Latpoint.c ==> Fstream.cpp ==> ( test_knn.cpp || knn_with_query.cpp ) 

ORDEN DE EJECUCIÓN (para datos sinteticos):
 Fstream.cpp ==> ( test_knn.cpp || knn_with_query.cpp ) 


*una vez realizado fstream lo unico que se hace es llamar a test o a knn y punto.



NOTAS FINALES: si quiere probar Java use los 2 scripts de shell en ./Pruebas_Java 


# Consultas
    esteban.risopatron1801@alumnos.ubiobio.cl
