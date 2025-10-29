#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]){
	if(argc < 4){
		printf("Usage: %s <points> <nodos> <input_filename> <output_filename>\n", argv[0]);
		return -1;
	}

	int cant_points = atoi(argv[1]);
	int cant_nodos = atoi(argv[2]);
	
	FILE * inputFile = fopen(argv[3], "r");
	if(inputFile == NULL){
		printf("Error! En la lectura del archivo de entrada.\n");
		return -1;
	}

	FILE *outputFile = fopen(argv[4], "w+");
 	if(outputFile == NULL){
		printf("Error! En la generación del archivo de salida.\n");
		return -1;
	}

	float x, y;
	fscanf(inputFile, "%f %f", &x, &y);
 	float xmax, xmin, ymax, ymin;
	xmax = x;
	xmin = x;
	ymax = y;
	ymin = y;

	for(int i=1; i<cant_points; i++){
		fscanf(inputFile, "%f %f", &x, &y);
		if(x < xmin){
			xmin = x;
		}else if(x > xmax){
			xmax = x;
		}

		if(y < ymin){
			ymin = y;
		}else if(y > ymax){
			ymax = y;
		}
	}
	fclose(inputFile);

	printf("****    Min / Max    ****\n");
	printf("xmin: %f\n", xmin);
	printf("xmax: %f\n", xmax);
	printf("ymin: %f\n", ymin);
	printf("ymax: %f\n", ymax);
	printf("****    Rangos    ****\n");
	printf("rango x: 0 .. %f\n", xmax - xmin);
	printf("rango y: 0 .. %f\n", ymax - ymin);

	int xfactor = (int)(cant_nodos/(xmax - xmin + 1));
	int yfactor = (int)(cant_nodos/(ymax - ymin + 1));
	int minFactor;
	if(xfactor < yfactor){
		minFactor = xfactor;
	}else{
		minFactor = yfactor;
	}

	printf("****    Factores    ****\n");
	printf("xfactor: %d\n", xfactor);
	printf("yfactor: %d\n", yfactor);
	printf("minFactor: %d\n", minFactor);
	printf("****    ---    ****\n");

	// Generando el archivo de salida

	inputFile = fopen(argv[3], "r");

	int xint, yint;
	for(int i=0; i<cant_points; i++){
		fscanf(inputFile, "%f %f", &x, &y);
		xint = (int) (minFactor * (x - xmin) + 1);
		yint = (int) (minFactor * (y - ymin) + 1);
		fprintf(outputFile,"%d %d\n",xint, yint);
		//fprintf(outputFile, "%d %d\n", xint, yint);
	}

	fclose(outputFile);
	return 0;
}
